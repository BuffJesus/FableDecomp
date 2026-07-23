//=== CEditWorldMap__SetSaveToBackupFiles @ 0204b6e0 ===

/* [ported from ego_r via strfp] */

void _SetSaveToBackupFiles_CEditWorldMap__QAEX_N_Z(undefined1 param_1)

{
  int in_ECX;
  
  *(undefined1 *)(in_ECX + 0x17d) = param_1;
  return;
}



//=== CEditWorldMap__LoadSingleLevelFromFiles @ 0207fed0 ===

/* [ported from ego_r via strfp] */

void _LoadSingleLevelFromFiles_CEditWorldMap__QAEXJV__CCountedPointer_VCAFile____0_Z
               (undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  undefined4 uStack_70;
  undefined1 *puStack_6c;
  undefined4 uStack_68;
  undefined1 *puStack_64;
  undefined1 *puStack_60;
  undefined1 *puStack_5c;
  char *pcStack_58;
  wchar_t *pwStack_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  int *local_3c;
  undefined1 *local_38;
  undefined1 *local_34;
  undefined1 *local_30;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_03ce8f88;
  local_10 = ExceptionList;
  local_8 = 1;
  pwStack_54 = (wchar_t *)param_1;
  pcStack_58 = (char *)0x207ff04;
  ExceptionList = &local_10;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  if (*(char *)(iVar3 + 0x2c) == '\0') {
    pwStack_54 = 
    L"e:\\fabletlc\\fable tlc build repository\\bbbprojects\\fable1_5mainpc\\fablelib\\edit_world_map.hpp"
    ;
    pcStack_58 = (char *)0x207ff1d;
    __0CWideString__QAE_PB_W_Z();
    local_8._0_1_ = 2;
    pwStack_54 = (wchar_t *)0xffffffff;
    pcStack_58 = "";
    puStack_5c = (undefined1 *)0x207ff30;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 3;
    pwStack_54 = (wchar_t *)0xffffffff;
    pcStack_58 = "Maps[map_no].Used";
    puStack_5c = (undefined1 *)0x207ff43;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 4;
    pwStack_54 = (wchar_t *)0x1;
    pcStack_58 = (char *)0xe7;
    puStack_5c = local_1c;
    puStack_60 = local_24;
    puStack_64 = local_2c;
    uStack_68 = 0x207ff5f;
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    uStack_68 = 0x207ff66;
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    uStack_68 = 0x207ff6d;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      ();
    local_11 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 3;
    pwStack_54 = (wchar_t *)0x207ff89;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 2;
    pwStack_54 = (wchar_t *)0x207ff95;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,1);
    pwStack_54 = (wchar_t *)0x207ffa1;
    __1CWideString__QAE_XZ();
    if (local_11 != '\0') {
      pwStack_54 = (wchar_t *)0x207ffaf;
      BVar4 = IsDebuggerPresent();
      if (BVar4 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  pwStack_54 = (wchar_t *)param_1;
  pcStack_58 = (char *)0x207ffc5;
  cVar2 = (**(code **)(*local_3c + 0x28))();
  if (cVar2 != '\0') {
    pcStack_58 = (char *)param_1;
    puStack_5c = (undefined1 *)0x207ffd8;
    _UnloadLevel_CWorldMap__IAEXJ_Z();
  }
  pcStack_58 = (char *)0x0;
  local_30 = (undefined1 *)&puStack_60;
  puStack_64 = &stack0x00000010;
  uStack_68 = 0x207ffeb;
  local_44 = __0__CCountedPointer_VCAFile____QAE_ABV0__Z();
  local_8._0_1_ = 5;
  local_34 = (undefined1 *)&uStack_68;
  puStack_6c = &stack0x00000008;
  uStack_70 = 0x2080009;
  local_40 = local_44;
  local_4c = __0__CCountedPointer_VCAFile____QAE_ABV0__Z();
  local_8._0_1_ = 6;
  local_38 = (undefined1 *)&uStack_70;
  local_48 = local_4c;
  local_50 = __0__CCountedPointer_VCAFile____QAE_ABV0__Z(&stack0x00000008);
  local_8._0_1_ = 1;
  _LoadLevelFromFiles_CWorldMap__IAEXJV__CCountedPointer_VCAFile____00W4ELoadType_1__Z(param_1);
  local_8 = (uint)local_8._1_3_ << 8;
  pcStack_58 = (char *)0x2080046;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  local_8 = 0xffffffff;
  pcStack_58 = (char *)0x2080055;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__SetLogText @ 028c02b0 ===

/* [ported from ego_r via strfp] */

void _SetLogText_CEditLevelMerger__QAEXPAV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
               (undefined4 param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0xd8) = param_1;
  return;
}



//=== CEditLevelMerger__SetOurText @ 028c02d0 ===

/* [ported from ego_r via strfp] */

void _SetOurText_CEditLevelMerger__QAEXPAV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
               (undefined4 param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0xdc) = param_1;
  return;
}



//=== CEditLevelMerger__SetTheirText @ 028c02f0 ===

/* [ported from ego_r via strfp] */

void _SetTheirText_CEditLevelMerger__QAEXPAV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
               (undefined4 param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0xe0) = param_1;
  return;
}



//=== CEditLevelMerger__GetConflicts @ 028c0430 ===

/* [ported from ego_r via strfp] */

int _GetConflicts_CEditLevelMerger__QAEAAV__vector_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std__XZ
              (void)

{
  int in_ECX;
  
  return in_ECX + 0x44;
}



//=== CEditLevelMerger__4CConflict @ 028c16b0 ===

/* [ported from ego_r via strfp] */

int __4CConflict_CEditLevelMerger__QAEAAV01_ABV01__Z(int param_1)

{
  int in_ECX;
  
  __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(param_1);
  __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(param_1 + 0x1c);
  *(undefined1 *)(in_ECX + 0x38) = *(undefined1 *)(param_1 + 0x38);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  return in_ECX;
}



//=== CEditLevelMerger__4CThingText @ 028c1710 ===

/* [ported from ego_r via strfp] */

int __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(int param_1)

{
  int in_ECX;
  
  __4CCharString__QAEABV0_ABV0__Z(param_1);
  __4CCharString__QAEABV0_ABV0__Z(param_1 + 8);
  *(undefined4 *)(in_ECX + 0x10) = *(undefined4 *)(param_1 + 0x10);
  __4CCharString__QAEABV0_ABV0__Z(param_1 + 0x14);
  return in_ECX;
}



//=== CEditLevelMerger___GCConflict @ 028c1800 ===

/* [ported from ego_r via strfp] */

void * ___GCConflict_CEditLevelMerger__QAEPAXI_Z(uint param_1)

{
  void *in_ECX;
  
  __1CConflict_CEditLevelMerger__QAE_XZ();
  if ((param_1 & 1) != 0) {
    operator_delete(in_ECX);
  }
  return in_ECX;
}



//=== CEditLevelMerger__1CConflict @ 028c1840 ===

/* [ported from ego_r via strfp] */

void __1CConflict_CEditLevelMerger__QAE_XZ(void)

{
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_03dda788;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  __1CThingText_CEditLevelMerger__QAE_XZ();
  local_8 = 0xffffffff;
  __1CThingText_CEditLevelMerger__QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__1CThingText @ 028c18a0 ===

/* [ported from ego_r via strfp] */

void __1CThingText_CEditLevelMerger__QAE_XZ(void)

{
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  puStack_c = &LAB_03dda7b3;
  local_10 = ExceptionList;
  local_8 = 1;
  ExceptionList = &local_10;
  __1CCharString__QAE_XZ();
  local_8 = local_8 & 0xffffff00;
  __1CCharString__QAE_XZ();
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== CEditWorldMap__PostRegionLoad @ 0296a6d0 ===

/* [ported from ego_r via strfp] */

void _PostRegionLoad_CEditWorldMap__UAEXJ_Z(void)

{
  return;
}



//=== CEditWorldMap__AddLevelFromFileWithUID @ 0296ade0 ===

/* [ported from ego_r via strfp] */

int _AddLevelFromFileWithUID_CEditWorldMap__QAEJABVC2DCoordI__ABVCCharString__K_Z
              (undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  int iVar4;
  
  iVar1 = _AddLevelFromFile_CWorldMap__IAEJABVC2DCoordI__ABVCCharString___Z(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ();
    _resize___vector_VCEditMapInfo__V__allocator_VCEditMapInfo___std___std__QAEXI_Z(uVar2);
    puVar3 = (undefined1 *)
             __A__vector_VCEditMapInfo__V__allocator_VCEditMapInfo___std___std__QAEAAVCEditMapInfo__I_Z
                       (iVar1);
    *puVar3 = 0;
    if (param_3 != 0) {
      iVar4 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(iVar1);
      *(int *)(iVar4 + 0x28) = param_3;
    }
  }
  return iVar1;
}



//=== CEditWorldMap__SaveToFile @ 0296b5e0 ===

/* [ported from ego_r via strfp] */

void _SaveToFile_CEditWorldMap__UAEXAAVCAFile___Z(undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  BOOL BVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined1 local_58 [8];
  undefined1 local_50 [11];
  char local_45;
  undefined1 local_34 [8];
  undefined1 local_2c [8];
  CAtlWinModule local_24 [20];
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df2388;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0__vector_JV__allocator_J_std___std__QAE_XZ();
  local_8 = 0;
  _GetRegionlessMaps_CEditWorldMap__ABEXAAV__vector_JV__allocator_J_std___std___Z(local_24);
  cVar2 = _empty___vector_JV__allocator_J_std___std__QBE_NXZ();
  if (cVar2 == '\0') {
    __0CCharString__QAE_PBDJ_Z(&DAT_04349988,0xffffffff);
    local_8._0_1_ = 1;
    iVar3 = _size___vector_JV__allocator_J_std___std__QBEIXZ();
    if (iVar3 == 1) {
      __YCCharString__QAEABV0_PBD_Z(&DAT_0434998c);
      uVar4 = _GFIntVectorToCharString__YI_AVCCharString__ABV__vector_JV__allocator_J_std___std___Z
                        ();
      local_8._0_1_ = 2;
      __YCCharString__QAEABV0_ABV0__Z(uVar4);
      local_8 = CONCAT31(local_8._1_3_,1);
      __1CCharString__QAE_XZ();
      __YCCharString__QAEABV0_PBD_Z(" is not a member of a region.");
    }
    else {
      __YCCharString__QAEABV0_PBD_Z(&DAT_043499b0);
      uVar4 = _GFIntVectorToCharString__YI_AVCCharString__ABV__vector_JV__allocator_J_std___std___Z
                        ();
      local_8._0_1_ = 3;
      __YCCharString__QAEABV0_ABV0__Z(uVar4);
      local_8 = CONCAT31(local_8._1_3_,1);
      __1CCharString__QAE_XZ();
      __YCCharString__QAEABV0_PBD_Z(" are not members of a region.");
    }
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8._0_1_ = 4;
    __0CCharString__QAE_PBDJ_Z("",0xffffffff);
    local_8._0_1_ = 5;
    uVar9 = 2;
    uVar4 = 0x12f;
    puVar8 = local_50;
    puVar7 = local_34;
    puVar6 = local_58;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x12f,2);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar6,puVar7,puVar8,uVar4,uVar9);
    local_45 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 4;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,1);
    __1CWideString__QAE_XZ();
    if (local_45 != '\0') {
      BVar5 = IsDebuggerPresent();
      if (BVar5 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    local_8 = local_8 & 0xffffff00;
    __1CCharString__QAE_XZ();
  }
  __0CCharString__QAE_XZ();
  local_8._0_1_ = 6;
  _SaveMapsToString_CWorldMap__IAEXAAVCCharString___Z(local_2c);
  _EditSaveRegionsToString_CEditWorldMap__AAEXAAVCCharString___Z(local_2c);
  _SaveToFile_CCharString__QBEXAAVCAFile___Z(param_1);
  local_8 = (uint)local_8._1_3_ << 8;
  __1CCharString__QAE_XZ();
  local_8 = 0xffffffff;
  ATL::CAtlWinModule::~CAtlWinModule(local_24);
  ExceptionList = local_10;
  return;
}



//=== CEditWorldMap__EditSaveRegionsToString @ 0296b850 ===

/* WARNING: Removing unreachable block (ram,0x0296c8f1) */
/* WARNING: Removing unreachable block (ram,0x0296c2f3) */
/* WARNING: Removing unreachable block (ram,0x0296b974) */
/* WARNING: Removing unreachable block (ram,0x0296c61c) */
/* [ported from ego_r via strfp] */

void _EditSaveRegionsToString_CEditWorldMap__AAEXAAVCCharString___Z(void)

{
  code *pcVar1;
  undefined1 uVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  BOOL BVar6;
  uint uVar7;
  float *pfVar8;
  undefined4 *puVar9;
  char *pcVar10;
  float10 fVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  char *pcVar14;
  char *pcVar15;
  undefined8 uVar16;
  char *pcVar17;
  undefined4 uVar18;
  undefined *puVar19;
  char *pcVar20;
  undefined1 local_8ac [16];
  undefined1 local_89c [15];
  char local_88d;
  undefined1 local_880 [44];
  undefined1 local_854 [16];
  undefined1 local_844 [15];
  char local_835;
  undefined1 local_7e4 [15];
  char local_7d5;
  undefined1 local_6b4 [8];
  undefined1 local_6ac [8];
  undefined1 local_6a4 [11];
  char local_699;
  char local_698 [268];
  undefined4 local_58c;
  undefined4 local_588;
  int local_584;
  char local_580 [264];
  char local_478 [264];
  char local_370 [268];
  undefined4 local_264;
  undefined4 local_260;
  int local_25c;
  char local_258 [264];
  char local_150 [264];
  undefined1 local_48 [12];
  undefined1 local_3c [12];
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df2690;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  iVar4 = _size___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QBEIXZ();
  iVar5 = _size___vector_VCRegion__V__allocator_VCRegion___std___std__QBEIXZ();
  if (iVar4 != iVar5) {
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 2;
    uVar18 = 1;
    uVar16 = CONCAT44(0x13c,local_6a4);
    puVar13 = local_6ac;
    puVar12 = local_6b4;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar12,puVar13,local_6a4,0x13c,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar12,puVar13,uVar16,uVar18);
    local_699 = '\x01' - (cVar3 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_699 != '\0') && (BVar6 = IsDebuggerPresent(), BVar6 != 0)) {
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
  }
  local_14 = 1;
  local_18 = 1;
  while (uVar7 = _size___vector_VCRegion__V__allocator_VCRegion___std___std__QBEIXZ(),
        local_18 < uVar7) {
    __A__vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QAEAAVCEditRegion__I_Z
              (local_18);
    cVar3 = _IsUsedByEditor_CEditRegion__QBE_NXZ();
    if (cVar3 != '\0') {
      local_28 = __A__vector_VCRegion__V__allocator_VCRegion___std___std__QAEAAVCRegion__I_Z
                           (local_18);
      puVar19 = &DAT_04349a48;
      _GFIntToCharString__YI_AVCCharString__J_Z(&DAT_04349a48);
      local_8 = 3;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 4;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 5;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,3);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349a58;
      _GetName_CRegion__QBEABVCCharString__XZ(&DAT_04349a58);
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8 = 6;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 7;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8 = CONCAT31(local_8._1_3_,6);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349a70;
      _GetDisplayName_CRegion__QBEABVCCharString__XZ(&DAT_04349a70);
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8 = 8;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 9;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8 = CONCAT31(local_8._1_3_,8);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349a8c;
      _GetRegionDefName_CRegion__QBEABVCCharString__XZ(&DAT_04349a8c);
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8 = 10;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0xb;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8 = CONCAT31(local_8._1_3_,10);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      cVar3 = _IsOnWorldMap_CRegion__QBE_NXZ();
      if (cVar3 != '\0') {
        __YCCharString__QAEABV0_PBD_Z("AppearOnWorldMap;\r\n");
      }
      puVar19 = &DAT_043498a3;
      _GetMiniMapGraphic_CRegion__QBEABVCCharString__XZ(&DAT_043498a3);
      cVar3 = __9CCharString__QBE_NPBD_Z(puVar19);
      if (cVar3 != '\0') {
        puVar19 = &DAT_04349ab4;
        _GetMiniMapGraphic_CRegion__QBEABVCCharString__XZ(&DAT_04349ab4);
        __H_YI_AVCCharString__PBDABV0__Z();
        local_8 = 0xc;
        uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
        local_8._0_1_ = 0xd;
        __YCCharString__QAEABV0_ABV0__Z(uVar18);
        local_8 = CONCAT31(local_8._1_3_,0xc);
        __1CCharString__QAE_XZ();
        local_8 = 0xffffffff;
        __1CCharString__QAE_XZ();
      }
      puVar19 = &DAT_04349ac8;
      fVar11 = (float10)_GetMiniMapScale_CRegion__QBEMXZ(&DAT_04349ac8);
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)fVar11);
      local_8 = 0xe;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0xf;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x10;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0xf;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0xe);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349adc;
      pfVar8 = (float *)_GetMiniMapOffset_CRegion__QBEABVC2DVector__XZ(&DAT_04349adc);
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*pfVar8);
      local_8 = 0x11;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x12;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x13;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x12;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x11);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349af0;
      iVar4 = _GetMiniMapOffset_CRegion__QBEABVC2DVector__XZ(&DAT_04349af0);
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*(float *)(iVar4 + 4));
      local_8 = 0x14;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x15;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x16;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x15;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x14);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349b04;
      pfVar8 = (float *)_GetWorldMapOffset_CRegion__QBE_BVC2DVector__XZ();
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*pfVar8);
      local_8 = 0x17;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x18;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x19;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x18;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x17);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349b1c;
      iVar4 = _GetWorldMapOffset_CRegion__QBE_BVC2DVector__XZ();
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*(float *)(iVar4 + 4));
      local_8 = 0x1a;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x1b;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x1c;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x1b;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x1a);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349b34;
      pfVar8 = (float *)_GetNameGraphicOffset_CRegion__QBE_BVC2DVector__XZ();
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*pfVar8);
      local_8 = 0x1d;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x1e;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x1f;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x1e;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x1d);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      puVar19 = &DAT_04349b4c;
      iVar4 = _GetNameGraphicOffset_CRegion__QBE_BVC2DVector__XZ();
      _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*(float *)(iVar4 + 4));
      local_8 = 0x20;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x21;
      uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
      local_8._0_1_ = 0x22;
      __YCCharString__QAEABV0_ABV0__Z(uVar18);
      local_8._0_1_ = 0x21;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0x20);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      local_2c = _GetMiniMapRegionExitTextOffsets_CRegion__QBEABV__map_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__std__XZ
                           ();
      _begin____Tree_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__QBE_AV___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___2_XZ
                (local_3c);
      local_8 = 0x23;
      while( true ) {
        uVar18 = _end____Tree_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__QBE_AV___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___2_XZ
                           (local_7e4);
        local_8._0_1_ = 0x24;
        local_7d5 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QBE_NABV01__Z
                              (uVar18);
        local_8._0_1_ = 0x23;
        __1___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAE_XZ
                  ();
        if (local_7d5 == '\0') break;
        puVar19 = &DAT_04349b64;
        iVar4 = __C___Tree_unchecked_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__U_Iterator_base12_2__std__QBEPBU__pair___CBVCCharString__VC2DVector___1_XZ
                          (&DAT_04349b64);
        uVar18 = _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*(float *)(iVar4 + 8));
        local_8._0_1_ = 0x25;
        uVar18 = __C___Tree_unchecked_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__U_Iterator_base12_2__std__QBEPBU__pair___CBVCCharString__VC2DVector___1_XZ
                           (&DAT_04349b68,uVar18);
        __H_YI_AVCCharString__PBDABV0__Z(uVar18);
        local_8._0_1_ = 0x26;
        __H_YI_AVCCharString__ABV0_PBD_Z();
        local_8._0_1_ = 0x27;
        __H_YI_AVCCharString__ABV0_0_Z();
        local_8._0_1_ = 0x28;
        uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
        local_8._0_1_ = 0x29;
        __YCCharString__QAEABV0_ABV0__Z(uVar18);
        local_8._0_1_ = 0x28;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x27;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x26;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x25;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x23;
        __1CCharString__QAE_XZ();
        puVar19 = &DAT_04349b8c;
        iVar4 = __C___Tree_unchecked_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__U_Iterator_base12_2__std__QBEPBU__pair___CBVCCharString__VC2DVector___1_XZ
                          (&DAT_04349b8c);
        _GFFloatToCharString__YI_AVCCharString__NJ_Z((double)*(float *)(iVar4 + 0xc));
        local_8._0_1_ = 0x2a;
        uVar18 = __C___Tree_unchecked_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__U_Iterator_base12_2__std__QBEPBU__pair___CBVCCharString__VC2DVector___1_XZ
                           (&DAT_04349b90);
        __H_YI_AVCCharString__PBDABV0__Z(uVar18);
        local_8._0_1_ = 0x2b;
        __H_YI_AVCCharString__ABV0_PBD_Z();
        local_8._0_1_ = 0x2c;
        __H_YI_AVCCharString__ABV0_0_Z();
        local_8._0_1_ = 0x2d;
        uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
        local_8._0_1_ = 0x2e;
        __YCCharString__QAEABV0_ABV0__Z(uVar18);
        local_8._0_1_ = 0x2d;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x2c;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x2b;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x2a;
        __1CCharString__QAE_XZ();
        local_8 = CONCAT31(local_8._1_3_,0x23);
        __1CCharString__QAE_XZ();
        __E___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAEAAV01_XZ
                  ();
      }
      local_8 = 0xffffffff;
      __1___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAE_XZ
                ();
      local_30 = _GetMaps_CRegion__QBEABV__vector_JV__allocator_J_std___std__XZ();
      _begin___vector_JV__allocator_J_std___std__QBE_AV___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                (local_48);
      local_8 = 0x2f;
      while( true ) {
        uVar18 = _end___vector_JV__allocator_J_std___std__QBE_AV___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                           (local_844);
        local_8._0_1_ = 0x30;
        local_835 = FID_conflict_operator__(uVar18);
        local_8._0_1_ = 0x2f;
        __1___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
        if (local_835 == '\0') break;
        puVar9 = (undefined4 *)
                 __D___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___std__QBEABJXZ
                           ();
        local_264 = *puVar9;
        local_260 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_264);
        pcVar20 = local_258;
        pcVar17 = local_150;
        pcVar15 = local_370;
        pcVar14 = (char *)0x0;
        pcVar10 = (char *)__BCCharString__QBEPBDXZ();
        _splitpath(pcVar10,pcVar14,pcVar15,pcVar17,pcVar20);
        puVar13 = local_854;
        _GetLevelDir_NResourceDirectoryNames__YI_AVCWideString__XZ(puVar13);
        local_8._0_1_ = 0x31;
        _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar13);
        local_8._0_1_ = 0x32;
        __BCCharString__QBEPBDXZ();
        local_25c = _strstr__YAPADPADPBD_Z();
        local_8._0_1_ = 0x31;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x2f;
        __1CWideString__QAE_XZ();
        __0CCharString__QAE_XZ();
        local_8._0_1_ = 0x33;
        uVar2 = (undefined1)local_8;
        local_8._0_1_ = 0x33;
        if (local_25c != 0) {
          _GetLevelDir_NResourceDirectoryNames__YI_AVCWideString__XZ();
          local_8._0_1_ = 0x34;
          iVar4 = _GetLength_CWideString__QBEJXZ();
          __4CCharString__QAEABV0_PBD_Z(iVar4 + local_25c);
          local_8._0_1_ = 0x33;
          __1CWideString__QAE_XZ();
          uVar2 = (undefined1)local_8;
        }
        local_8._0_1_ = uVar2;
        pcVar20 = local_258;
        __H_YI_AVCCharString__ABV0_PBD_Z();
        local_8._0_1_ = 0x35;
        __H_YI_AVCCharString__ABV0_PBD_Z(pcVar20);
        local_8._0_1_ = 0x37;
        __1CCharString__QAE_XZ();
        puVar19 = &DAT_04349bb4;
        __H_YI_AVCCharString__PBDABV0__Z();
        local_8._0_1_ = 0x38;
        uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
        local_8._0_1_ = 0x39;
        __YCCharString__QAEABV0_ABV0__Z(uVar18);
        local_8._0_1_ = 0x38;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x37;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x33;
        __1CCharString__QAE_XZ();
        local_8 = CONCAT31(local_8._1_3_,0x2f);
        __1CCharString__QAE_XZ();
        __E___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAEAAV01_XZ();
      }
      local_8 = 0xffffffff;
      __1___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
      __0___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
      local_8 = 0x3a;
      puVar13 = local_880;
      _GetVisMaps_CRegion__QAEAAV__vector_JV__allocator_J_std___std__XZ(puVar13);
      uVar18 = _begin___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                         (puVar13);
      local_8._0_1_ = 0x3b;
      __4___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAEAAV01_ABV01__Z
                (uVar18);
      local_8 = CONCAT31(local_8._1_3_,0x3a);
      __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
      while( true ) {
        puVar13 = local_89c;
        _GetVisMaps_CRegion__QAEAAV__vector_JV__allocator_J_std___std__XZ(puVar13);
        uVar18 = _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                           (puVar13);
        local_8._0_1_ = 0x3c;
        local_88d = FID_conflict_operator__(uVar18);
        local_8._0_1_ = 0x3a;
        __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
        if (local_88d == '\0') break;
        puVar9 = (undefined4 *)
                 __D___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QBEAAJXZ();
        local_58c = *puVar9;
        local_588 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_58c);
        pcVar20 = local_580;
        pcVar17 = local_478;
        pcVar15 = local_698;
        pcVar14 = (char *)0x0;
        pcVar10 = (char *)__BCCharString__QBEPBDXZ();
        _splitpath(pcVar10,pcVar14,pcVar15,pcVar17,pcVar20);
        puVar13 = local_8ac;
        _GetLevelDir_NResourceDirectoryNames__YI_AVCWideString__XZ(puVar13);
        local_8._0_1_ = 0x3d;
        _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar13);
        local_8._0_1_ = 0x3e;
        __BCCharString__QBEPBDXZ();
        local_584 = _strstr__YAPADPADPBD_Z();
        local_8._0_1_ = 0x3d;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x3a;
        __1CWideString__QAE_XZ();
        __0CCharString__QAE_XZ();
        local_8._0_1_ = 0x3f;
        uVar2 = (undefined1)local_8;
        local_8._0_1_ = 0x3f;
        if (local_584 != 0) {
          _GetLevelDir_NResourceDirectoryNames__YI_AVCWideString__XZ();
          local_8._0_1_ = 0x40;
          iVar4 = _GetLength_CWideString__QBEJXZ();
          __4CCharString__QAEABV0_PBD_Z(iVar4 + local_584);
          local_8._0_1_ = 0x3f;
          __1CWideString__QAE_XZ();
          uVar2 = (undefined1)local_8;
        }
        local_8._0_1_ = uVar2;
        pcVar20 = local_580;
        __H_YI_AVCCharString__ABV0_PBD_Z();
        local_8._0_1_ = 0x41;
        __H_YI_AVCCharString__ABV0_PBD_Z(pcVar20);
        local_8._0_1_ = 0x43;
        __1CCharString__QAE_XZ();
        puVar19 = &DAT_04349bcc;
        __H_YI_AVCCharString__PBDABV0__Z();
        local_8._0_1_ = 0x44;
        uVar18 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar19);
        local_8._0_1_ = 0x45;
        __YCCharString__QAEABV0_ABV0__Z(uVar18);
        local_8._0_1_ = 0x44;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x43;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0x3f;
        __1CCharString__QAE_XZ();
        local_8 = CONCAT31(local_8._1_3_,0x3a);
        __1CCharString__QAE_XZ();
        __E___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_AV01_H_Z();
        __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
      }
      __YCCharString__QAEABV0_PBD_Z("EndRegion;\r\n\r\n");
      local_14 = local_14 + 1;
      local_8 = 0xffffffff;
      __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
    }
    local_18 = local_18 + 1;
  }
  ExceptionList = local_10;
  return;
}



//=== CEditWorldMap__LoadFromFile @ 0296d140 ===

/* WARNING: Removing unreachable block (ram,0x0296d191) */
/* [ported from ego_r via strfp] */

void _LoadFromFile_CEditWorldMap__UAEXAAVCAFile__J_Z(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  undefined4 local_8;
  
  _LoadFromFile_CWorldMap__UAEXAAVCAFile__J_Z(param_1,param_2);
  uVar1 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ();
  _resize___vector_VCEditMapInfo__V__allocator_VCEditMapInfo___std___std__QAEXI_Z(uVar1);
  uVar1 = _size___vector_VCRegion__V__allocator_VCRegion___std___std__QBEIXZ();
  _resize___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QAEXI_Z(uVar1);
  for (local_8 = 1; uVar2 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ(),
      local_8 < uVar2; local_8 = local_8 + 1) {
    iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_8);
    if (*(char *)(iVar3 + 0x2c) != '\0') {
      _SetMapAsEditable_CEditWorldMap__QAEXJ_N_Z(local_8,0);
    }
  }
  return;
}



//=== CEditWorldMap__CreateAndSaveNewLevel @ 0296d2d0 ===

/* [ported from ego_r via strfp] */

undefined4
_CreateAndSaveNewLevel_CEditWorldMap__QAEJVCWideString__ABVC2DCoordI__ABVCMapInit___Z
          (undefined4 param_1,undefined4 param_2,int *param_3,int *param_4)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  BOOL BVar6;
  int *in_ECX;
  undefined4 uStack_cc;
  undefined1 *puStack_c8;
  undefined1 *puStack_c4;
  undefined1 *puStack_c0;
  undefined1 *puStack_bc;
  int *piStack_b8;
  undefined4 local_84;
  undefined1 local_70 [8];
  undefined1 *local_68;
  undefined1 local_64 [8];
  undefined1 local_5c [8];
  undefined1 local_54 [11];
  char local_49;
  undefined1 *local_30;
  undefined1 *local_2c;
  char local_25;
  void *local_24;
  undefined4 local_20;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_03df27ab;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  local_14 = (undefined1 *)(**(code **)(*in_ECX + 0x78))();
  piStack_b8 = (int *)0x296d315;
  _ToCharString_CWideString__QBE_AVCCharString__XZ();
  local_8._0_1_ = 1;
  piStack_b8 = (int *)local_14;
  puStack_bc = (undefined1 *)0x296d335;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  piStack_b8 = (int *)0x296d33f;
  __4CCharString__QAEABV0_ABV0__Z();
  local_8._0_1_ = 0;
  __1CCharString__QAE_XZ();
  piStack_b8 = (int *)0x296d35a;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  *(undefined1 *)(iVar3 + 0x2c) = 1;
  piStack_b8 = (int *)(*param_3 + *param_4);
  puStack_bc = (undefined1 *)param_3[1];
  puStack_c0 = (undefined1 *)*param_3;
  puStack_c4 = local_14;
  puStack_c8 = (undefined1 *)0x296d392;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  puStack_c4 = (undefined1 *)0x296d399;
  _Initialise_C2DBoxI__QAEXJJJJ_Z();
  piStack_b8 = (int *)0x296d3a8;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  *(undefined1 *)(iVar3 + 0x2d) = 1;
  piStack_b8 = (int *)0x296d3b6;
  local_24 = operator_new(0x1d78);
  local_8._0_1_ = 2;
  if (local_24 == (void *)0x0) {
    local_84 = 0;
  }
  else {
    piStack_b8 = param_3;
    puStack_bc = (undefined1 *)0x296d3d9;
    puStack_bc = (undefined1 *)_GetWorldSeed_CWorld__QBEPAKXZ();
    puStack_c0 = (undefined1 *)in_ECX[2];
    puStack_c4 = (undefined1 *)0x296d3ec;
    puStack_c4 = (undefined1 *)_PeekDefinitionManager_CWorld__QBEABVCGameDefinitionManager__XZ();
    puStack_c8 = (undefined1 *)0x296d3f5;
    local_84 = __0CEditMap__QAE_ABVCGameDefinitionManager__AAVCWorld__PAKABVC2DCoordI__ABVCMapInit___Z
                         ();
  }
  local_20 = local_84;
  local_8._0_1_ = 0;
  piStack_b8 = (int *)local_14;
  puStack_bc = (undefined1 *)0x296d41e;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  piStack_b8 = (int *)0x296d428;
  _reset___CCountedPointer_VCMap____QAEXPAVCMap___Z();
  piStack_b8 = (int *)0x296d434;
  uVar4 = _AllocateMapUID_CWorldMap__IAEKJ_Z();
  piStack_b8 = (int *)0x296d445;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  *(undefined4 *)(iVar3 + 0x28) = uVar4;
  piStack_b8 = (int *)0x296d457;
  __C__scoped_ptr_VCThingMapManager_NThingMap___boost__QBEPAVCThingMapManager_NThingMap__XZ();
  piStack_b8 = (int *)0x296d45e;
  _AddGameMap_CThingMapManager_NThingMap__QAEXJ_Z();
  piStack_b8 = (int *)local_14;
  puStack_bc = (undefined1 *)0x296d46f;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  piStack_b8 = (int *)(iVar3 + 0x20);
  puStack_bc = (undefined1 *)0x0;
  puStack_c0 = local_14;
  puStack_c4 = (undefined1 *)0x296d484;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  puStack_c0 = (undefined1 *)0x296d490;
  _SplitPath_CAFile__SIXABVCCharString__PAV2_111_Z();
  piStack_b8 = (int *)0x296d4a2;
  puVar5 = (undefined1 *)
           __A__vector_VCEditMapInfo__V__allocator_VCEditMapInfo___std___std__QAEAAVCEditMapInfo__I_Z
                     ();
  *puVar5 = 0;
  piStack_b8 = (int *)0x296d4b7;
  _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
            ();
  local_8._0_1_ = 3;
  local_2c = (undefined1 *)&puStack_c0;
  puStack_c4 = (undefined1 *)&puStack_c0;
  puStack_c8 = (undefined1 *)0x296d4f4;
  _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
            ();
  local_8._0_1_ = 4;
  local_30 = (undefined1 *)&uStack_cc;
  _begin___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
            (&uStack_cc);
  local_8._0_1_ = 3;
  ___find_V___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__J_std__YI_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___0_V10_0ABJ_Z
            ();
  local_8._0_1_ = 5;
  piStack_b8 = (int *)0x296d55a;
  cVar2 = FID_conflict_operator__();
  local_25 = '\x01' - (cVar2 != '\0');
  local_8._0_1_ = 3;
  __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
  local_8._0_1_ = 0;
  __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
  if (local_25 != '\0') {
    piStack_b8 = (int *)0x296d5a2;
    __0CWideString__QAE_PB_W_Z();
    local_8._0_1_ = 6;
    piStack_b8 = (int *)&DAT_043498c1;
    puStack_bc = (undefined1 *)0x296d5b5;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 7;
    piStack_b8 = (int *)0x4349c18;
    puStack_bc = (undefined1 *)0x296d5c8;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 8;
    piStack_b8 = (int *)0x1ca;
    puStack_bc = local_54;
    puStack_c0 = local_5c;
    puStack_c4 = local_64;
    puStack_c8 = (undefined1 *)0x296d5e4;
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    puStack_c8 = (undefined1 *)0x296d5eb;
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    puStack_c8 = (undefined1 *)0x296d5f2;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      ();
    local_49 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 7;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 6;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0;
    __1CWideString__QAE_XZ();
    if (local_49 != '\0') {
      BVar6 = IsDebuggerPresent();
      if (BVar6 != 0) {
        pcVar1 = (code *)swi(3);
        uVar4 = (*pcVar1)();
        return uVar4;
      }
    }
  }
  piStack_b8 = (int *)0x296d64b;
  _push_back___CArray_J__QAEXABJ_Z();
  piStack_b8 = (int *)local_14;
  puStack_bc = (undefined1 *)0x296d65e;
  piStack_b8 = (int *)__A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z();
  puStack_bc = (undefined1 *)0x296d667;
  _SetMapPlacement_CWorldMap__IAEXABVC2DBoxI__J_Z();
  piStack_b8 = (int *)0x296d670;
  _Get___CTBaseSingleton_VCEngineManager____SIAAVCEngineManager__XZ();
  piStack_b8 = (int *)0x296d677;
  _AddMap_CEngineManager__QAEXJ_Z();
  piStack_b8 = (int *)0x1;
  local_68 = (undefined1 *)&puStack_c0;
  puStack_c4 = (undefined1 *)&param_1;
  puStack_c8 = (undefined1 *)0x296d68c;
  __0CWideString__QAE_ABV0__Z();
  puStack_c4 = local_14;
  puStack_c8 = local_70;
  uStack_cc = 0x296d6a2;
  _SaveSingleLevel_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JVCWideString___N1_Z();
  piStack_b8 = (int *)0x296d6ae;
  _UnloadLevel_CWorldMap__IAEXJ_Z();
  puVar5 = local_14;
  local_8 = 0xffffffff;
  __1CWideString__QAE_XZ();
  ExceptionList = local_10;
  return puVar5;
}



//=== CEditWorldMap__CreateNewRegion @ 0296d920 ===

/* WARNING: Removing unreachable block (ram,0x0296da23) */
/* [ported from ego_r via strfp] */

uint _CreateNewRegion_CEditWorldMap__QAEJXZ(void)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  BOOL BVar5;
  uint uVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_3c [8];
  undefined1 local_34 [8];
  undefined1 local_2c [11];
  char local_21;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df2813;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  iVar3 = _size___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QBEIXZ();
  iVar4 = _size___vector_VCRegion__V__allocator_VCRegion___std___std__QBEIXZ();
  if (iVar3 != iVar4) {
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_043498c2,0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z("EditRegions.size()==Regions.size()",0xffffffff);
    local_8._0_1_ = 2;
    uVar11 = 1;
    uVar10 = 0x1d8;
    puVar9 = local_2c;
    puVar8 = local_34;
    puVar7 = local_3c;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0x1d8,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar7,puVar8,puVar9,uVar10,uVar11);
    local_21 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_21 != '\0') && (BVar5 = IsDebuggerPresent(), BVar5 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar6 = (*pcVar1)();
      return uVar6;
    }
  }
  local_14 = 1;
  while (uVar6 = _size___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QBEIXZ(),
        local_14 < uVar6) {
    local_18 = __A__vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QAEAAVCEditRegion__I_Z
                         (local_14);
    cVar2 = _IsUsedByEditor_CEditRegion__QBE_NXZ();
    if (cVar2 == '\0') break;
    local_14 = local_14 + 1;
  }
  uVar6 = _size___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QBEIXZ();
  if (local_14 < uVar6) {
    local_1c = __A__vector_VCRegion__V__allocator_VCRegion___std___std__QAEAAVCRegion__I_Z(local_14)
    ;
    _Initialise_CRegion__QAEXXZ();
    local_20 = __A__vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QAEAAVCEditRegion__I_Z
                         (local_14);
    _Initialise_CEditRegion__QAEXXZ();
  }
  else {
    uVar10 = __0CRegion__QAE_XZ();
    local_8 = 3;
    _push_back___vector_VCRegion__V__allocator_VCRegion___std___std__QAEX__QAVCRegion___Z(uVar10);
    local_8 = 0xffffffff;
    __1CRegion__QAE_XZ();
    uVar10 = __0CEditRegion__QAE_XZ();
    _push_back___vector_VCEditRegion__V__allocator_VCEditRegion___std___std__QAEX__QAVCEditRegion___Z
              (uVar10);
  }
  ExceptionList = local_10;
  return local_14;
}



//=== CEditWorldMap__SaveSingleLevel @ 0296e150 ===

/* [ported from ego_r via strfp] */

undefined4 *
_SaveSingleLevel_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JVCWideString___N1_Z
          (undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,char param_5
          ,char param_6)

{
  bool bVar1;
  char cVar2;
  int in_ECX;
  undefined8 uVar3;
  undefined4 uStack_160;
  char *pcStack_15c;
  undefined1 *puStack_158;
  undefined4 local_10c;
  undefined4 local_108;
  undefined1 local_d0 [24];
  undefined1 local_b8 [8];
  undefined1 *local_b0;
  undefined1 *local_ac;
  void *local_a8;
  undefined4 local_a4;
  void *local_a0;
  undefined4 local_9c;
  CAsyncMonikerFile local_78 [32];
  CAsyncMonikerFile local_58 [32];
  int local_38;
  undefined1 local_34 [8];
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined4 local_1c;
  undefined4 local_18;
  undefined1 local_12;
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_03df29a2;
  local_10 = ExceptionList;
  local_8 = 0;
  puStack_158 = (undefined1 *)0x296e18a;
  ExceptionList = &local_10;
  local_38 = _GetMapInfo_CWorldMap__QAEAAVCMapInfo__J_Z();
  _GetThingFilenameFromLevelFilename_CWorldMap__SI_AVCWideString__ABV2__Z();
  local_8._0_1_ = 1;
  if (*(char *)(in_ECX + 0x17d) != '\0') {
    puStack_158 = (undefined1 *)0xffffffff;
    pcStack_15c = ".backup";
    uStack_160 = 0x296e1c7;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 2;
    puStack_158 = (undefined1 *)0x296e1e8;
    _ToWideString_CCharString__QBE_AVCWideString__XZ();
    local_8._0_1_ = 3;
    puStack_158 = (undefined1 *)0x296e20d;
    __YCWideString__QAEABV0_ABV0__Z();
    local_8._0_1_ = 2;
    __1CWideString__QAE_XZ();
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    puStack_158 = (undefined1 *)0xffffffff;
    pcStack_15c = ".backup";
    uStack_160 = 0x296e241;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 4;
    puStack_158 = (undefined1 *)0x296e262;
    _ToWideString_CCharString__QBE_AVCWideString__XZ();
    local_8._0_1_ = 5;
    puStack_158 = (undefined1 *)0x296e287;
    __YCWideString__QAEABV0_ABV0__Z();
    local_8._0_1_ = 4;
    __1CWideString__QAE_XZ();
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
  }
  puStack_158 = (undefined1 *)0x296e2ac;
  local_a0 = operator_new(0x30);
  local_8._0_1_ = 6;
  if (local_a0 == (void *)0x0) {
    local_108 = 0;
  }
  else {
    local_108 = __0CMemoryFile__QAE_XZ();
  }
  local_9c = local_108;
  local_8._0_1_ = 1;
  puStack_158 = (undefined1 *)0x296e2fe;
  __0__CCountedPointer_VCMemoryFile____QAE_PAVCMemoryFile___Z();
  local_8._0_1_ = 7;
  puStack_158 = (undefined1 *)0x296e309;
  local_a8 = operator_new(0x30);
  local_8._0_1_ = 8;
  if (local_a8 == (void *)0x0) {
    local_10c = 0;
  }
  else {
    local_10c = __0CMemoryFile__QAE_XZ();
  }
  local_a4 = local_10c;
  local_8._0_1_ = 7;
  puStack_158 = (undefined1 *)0x296e35b;
  __0__CCountedPointer_VCMemoryFile____QAE_PAVCMemoryFile___Z();
  local_8._0_1_ = 9;
  __C__CCountedPointer_VCMemoryFile____QBEPAVCMemoryFile__XZ();
  _Open_CMemoryFile__QAEXXZ();
  __C__CCountedPointer_VCMemoryFile____QBEPAVCMemoryFile__XZ();
  _Open_CMemoryFile__QAEXXZ();
  local_ac = (undefined1 *)&puStack_158;
  pcStack_15c = local_24;
  uStack_160 = 0x296e391;
  ____0VCMemoryFile_____CCountedPointer_VCAFile____QAE_ABV__CCountedPointer_VCMemoryFile_____Z();
  local_8._0_1_ = 10;
  local_b0 = (undefined1 *)&uStack_160;
  ____0VCMemoryFile_____CCountedPointer_VCAFile____QAE_ABV__CCountedPointer_VCMemoryFile_____Z
            (local_2c);
  local_8 = CONCAT31(local_8._1_3_,9);
  _SaveSingleLevelToFiles_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JV__CCountedPointer_VCAFile____0_Z
            (&local_1c,param_2);
  local_12 = 1;
  local_11 = '\x01';
  if ((param_6 != '\0') && (cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z(), cVar2 != '\0')) {
    _SetFileReadOnly_CAFile__SIXABVCWideString___N_Z();
  }
  cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z();
  if ((cVar2 == '\0') || (cVar2 = _IsFileReadOnly_CAFile__SI_NABVCWideString___Z(), cVar2 == '\0'))
  {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (!bVar1) {
    puStack_158 = (undefined1 *)0x4;
    pcStack_15c = (char *)&param_3;
    uStack_160 = 0x296e45e;
    __0CDiskFileWin32__QAE_ABVCWideString__W4FileAccessType__K_Z();
    local_8._0_1_ = 0xb;
    __D__CCountedPointer_VCMemoryFile____QBEAAVCMemoryFile__XZ();
    _CopyFileW_CAFile__SIXAAV1_0_Z();
    puStack_158 = local_b8;
    pcStack_15c = (char *)0x296e488;
    puStack_158 = (undefined1 *)_ToCharString_CWideString__QBE_AVCCharString__XZ();
    local_8._0_1_ = 0xc;
    pcStack_15c = (undefined1 *)0x296e4b5;
    __H_YI_AVCCharString__PBDABV0__Z();
    local_8._0_1_ = 0xd;
    puStack_158 = (undefined1 *)0x296e4dc;
    __H_YI_AVCCharString__ABV0_PBD_Z();
    local_8._0_1_ = 0xe;
    __BCCharString__QBEPBDXZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_8._0_1_ = 0xd;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0xc;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0xb;
    __1CCharString__QAE_XZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_8 = CONCAT31(local_8._1_3_,9);
    CAsyncMonikerFile::~CAsyncMonikerFile(local_58);
  }
  if (local_11 != '\0') {
    if ((param_6 != '\0') && (cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z(), cVar2 != '\0'))
    {
      _SetFileReadOnly_CAFile__SIXABVCWideString___N_Z();
    }
    cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z();
    if ((cVar2 == '\0') || (cVar2 = _IsFileReadOnly_CAFile__SI_NABVCWideString___Z(), cVar2 == '\0')
       ) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) {
      puStack_158 = (undefined1 *)0x4;
      pcStack_15c = local_34;
      uStack_160 = 0x296e5c5;
      __0CDiskFileWin32__QAE_ABVCWideString__W4FileAccessType__K_Z();
      local_8._0_1_ = 0xf;
      __D__CCountedPointer_VCMemoryFile____QBEAAVCMemoryFile__XZ();
      _CopyFileW_CAFile__SIXAAV1_0_Z();
      puStack_158 = local_d0;
      pcStack_15c = (char *)0x296e5ef;
      puStack_158 = (undefined1 *)_ToCharString_CWideString__QBE_AVCCharString__XZ();
      local_8._0_1_ = 0x10;
      pcStack_15c = (undefined1 *)0x296e61c;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x11;
      puStack_158 = (undefined1 *)0x296e643;
      __H_YI_AVCCharString__ABV0_PBD_Z();
      local_8._0_1_ = 0x12;
      __BCCharString__QBEPBDXZ();
      _GFOutputDebugString__YIXPBD_Z();
      local_8._0_1_ = 0x11;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 0x10;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 0xf;
      __1CCharString__QAE_XZ();
      _GFOutputDebugString__YIXPBD_Z();
      local_8 = CONCAT31(local_8._1_3_,9);
      CAsyncMonikerFile::~CAsyncMonikerFile(local_78);
    }
  }
  if (param_5 == '\0') {
    uVar3 = _GetFileModifiedTimeAsSomeWackyWin32BigNumber_CAFile__SI_KABVCWideString___Z();
    *(undefined8 *)(local_38 + 0x40) = uVar3;
    uVar3 = _GetFileModifiedTimeAsSomeWackyWin32BigNumber_CAFile__SI_KABVCWideString___Z();
    *(undefined8 *)(local_38 + 0x48) = uVar3;
  }
  *param_1 = local_1c;
  param_1[1] = local_18;
  local_8._0_1_ = 7;
  __1__CCountedPointer_VCMemoryFile____QAE_XZ();
  local_8._0_1_ = 1;
  __1__CCountedPointer_VCMemoryFile____QAE_XZ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1CWideString__QAE_XZ();
  local_8 = 0xffffffff;
  __1CWideString__QAE_XZ();
  ExceptionList = local_10;
  return param_1;
}



//=== CEditWorldMap__SaveSingleLevelToFiles @ 0296e8b0 ===

/* [ported from ego_r via strfp] */

int * _SaveSingleLevelToFiles_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JV__CCountedPointer_VCAFile____0_Z
                (int *param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int in_ECX;
  int iVar4;
  int iVar5;
  undefined1 *puVar6;
  CAtlWinModule *pCVar7;
  undefined1 local_58 [28];
  undefined1 local_3c [8];
  CAtlWinModule local_34 [20];
  undefined4 local_20;
  int local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_03df2a00;
  local_10 = ExceptionList;
  local_8 = 1;
  ExceptionList = &local_10;
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_18 = (**(code **)(*piVar1 + 0x1c))();
  __0CCharString__QAE_XZ();
  local_8._0_1_ = 2;
  __0__vector_VCCharString__V__allocator_VCCharString___std___std__QAE_XZ();
  local_8._0_1_ = 3;
  pCVar7 = local_34;
  puVar6 = local_3c;
  uVar2 = param_2;
  (**(code **)(**(int **)(in_ECX + 8) + 0xc))(puVar6,param_2,pCVar7);
  _SaveToScript_CThingManager__QAEXAAVCCharString__JAAV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
            (puVar6,uVar2,pCVar7);
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ();
  _SaveToFile_CCharString__QBEXAAVCAFile___Z(uVar2);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_1c = (**(code **)(*piVar1 + 0x1c))();
  iVar4 = local_1c - local_18;
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  iVar3 = (**(code **)(*piVar1 + 0x1c))();
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ();
  _SaveToFile___CReplaceSerialise_VCFileFormatLevel__VCOldFileFormatLevel3____QAEXAAVCAFile___Z
            (uVar2);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  puVar6 = local_58;
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(puVar6);
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_2);
  __C__CCountedPointer_VCMap____QBEPAVCMap__XZ(uVar2);
  _SaveToFile_CMap__QBEXAAVCAFile__AAVCFileFormatLevel___Z(uVar2,puVar6);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  puVar6 = local_58;
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(puVar6);
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_2);
  __C__CCountedPointer_VCMap____QBEPAVCMap__XZ(uVar2);
  _SaveBrushesToFile_CMap__QBEXAAVCAFile__AAVCFileFormatLevel___Z(uVar2,puVar6);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  pCVar7 = local_34;
  puVar6 = local_58;
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(puVar6,pCVar7);
  _SaveNavigationMapToFile_CWorldMap__QBEXJAAVCAFile__AAVCFileFormatLevel__ABV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
            (param_2,uVar2,puVar6,pCVar7);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_14 = (**(code **)(*piVar1 + 0x1c))();
  iVar5 = local_14 - iVar3;
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  (**(code **)(*piVar1 + 0x14))(iVar3);
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ();
  _SaveToFile___CReplaceSerialise_VCFileFormatLevel__VCOldFileFormatLevel3____QAEXAAVCAFile___Z
            (uVar2);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  (**(code **)(*piVar1 + 0x14))(local_14);
  *param_1 = iVar5;
  param_1[1] = iVar4;
  local_8._0_1_ = 2;
  ATL::CAtlWinModule::~CAtlWinModule(local_34);
  local_8._0_1_ = 1;
  __1CCharString__QAE_XZ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  local_8 = 0xffffffff;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  ExceptionList = local_10;
  return param_1;
}



//=== CEditWorldMap__EditLoadLevel @ 0296f070 ===

/* [ported from ego_r via strfp] */

void _EditLoadLevel_CEditWorldMap__AAEXJ_Z(undefined4 param_1)

{
  _LoadLevel_CWorldMap__IAEXJW4ELoadType_1__Z(param_1,0);
  return;
}



//=== CEditWorldMap__ReloadMap @ 0296f740 ===

/* [ported from ego_r via strfp] */

void _ReloadMap_CEditWorldMap__QAEXJ_Z(undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  int *in_ECX;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df2b88;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_1);
  if (*(char *)(iVar3 + 0x2c) == '\0') {
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_04349985,0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z("Maps[n].Used",0xffffffff);
    local_8._0_1_ = 2;
    uVar9 = 1;
    uVar8 = 0x2f4;
    puVar7 = local_1c;
    puVar6 = local_24;
    puVar5 = local_2c;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar5,puVar6,puVar7,0x2f4,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar5,puVar6,puVar7,uVar8,uVar9);
    local_11 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if (local_11 != '\0') {
      BVar4 = IsDebuggerPresent();
      if (BVar4 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  cVar2 = (**(code **)(*in_ECX + 0x28))(param_1);
  if (cVar2 != '\0') {
    _UnloadLevel_CWorldMap__IAEXJ_Z(param_1);
    _LoadLevel_CWorldMap__IAEXJW4ELoadType_1__Z(param_1,0);
    (**(code **)(*in_ECX + 0x1c))(param_1);
    iVar3 = __A__vector_VCEditMapInfo__V__allocator_VCEditMapInfo___std___std__QAEAAVCEditMapInfo__I_Z
                      (param_1);
    *(undefined1 *)(iVar3 + 1) = 0;
  }
  ExceptionList = local_10;
  return;
}



//=== CEditWorldMap__LoadTopologyFromFile @ 0296f8e0 ===

/* [ported from ego_r via strfp] */

void _LoadTopologyFromFile_CEditWorldMap__MAEXJV__CCountedPointer_VCAFile____AAVCASuspendableProcess___Z
               (undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  void *pvVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int in_ECX;
  undefined1 *puVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 local_3c;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_03df2bcb;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_1);
  if (*(char *)(iVar3 + 0x2c) == '\0') {
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_04349986,0xffffffff);
    local_8._0_1_ = 2;
    __0CCharString__QAE_PBDJ_Z("Maps[index].Used",0xffffffff);
    local_8._0_1_ = 3;
    uVar14 = 1;
    uVar13 = 0x304;
    puVar12 = local_1c;
    puVar11 = local_24;
    puVar10 = local_2c;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar10,puVar11,puVar12,0x304,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar10,puVar11,puVar12,uVar13,uVar14);
    local_11 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 2;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CWideString__QAE_XZ();
    if (local_11 != '\0') {
      BVar4 = IsDebuggerPresent();
      if (BVar4 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  pvVar5 = operator_new(0x1d78);
  local_8._0_1_ = 4;
  if (pvVar5 == (void *)0x0) {
    local_3c = 0;
  }
  else {
    uVar6 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ();
    uVar7 = _GetMapBox_CWorldMap__QBEABVC2DBoxI__J_Z(param_1);
    uVar14 = param_1;
    uVar8 = _GetWorldSeed_CWorld__QBEPAKXZ(param_1,uVar7,uVar6);
    uVar13 = *(undefined4 *)(in_ECX + 8);
    uVar9 = _PeekDefinitionManager_CWorld__QBEABVCGameDefinitionManager__XZ(uVar13,uVar8);
    local_3c = __0CEditMap__QAE_ABVCGameDefinitionManager__AAVCWorld__PAKJABVC2DBoxI__AAVCAFile___Z
                         (uVar9,uVar13,uVar8,uVar14,uVar7,uVar6);
  }
  local_8 = (uint)local_8._1_3_ << 8;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_1);
  _reset___CCountedPointer_VCMap____QAEXPAVCMap___Z(local_3c);
  _push_back___CArray_J__QAEXABJ_Z(&param_1);
  _ActivateTopology_CWorldMap__QAEXJ_Z(param_1);
  _UpdateAllEdges_CWorldMap__IAEXXZ();
  local_8 = 0xffffffff;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== CEditWorldMap__AddNavNodeToHistory @ 02972930 ===

/* WARNING: Removing unreachable block (ram,0x02972bd1) */
/* [ported from ego_r via strfp] */

void _AddNavNodeToHistory_CEditWorldMap__ABEXPBVCANavQuadTreeNode__AAV__vector_JV__allocator_J_std___std___Z
               (int param_1)

{
  int iVar1;
  code *pcVar2;
  char cVar3;
  BOOL BVar4;
  int *piVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 local_98 [8];
  undefined1 local_90 [8];
  undefined1 local_88 [11];
  char local_7d;
  undefined4 local_7c;
  undefined1 local_78 [8];
  undefined1 local_70 [8];
  undefined1 local_68 [11];
  char local_5d;
  undefined1 local_5c [8];
  undefined1 local_54 [8];
  undefined1 local_4c [11];
  char local_41;
  undefined1 local_40 [8];
  undefined1 local_38 [8];
  undefined1 local_30 [11];
  char local_25;
  int local_24;
  int local_20;
  int local_1c;
  char local_15;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df2d99;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  if ((param_1 == 0) ||
     (ExceptionList = &local_10, cVar3 = _empty___vector_JV__allocator_J_std___std__QBE_NXZ(),
     cVar3 != '\0')) {
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_0434998f,0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z("pnode && NOT history.empty()",0xffffffff);
    local_8._0_1_ = 2;
    uVar10 = 1;
    uVar9 = 0x5b0;
    puVar8 = local_30;
    puVar7 = local_38;
    puVar6 = local_40;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x5b0,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar6,puVar7,puVar8,uVar9,uVar10);
    local_25 = '\x01' - (cVar3 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_25 != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
  }
  local_1c = _GetNavNodeLevel_CEditWorldMap__ABEJPBVCANavQuadTreeNode___Z(param_1);
  local_14 = _size___vector_JV__allocator_J_std___std__QBEIXZ();
  local_14 = local_14 + -1;
  if (local_1c == local_14) {
    piVar5 = (int *)_back___vector_JV__allocator_J_std___std__QAEAAJXZ();
    if (*piVar5 != 0) {
      __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
      local_8 = 3;
      __0CCharString__QAE_PBDJ_Z(&DAT_043499ae,0xffffffff);
      local_8._0_1_ = 4;
      __0CCharString__QAE_PBDJ_Z("history.back()==NAV_CORNER_TL",0xffffffff);
      local_8._0_1_ = 5;
      uVar10 = 1;
      uVar9 = 0x5b7;
      puVar8 = local_4c;
      puVar7 = local_54;
      puVar6 = local_5c;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x5b7,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar6,puVar7,puVar8,uVar9,uVar10);
      local_41 = '\x01' - (cVar3 != '\x01');
      local_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,3);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_41 != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
        pcVar2 = (code *)swi(3);
        (*pcVar2)();
        return;
      }
    }
    _pop_back___vector_JV__allocator_J_std___std__QAEXXZ();
  }
  else {
    if (local_1c <= local_14) {
      __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
      local_8 = 6;
      __0CCharString__QAE_PBDJ_Z(&DAT_043499af,0xffffffff);
      local_8._0_1_ = 7;
      __0CCharString__QAE_PBDJ_Z("sampled_level > this_level",0xffffffff);
      local_8._0_1_ = 8;
      uVar10 = 1;
      uVar9 = 0x5bc;
      puVar8 = local_68;
      puVar7 = local_70;
      puVar6 = local_78;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x5bc,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar6,puVar7,puVar8,uVar9,uVar10);
      local_5d = '\x01' - (cVar3 != '\x01');
      local_8._0_1_ = 7;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,6);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_5d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
        pcVar2 = (code *)swi(3);
        (*pcVar2)();
        return;
      }
    }
    local_24 = (local_1c - local_14) + -1;
    for (local_20 = local_24; 0 < local_20; local_20 = local_20 + -1) {
      local_7c = 0;
      _push_back___vector_JV__allocator_J_std___std__QAEX__QAJ_Z(&local_7c);
    }
  }
  local_15 = '\0';
LAB_02972c03:
  do {
    cVar3 = _empty___vector_JV__allocator_J_std___std__QBE_NXZ();
    if ((cVar3 != '\0') || (local_15 != '\0')) {
      ExceptionList = local_10;
      return;
    }
    piVar5 = (int *)_back___vector_JV__allocator_J_std___std__QAEAAJXZ();
    iVar1 = *piVar5;
    if (-1 < iVar1) {
      if (iVar1 < 3) {
        local_15 = '\x01';
        piVar5 = (int *)_back___vector_JV__allocator_J_std___std__QAEAAJXZ();
        *piVar5 = *piVar5 + 1;
        goto LAB_02972c03;
      }
      if (iVar1 == 3) {
        _pop_back___vector_JV__allocator_J_std___std__QAEXXZ();
        goto LAB_02972c03;
      }
    }
    __0CWideString__QAE_PB_W_Z(L"edit_world_map.cpp");
    local_8 = 9;
    __0CCharString__QAE_PBDJ_Z("Invalid corner index",0xffffffff);
    local_8._0_1_ = 10;
    __0CCharString__QAE_PBDJ_Z(&DAT_043499b3,0xffffffff);
    local_8._0_1_ = 0xb;
    uVar10 = 1;
    uVar9 = 0x5d5;
    puVar8 = local_88;
    puVar7 = local_90;
    puVar6 = local_98;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x5d5,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar6,puVar7,puVar8,uVar9,uVar10);
    local_7d = '\x01' - (cVar3 != '\x01');
    local_8._0_1_ = 10;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,9);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_7d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
  } while( true );
}



//=== CEditWorldMap__PostLoadTopology @ 0297d860 ===

/* [ported from ego_r via strfp] */

void _PostLoadTopology_CEditWorldMap__MAEXJ_Z(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = _GetMapBox_CWorldMap__QBEABVC2DBoxI__J_Z(param_1);
  _Get___CTBaseSingleton_VCEngineManager____SIAAVCEngineManager__XZ(uVar1);
  _AddChangedArea_CEngineManager__QAEXABVC2DBoxI___Z(uVar1);
  uVar1 = _GetMapBox_CWorldMap__QBEABVC2DBoxI__J_Z(param_1);
  _InitPassabilityFromThemesIfNecessary_CWorldMap__IAEXABVC2DBoxI___Z(uVar1);
  _UpdateAllEdges_CWorldMap__IAEXXZ();
  _Get___CTBaseSingleton_VCEngineManager____SIAAVCEngineManager__XZ(param_1);
  _SetMapAsLoaded_CEngineManager__QAEXJ_Z(param_1);
  return;
}



//=== CEditWorldMap__GetLoadableSections @ 0297d8d0 ===

/* [ported from ego_r via strfp] */

undefined4
_GetLoadableSections_CEditWorldMap__UBE_AV__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__XZ
          (undefined4 param_1)

{
  undefined4 uVar1;
  undefined1 local_3c [24];
  undefined1 local_24 [20];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03df39a9;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__QAE_XZ();
  local_8 = 1;
  uVar1 = _GetLoadAllSectionName_CLevelLoader__SI_AVCCharString__XZ();
  local_8._0_1_ = 2;
  ___insert_VCCharString______Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AU__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_1___QAVCCharString___Z
            (local_3c,uVar1);
  __1__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_std__QAE_XZ
            ();
  local_8._0_1_ = 1;
  __1CCharString__QAE_XZ();
  __0__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__QAE___QAV01__Z
            (local_24);
  local_8 = (uint)local_8._1_3_ << 8;
  __1__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__QAE_XZ();
  ExceptionList = local_10;
  return param_1;
}



//=== CEditLevelMerger__ResolveNextFile @ 02b25920 ===

/* WARNING: Removing unreachable block (ram,0x02b25f7e) */
/* WARNING: Removing unreachable block (ram,0x02b25f4b) */
/* [ported from ego_r via strfp] */

undefined1 _ResolveNextFile_CEditLevelMerger__QAE_NXZ(void)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 uStack_1fc;
  undefined1 *puStack_1f8;
  undefined1 *puStack_1f4;
  undefined4 *puStack_1f0;
  CAtlWinModule *pCStack_1ec;
  undefined4 local_1e8;
  undefined4 local_1e4;
  undefined4 local_1e0;
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  undefined4 local_1b4;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_1a0;
  undefined4 local_19c;
  undefined4 local_198;
  undefined4 local_194;
  undefined4 local_190;
  undefined4 local_18c;
  int *local_188;
  int local_184;
  undefined1 local_17d;
  undefined1 local_17c [8];
  undefined1 *local_174;
  undefined1 local_170 [8];
  undefined1 local_168 [11];
  char local_15d;
  undefined1 local_15c [8];
  undefined1 local_154 [11];
  char local_149;
  undefined4 *local_148;
  undefined4 *local_144;
  undefined4 *local_140;
  undefined4 *local_13c;
  undefined4 *local_138;
  undefined4 *local_134;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 local_128;
  undefined4 local_124 [11];
  undefined1 local_f8 [8];
  undefined1 local_f0 [8];
  undefined1 local_e8 [8];
  undefined1 local_e0 [8];
  undefined1 local_d8 [11];
  char local_cd;
  undefined4 local_cc;
  SchedulerBase *local_c8;
  char local_c1;
  undefined4 *local_c0;
  char local_b9;
  undefined4 *local_b8;
  undefined4 *local_b4;
  undefined4 *local_b0;
  int local_ac;
  int local_a8;
  undefined1 local_a4 [11];
  char local_99;
  CAtlWinModule local_98 [20];
  CAtlWinModule local_84 [20];
  undefined1 local_70 [20];
  undefined4 local_5c;
  int local_58;
  int local_54;
  undefined1 local_50 [20];
  undefined1 local_3c [20];
  int *local_28;
  undefined1 local_24 [20];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e286cc;
  local_10 = ExceptionList;
  pCStack_1ec = (CAtlWinModule *)0x2b25949;
  ExceptionList = &local_10;
  _Get_CMainGameComponent__SIPAV1_XZ();
  pCStack_1ec = (CAtlWinModule *)0x2b25950;
  local_188 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  pCStack_1ec = (CAtlWinModule *)0x2b25969;
  local_28 = (int *)(**(code **)(*local_188 + 0x30))();
  pCStack_1ec = (CAtlWinModule *)0x2b25974;
  iVar2 = _GetNoMaps_CWorldMap__QBEJXZ();
  if (*(int *)(local_184 + 0xe4) == iVar2) {
    pCStack_1ec = (CAtlWinModule *)0x2b2598d;
    _Finito_CEditLevelMerger__IAEXXZ();
  }
  pCStack_1ec = (CAtlWinModule *)0x2b25995;
  iVar2 = _GetNoMaps_CWorldMap__QBEJXZ();
  if (*(int *)(local_184 + 0xe4) < iVar2) {
    pCStack_1ec = (CAtlWinModule *)0x2b259b5;
    _CreateFileNames_CEditLevelMerger__IAEXXZ();
    pCStack_1ec = (CAtlWinModule *)local_d8;
    puStack_1f0 = (undefined4 *)0x2b259cd;
    pCStack_1ec = (CAtlWinModule *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
    local_8 = 0;
    puStack_1f0 = (undefined4 *)0x2b259fe;
    local_190 = pCStack_1ec;
    local_18c = pCStack_1ec;
    __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
    puStack_1f0 = (undefined4 *)0x2b25a05;
    cVar1 = _IsFileOpenForEdit_CP4Api_NP4Api__QBE_NABVCWideString___Z();
    local_cd = '\x01' - (cVar1 != '\0');
    local_8 = 0xffffffff;
    pCStack_1ec = (CAtlWinModule *)0x2b25a27;
    __1CWideString__QAE_XZ();
    if (local_cd == '\0') {
      pCStack_1ec = (CAtlWinModule *)0x2b25a68;
      _InitResolve_CEditLevelMerger__IAEXXZ();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xa0);
      puStack_1f0 = (undefined4 *)(local_184 + 0x90);
      puStack_1f4 = (undefined1 *)0x2b25a8c;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xa8);
      puStack_1f0 = (undefined4 *)(local_184 + 0x98);
      puStack_1f4 = (undefined1 *)0x2b25ab1;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = (CAtlWinModule *)local_e0;
      puStack_1f0 = (undefined4 *)0x2b25ac9;
      pCStack_1ec = (CAtlWinModule *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
      local_8 = 1;
      puStack_1f0 = (undefined4 *)0x2b25afa;
      local_198 = pCStack_1ec;
      local_194 = pCStack_1ec;
      __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
      puStack_1f0 = (undefined4 *)0x2b25b01;
      _RevertFile_CP4Api_NP4Api__QAE_NABVCWideString___Z();
      local_8 = 0xffffffff;
      pCStack_1ec = (CAtlWinModule *)0x2b25b13;
      __1CWideString__QAE_XZ();
      pCStack_1ec = (CAtlWinModule *)local_e8;
      puStack_1f0 = (undefined4 *)0x2b25b2b;
      pCStack_1ec = (CAtlWinModule *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
      local_8 = 2;
      puStack_1f0 = (undefined4 *)0x2b25b5c;
      local_1a0 = pCStack_1ec;
      local_19c = pCStack_1ec;
      __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
      puStack_1f0 = (undefined4 *)0x2b25b63;
      _RevertFile_CP4Api_NP4Api__QAE_NABVCWideString___Z();
      local_8 = 0xffffffff;
      pCStack_1ec = (CAtlWinModule *)0x2b25b75;
      __1CWideString__QAE_XZ();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xc0);
      puStack_1f0 = (undefined4 *)(local_184 + 0x90);
      puStack_1f4 = (undefined1 *)0x2b25b99;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 200);
      puStack_1f0 = (undefined4 *)(local_184 + 0x98);
      puStack_1f4 = (undefined1 *)0x2b25bbd;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = (CAtlWinModule *)local_f0;
      puStack_1f0 = (undefined4 *)0x2b25bd5;
      pCStack_1ec = (CAtlWinModule *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
      local_8 = 3;
      puStack_1f0 = (undefined4 *)0x2b25c06;
      local_1a8 = pCStack_1ec;
      local_1a4 = pCStack_1ec;
      __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
      puStack_1f0 = (undefined4 *)0x2b25c0d;
      _SyncFile_CP4Api_NP4Api__QAE_NABVCWideString___Z();
      local_8 = 0xffffffff;
      pCStack_1ec = (CAtlWinModule *)0x2b25c1f;
      __1CWideString__QAE_XZ();
      pCStack_1ec = (CAtlWinModule *)local_f8;
      puStack_1f0 = (undefined4 *)0x2b25c37;
      pCStack_1ec = (CAtlWinModule *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
      local_8 = 4;
      puStack_1f0 = (undefined4 *)0x2b25c68;
      local_1b0 = pCStack_1ec;
      local_1ac = pCStack_1ec;
      __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
      puStack_1f0 = (undefined4 *)0x2b25c6f;
      _SyncFile_CP4Api_NP4Api__QAE_NABVCWideString___Z();
      local_8 = 0xffffffff;
      pCStack_1ec = (CAtlWinModule *)0x2b25c81;
      __1CWideString__QAE_XZ();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xb0);
      puStack_1f0 = (undefined4 *)(local_184 + 0x90);
      puStack_1f4 = (undefined1 *)0x2b25ca6;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xb8);
      puStack_1f0 = (undefined4 *)(local_184 + 0x98);
      puStack_1f4 = (undefined1 *)0x2b25cca;
      _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z();
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)0x2b25ce2;
      _LoadMapIntoMemory_CEditLevelMerger__IAEXJ_Z();
      pCStack_1ec = (CAtlWinModule *)0x2b25cea;
      __0__vector_VCCharString__V__allocator_VCCharString___std___std__QAE_XZ();
      local_8 = 5;
      pCStack_1ec = local_84;
      puStack_1f0 = (undefined4 *)0x2b25d06;
      _push_back___vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEXABV__vector_VCCharString__V__allocator_VCCharString___std___2__Z
                ();
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)(local_184 + 0xa8);
      puStack_1f4 = local_50;
      puStack_1f8 = (undefined1 *)0x2b25d2f;
      _ParseTngFile_CEditLevelMerger__IAE_AV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__AAVCCharString__K_Z
                ();
      local_8._0_1_ = 6;
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)(local_184 + 0xb8);
      puStack_1f4 = local_3c;
      puStack_1f8 = (undefined1 *)0x2b25d5c;
      _ParseTngFile_CEditLevelMerger__IAE_AV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__AAVCCharString__K_Z
                ();
      local_8._0_1_ = 7;
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)(local_184 + 200);
      puStack_1f4 = local_70;
      puStack_1f8 = (undefined1 *)0x2b25d88;
      _ParseTngFile_CEditLevelMerger__IAE_AV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__AAVCCharString__K_Z
                ();
      local_8._0_1_ = 8;
      pCStack_1ec = (CAtlWinModule *)0x2b25d94;
      __0__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
                ();
      local_8._0_1_ = 9;
      pCStack_1ec = (CAtlWinModule *)local_24;
      puStack_1f0 = (undefined4 *)0x2b25daa;
      _push_back___vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEXABV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__2__Z
                ();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0x98);
      puStack_1f0 = (undefined4 *)0x2b25dc6;
      __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 10;
      pCStack_1ec = (CAtlWinModule *)local_a4;
      puStack_1f0 = (undefined4 *)0x2b25de2;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                ();
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)local_3c;
      puStack_1f4 = local_50;
      puStack_1f8 = local_70;
      uStack_1fc = 0x2b25e06;
      uVar3 = _Resolve_CEditLevelMerger__IAE_NAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__00J_Z
                        ();
      pCStack_1ec = (CAtlWinModule *)(uVar3 & 0xff);
      puStack_1f0 = (undefined4 *)0x2b25e15;
      _push_back___vector__NV__allocator__N_std___std__QAEX_N_Z();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0x90);
      puStack_1f0 = (undefined4 *)0x2b25e31;
      pCStack_1ec = (CAtlWinModule *)__H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0xb;
      puStack_1f0 = (undefined4 *)0x2b25e59;
      local_1b8 = pCStack_1ec;
      local_1b4 = pCStack_1ec;
      __4CCharString__QAEABV0_ABV0__Z();
      local_8._0_1_ = 10;
      pCStack_1ec = (CAtlWinModule *)0x2b25e68;
      __1CCharString__QAE_XZ();
      pCStack_1ec = (CAtlWinModule *)local_a4;
      puStack_1f0 = (undefined4 *)0x2b25e80;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                ();
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xa0);
      puStack_1f0 = (undefined4 *)0x2b25e98;
      uVar4 = _LoadLevFile_CEditLevelMerger__IAEPAVCMap__AAVCCharString___Z();
      *(undefined4 *)(local_184 + 0x110) = uVar4;
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xb0);
      puStack_1f0 = (undefined4 *)0x2b25ebc;
      uVar4 = _LoadLevFile_CEditLevelMerger__IAEPAVCMap__AAVCCharString___Z();
      *(undefined4 *)(local_184 + 0x114) = uVar4;
      pCStack_1ec = (CAtlWinModule *)(local_184 + 0xc0);
      puStack_1f0 = (undefined4 *)0x2b25ee0;
      uVar4 = _LoadLevFile_CEditLevelMerger__IAEPAVCMap__AAVCCharString___Z();
      *(undefined4 *)(local_184 + 0x118) = uVar4;
      local_99 = '\0';
      pCStack_1ec = (CAtlWinModule *)0x2b25f04;
      local_58 = _GetHeightMapSizeX_CMap__QBEJXZ();
      pCStack_1ec = (CAtlWinModule *)0x2b25f18;
      local_54 = _GetHeightMapSizeY_CMap__QBEJXZ();
      pCStack_1ec = *(CAtlWinModule **)(local_184 + 0xe4);
      puStack_1f0 = (undefined4 *)0x2b25f35;
      local_5c = (**(code **)(*local_28 + 0x48))();
      puStack_1f0 = (undefined4 *)0x2b25f43;
      __0__vector_W4CameFrom_CEditLevelMerger__V__allocator_W4CameFrom_CEditLevelMerger___std___std__QAE_XZ
                ();
      local_8._0_1_ = 0xc;
      for (local_a8 = 0; local_a8 < local_54; local_a8 = local_a8 + 1) {
        for (local_ac = 0; local_ac < local_58; local_ac = local_ac + 1) {
          puStack_1f0 = (undefined4 *)local_a8;
          puStack_1f4 = (undefined1 *)local_ac;
          puStack_1f8 = (undefined1 *)0x2b25fc6;
          puStack_1f0 = (undefined4 *)__0C2DCoordI__QAE_JJ_Z();
          puStack_1f4 = (undefined1 *)0x2b25fd8;
          _GetHeightMap_CMap__QAEAAVCHeightMap__XZ();
          puStack_1f4 = (undefined1 *)0x2b25fdf;
          local_b8 = (undefined4 *)_GetCell_CHeightMap__QAEAAVCHeightMapCell__ABVC2DCoordI___Z();
          puStack_1f0 = (undefined4 *)local_a8;
          puStack_1f4 = (undefined1 *)local_ac;
          puStack_1f8 = (undefined1 *)0x2b25ffe;
          puStack_1f0 = (undefined4 *)__0C2DCoordI__QAE_JJ_Z();
          puStack_1f4 = (undefined1 *)0x2b26010;
          _GetHeightMap_CMap__QAEAAVCHeightMap__XZ();
          puStack_1f4 = (undefined1 *)0x2b26017;
          local_b4 = (undefined4 *)_GetCell_CHeightMap__QAEAAVCHeightMapCell__ABVC2DCoordI___Z();
          puStack_1f0 = (undefined4 *)local_a8;
          puStack_1f4 = (undefined1 *)local_ac;
          puStack_1f8 = (undefined1 *)0x2b26036;
          puStack_1f0 = (undefined4 *)__0C2DCoordI__QAE_JJ_Z();
          puStack_1f4 = (undefined1 *)0x2b26048;
          _GetHeightMap_CMap__QAEAAVCHeightMap__XZ();
          puStack_1f4 = (undefined1 *)0x2b2604f;
          local_b0 = (undefined4 *)_GetCell_CHeightMap__QAEAAVCHeightMapCell__ABVC2DCoordI___Z();
          puStack_1f0 = (undefined4 *)local_a8;
          puStack_1f4 = (undefined1 *)local_ac;
          puStack_1f8 = (undefined1 *)0x2b2606e;
          puStack_1f0 = (undefined4 *)__0C2DCoordI__QAE_JJ_Z();
          puStack_1f4 = (undefined1 *)0x2b26077;
          _GetHeightMap_CMap__QAEAAVCHeightMap__XZ();
          puStack_1f4 = (undefined1 *)0x2b2607e;
          local_c0 = (undefined4 *)_GetCell_CHeightMap__QAEAAVCHeightMapCell__ABVC2DCoordI___Z();
          puStack_1f0 = local_b0;
          puStack_1f4 = (undefined1 *)0x2b26096;
          local_b9 = __8CHeightMapCell__QBE_NABV0__Z();
          puStack_1f0 = local_b0;
          puStack_1f4 = (undefined1 *)0x2b260ae;
          local_c1 = __8CHeightMapCell__QBE_NABV0__Z();
          if ((local_b9 == '\0') || (local_c1 == '\0')) {
            local_99 = '\x01';
          }
          else {
            local_124[0] = 0;
            puStack_1f0 = local_124;
            puStack_1f4 = (undefined1 *)0x2b260e6;
            _push_back___vector_W4CameFrom_CEditLevelMerger__V__allocator_W4CameFrom_CEditLevelMerger___std___std__QAEX__QAW4CameFrom_CEditLevelMerger___Z
                      ();
            *local_c0 = *local_b0;
            local_c0[1] = local_b0[1];
            local_c0[2] = local_b0[2];
          }
          if ((local_b9 == '\0') && (local_c1 != '\0')) {
            local_128 = 1;
            puStack_1f0 = &local_128;
            puStack_1f4 = (undefined1 *)0x2b2613d;
            _push_back___vector_W4CameFrom_CEditLevelMerger__V__allocator_W4CameFrom_CEditLevelMerger___std___std__QAEX__QAW4CameFrom_CEditLevelMerger___Z
                      ();
            *local_c0 = *local_b8;
            local_c0[1] = local_b8[1];
            local_c0[2] = local_b8[2];
          }
          if ((local_b9 != '\0') && (local_c1 == '\0')) {
            local_12c = 2;
            puStack_1f0 = &local_12c;
            puStack_1f4 = (undefined1 *)0x2b2618b;
            _push_back___vector_W4CameFrom_CEditLevelMerger__V__allocator_W4CameFrom_CEditLevelMerger___std___std__QAEX__QAW4CameFrom_CEditLevelMerger___Z
                      ();
            *local_c0 = *local_b4;
            local_c0[1] = local_b4[1];
            local_c0[2] = local_b4[2];
          }
          if ((local_b9 == '\0') && (local_c1 == '\0')) {
            *local_c0 = *local_b8;
            local_c0[1] = local_b8[1];
            local_c0[2] = local_b8[2];
            local_130 = 3;
            puStack_1f0 = &local_130;
            puStack_1f4 = (undefined1 *)0x2b261f5;
            _push_back___vector_W4CameFrom_CEditLevelMerger__V__allocator_W4CameFrom_CEditLevelMerger___std___std__QAEX__QAW4CameFrom_CEditLevelMerger___Z
                      ();
          }
        }
      }
      local_138 = *(undefined4 **)(local_184 + 0x110);
      local_134 = local_138;
      if (local_138 == (undefined4 *)0x0) {
        local_1bc = 0;
      }
      else {
        puStack_1f0 = (undefined4 *)0x1;
        puStack_1f4 = (undefined1 *)0x2b2623a;
        local_1bc = (**(code **)*local_138)();
      }
      local_140 = *(undefined4 **)(local_184 + 0x114);
      local_13c = local_140;
      if (local_140 == (undefined4 *)0x0) {
        local_1c0 = 0;
      }
      else {
        puStack_1f0 = (undefined4 *)0x1;
        puStack_1f4 = (undefined1 *)0x2b26287;
        local_1c0 = (**(code **)*local_140)();
      }
      local_148 = *(undefined4 **)(local_184 + 0x118);
      local_144 = local_148;
      if (local_148 == (undefined4 *)0x0) {
        local_1c4 = 0;
      }
      else {
        puStack_1f0 = (undefined4 *)0x1;
        puStack_1f4 = (undefined1 *)0x2b262d4;
        local_1c4 = (**(code **)*local_148)();
      }
      if (local_99 != '\0') {
        puStack_1f0 = (undefined4 *)0x2b262fa;
        local_c8 = (SchedulerBase *)_Get_CMainGameComponent__SIPAV1_XZ();
        puStack_1f0 = (undefined4 *)0x2b2630b;
        Concurrency::details::SchedulerBase::GetIScheduler(local_c8);
        puStack_1f0 = (undefined4 *)0x2b26312;
        local_cc = _GetCurrentEditWorldMap_CEditWorld__QAEAAVCEditWorldMap__XZ();
        puStack_1f0 = (undefined4 *)local_154;
        puStack_1f4 = (undefined1 *)0x2b26330;
        puStack_1f0 = (undefined4 *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
        local_8._0_1_ = 0xd;
        puStack_1f4 = (undefined1 *)0x2b2635e;
        local_1cc = puStack_1f0;
        local_1c8 = puStack_1f0;
        __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
        puStack_1f4 = (undefined1 *)0x2b26365;
        cVar1 = _IsFileOpenForEdit_CP4Api_NP4Api__QBE_NABVCWideString___Z();
        local_149 = '\x01' - (cVar1 != '\0');
        local_8._0_1_ = 0xc;
        puStack_1f0 = (undefined4 *)0x2b26384;
        __1CWideString__QAE_XZ();
        if (local_149 != '\0') {
          puStack_1f0 = (undefined4 *)local_15c;
          puStack_1f4 = (undefined1 *)0x2b263a7;
          puStack_1f0 = (undefined4 *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
          local_8._0_1_ = 0xe;
          puStack_1f4 = (undefined1 *)0x2b263d5;
          local_1d4 = puStack_1f0;
          local_1d0 = puStack_1f0;
          __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
          puStack_1f4 = (undefined1 *)0x2b263dc;
          _OpenFileForEdit_CP4Api_NP4Api__QAE_NABVCWideString___Z();
          local_8._0_1_ = 0xc;
          puStack_1f0 = (undefined4 *)0x2b263eb;
          __1CWideString__QAE_XZ();
        }
        puStack_1f0 = (undefined4 *)local_168;
        puStack_1f4 = (undefined1 *)0x2b26403;
        puStack_1f0 = (undefined4 *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
        local_8._0_1_ = 0xf;
        puStack_1f4 = (undefined1 *)0x2b26431;
        local_1dc = puStack_1f0;
        local_1d8 = puStack_1f0;
        __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
        puStack_1f4 = (undefined1 *)0x2b26438;
        cVar1 = _IsFileOpenForEdit_CP4Api_NP4Api__QBE_NABVCWideString___Z();
        local_15d = '\x01' - (cVar1 != '\0');
        local_8._0_1_ = 0xc;
        puStack_1f0 = (undefined4 *)0x2b26457;
        __1CWideString__QAE_XZ();
        if (local_15d != '\0') {
          puStack_1f0 = (undefined4 *)local_170;
          puStack_1f4 = (undefined1 *)0x2b2647a;
          puStack_1f0 = (undefined4 *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
          local_8._0_1_ = 0x10;
          puStack_1f4 = (undefined1 *)0x2b264a8;
          local_1e4 = puStack_1f0;
          local_1e0 = puStack_1f0;
          __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ();
          puStack_1f4 = (undefined1 *)0x2b264af;
          _OpenFileForEdit_CP4Api_NP4Api__QAE_NABVCWideString___Z();
          local_8._0_1_ = 0xc;
          puStack_1f0 = (undefined4 *)0x2b264be;
          __1CWideString__QAE_XZ();
        }
        puStack_1f0 = (undefined4 *)0x0;
        puStack_1f4 = (undefined1 *)0x1;
        local_174 = (undefined1 *)&uStack_1fc;
        local_1e8 = _ToWideString_CCharString__QBE_AVCWideString__XZ(&uStack_1fc);
        _SaveSingleLevel_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JVCWideString___N1_Z
                  (local_17c,*(undefined4 *)(local_184 + 0xe4));
      }
      *(int *)(local_184 + 0xe4) = *(int *)(local_184 + 0xe4) + 1;
      local_17d = 1;
      local_8._0_1_ = 10;
      puStack_1f0 = (undefined4 *)0x2b26535;
      ATL::CAtlWinModule::~CAtlWinModule(local_98);
      local_8._0_1_ = 9;
      puStack_1f0 = (undefined4 *)0x2b26544;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 8;
      puStack_1f0 = (undefined4 *)0x2b26550;
      __1__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
                ();
      local_8._0_1_ = 7;
      puStack_1f0 = (undefined4 *)0x2b2655c;
      __1__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
                ();
      local_8._0_1_ = 6;
      puStack_1f0 = (undefined4 *)0x2b26568;
      __1__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
                ();
      local_8 = CONCAT31(local_8._1_3_,5);
      puStack_1f0 = (undefined4 *)0x2b26574;
      __1__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
                ();
      local_8 = 0xffffffff;
      puStack_1f0 = (undefined4 *)0x2b26583;
      ATL::CAtlWinModule::~CAtlWinModule(local_84);
    }
    else {
      *(int *)(local_184 + 0xe4) = *(int *)(local_184 + 0xe4) + 1;
      pCStack_1ec = (CAtlWinModule *)0x2b25a58;
      local_17d = _ResolveNextFile_CEditLevelMerger__QAE_NXZ();
    }
  }
  else {
    local_17d = 0;
  }
  ExceptionList = local_10;
  return local_17d;
}



//=== CEditLevelMerger__LoadLevFile @ 02b26910 ===

/* [ported from ego_r via strfp] */

undefined4 _LoadLevFile_CEditLevelMerger__IAEPAVCMap__AAVCCharString___Z(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int in_ECX;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 local_70;
  undefined1 local_60 [16];
  void *local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  CAsyncMonikerFile local_38 [32];
  undefined4 local_18;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e2872b;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_14 = _Get_CMainGameComponent__SIPAV1_XZ();
  piVar1 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  uVar2 = *(undefined4 *)(in_ECX + 0xe4);
  (**(code **)(*piVar1 + 0x30))(uVar2);
  local_18 = _GetMapWithIndex_CWorldMap__QBEABVCMapInfo__J_Z(uVar2);
  __0CMapInit__QAE_XZ();
  local_48 = _GetWidth_C2DBoxI__QBEJXZ();
  local_44 = _GetHeight_C2DBoxI__QBEJXZ();
  local_40 = 0x38b9d;
  local_50 = operator_new(0x1d60);
  local_8 = 0;
  if (local_50 == (void *)0x0) {
    local_70 = 0;
  }
  else {
    puVar6 = &local_48;
    uVar2 = __0C2DCoordI__QAE_JJ_Z(0,0);
    puVar5 = &local_40;
    uVar3 = _GetWorld_CMainGameComponent__QAEAAVCWorld__XZ(puVar5,uVar2,puVar6);
    uVar4 = _PeekDefinitionManager_CMainGameComponent__QBEABVCGameDefinitionManager__XZ(uVar3);
    local_70 = __0CMap__QAE_ABVCGameDefinitionManager__AAVCWorld__PAKABVC2DCoordI__ABVCMapInit___Z
                         (uVar4,uVar3,puVar5,uVar2,puVar6);
  }
  local_4c = local_70;
  local_8 = 0xffffffff;
  local_3c = local_70;
  __0CDiskFileWin32__QAE_XZ();
  local_8 = 1;
  uVar4 = 2;
  uVar3 = 2;
  uVar2 = _ToWideString_CCharString__QBE_AVCWideString__XZ(local_60);
  local_8._0_1_ = 2;
  _Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z(uVar2,uVar3,uVar4);
  local_8 = CONCAT31(local_8._1_3_,1);
  __1CWideString__QAE_XZ();
  _LoadFromFile_CMap__QAEXAAVCAFile__PAVCASuspendableProcess___Z(local_38,0);
  _Close_CDiskFileWin32__UAEXXZ();
  uVar2 = local_3c;
  local_8 = 0xffffffff;
  CAsyncMonikerFile::~CAsyncMonikerFile(local_38);
  ExceptionList = local_10;
  return uVar2;
}



//=== CEditLevelMerger__CopyFileOnFile @ 02b26ad0 ===

/* [ported from ego_r via strfp] */

void _CopyFileOnFile_CEditLevelMerger__IAEXAAVCCharString__0_Z(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 local_60 [8];
  undefined1 local_58 [8];
  CAsyncMonikerFile local_50 [32];
  CAsyncMonikerFile local_30 [32];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28760;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CDiskFileWin32__QAE_XZ();
  local_8 = 0;
  __0CDiskFileWin32__QAE_XZ();
  local_8._0_1_ = 1;
  uVar3 = 0;
  uVar2 = 1;
  uVar1 = _ToWideString_CCharString__QBE_AVCWideString__XZ(local_58);
  local_8._0_1_ = 2;
  _Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z(uVar1,uVar2,uVar3);
  local_8._0_1_ = 1;
  __1CWideString__QAE_XZ();
  uVar3 = 0;
  uVar2 = 4;
  uVar1 = _ToWideString_CCharString__QBE_AVCWideString__XZ(local_60);
  local_8._0_1_ = 3;
  _Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z(uVar1,uVar2,uVar3);
  local_8._0_1_ = 1;
  __1CWideString__QAE_XZ();
  _CopyFileW_CAFile__SIXAAV1_0_Z();
  _Close_CDiskFileWin32__UAEXXZ();
  _Close_CDiskFileWin32__UAEXXZ();
  local_8 = (uint)local_8._1_3_ << 8;
  CAsyncMonikerFile::~CAsyncMonikerFile(local_30);
  local_8 = 0xffffffff;
  CAsyncMonikerFile::~CAsyncMonikerFile(local_50);
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__CreateFileNames @ 02b26c00 ===

/* [ported from ego_r via strfp] */

void _CreateFileNames_CEditLevelMerger__IAEXXZ(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int in_ECX;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined1 local_f8 [16];
  undefined1 local_e8 [8];
  undefined1 local_e0 [24];
  undefined1 local_c8 [16];
  undefined1 local_b8 [8];
  undefined1 local_b0 [24];
  undefined1 local_98 [16];
  undefined1 local_88 [8];
  undefined1 local_80 [24];
  undefined1 local_68 [16];
  undefined1 local_58 [8];
  undefined1 local_50 [8];
  undefined1 local_48 [24];
  undefined1 local_30 [8];
  undefined1 local_28 [8];
  undefined4 local_20;
  undefined1 local_1c [8];
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e2888d;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Get_CMainGameComponent__SIPAV1_XZ();
  piVar1 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  local_14 = (**(code **)(*piVar1 + 0x30))();
  local_20 = _GetMapWithIndex_CWorldMap__QBEABVCMapInfo__J_Z(*(undefined4 *)(in_ECX + 0xe4));
  __0CCharString__QAE_XZ();
  local_8 = 0;
  __0CCharString__QAE_XZ();
  local_8._0_1_ = 1;
  _SplitPath_CAFile__SIXABVCCharString__PAV2_111_Z(local_30,local_28,0);
  uVar2 = _ConvertPathToFullPath_CAFile__SI_AVCCharString__ABV2__Z();
  local_8._0_1_ = 2;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 1;
  __1CCharString__QAE_XZ();
  __0CWideString__QAE_XZ();
  local_8._0_1_ = 3;
  _ToWideString_CCharString__QBE_AVCWideString__XZ(local_48);
  local_8._0_1_ = 4;
  _ConvertPathToFullPath_CAFile__SI_NABVCWideString__AAV2__Z();
  local_8._0_1_ = 3;
  __1CWideString__QAE_XZ();
  uVar2 = _ToCharString_CWideString__QBE_AVCCharString__XZ(local_50);
  local_8._0_1_ = 5;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 3;
  __1CCharString__QAE_XZ();
  __0CWideString__QAE_PB_W_Z(&DAT_043c373c);
  local_8._0_1_ = 6;
  puVar6 = local_68;
  _SetFilenameExtension_CAFile__SI_AVCWideString__ABV2_0_Z(local_58);
  local_8._0_1_ = 7;
  uVar2 = _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar6);
  local_8._0_1_ = 8;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 7;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 6;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 3;
  __1CWideString__QAE_XZ();
  __0CCharString__QAE_XZ();
  local_8._0_1_ = 9;
  __4CCharString__QAEABV0_PBD_Z("Ours.lev");
  puVar6 = local_1c;
  __H_YI_AVCCharString__ABV0_0_Z(local_28);
  local_8._0_1_ = 10;
  uVar2 = __H_YI_AVCCharString__ABV0_0_Z(puVar6);
  local_8._0_1_ = 0xb;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 10;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CCharString__QAE_XZ();
  __0CWideString__QAE_PB_W_Z(&DAT_043c3750);
  local_8._0_1_ = 0xc;
  puVar7 = local_98;
  puVar6 = local_80;
  _ToWideString_CCharString__QBE_AVCWideString__XZ(local_88);
  local_8._0_1_ = 0xd;
  _SetFilenameExtension_CAFile__SI_AVCWideString__ABV2_0_Z(puVar6);
  local_8._0_1_ = 0xe;
  uVar2 = _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar7);
  local_8._0_1_ = 0xf;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 0xe;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0xd;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 0xc;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CWideString__QAE_XZ();
  __4CCharString__QAEABV0_PBD_Z("Theirs.lev");
  puVar6 = local_1c;
  __H_YI_AVCCharString__ABV0_0_Z(local_28);
  local_8._0_1_ = 0x10;
  uVar2 = __H_YI_AVCCharString__ABV0_0_Z(puVar6);
  local_8._0_1_ = 0x11;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 0x10;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CCharString__QAE_XZ();
  __0CWideString__QAE_PB_W_Z(&DAT_043c3764);
  local_8._0_1_ = 0x12;
  puVar6 = local_c8;
  puVar7 = local_b0;
  _ToWideString_CCharString__QBE_AVCWideString__XZ(local_b8);
  local_8._0_1_ = 0x13;
  _SetFilenameExtension_CAFile__SI_AVCWideString__ABV2_0_Z(puVar7);
  local_8._0_1_ = 0x14;
  uVar2 = _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar6);
  local_8._0_1_ = 0x15;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 0x14;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0x13;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 0x12;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CWideString__QAE_XZ();
  __4CCharString__QAEABV0_PBD_Z("Prev.lev");
  puVar6 = local_1c;
  __H_YI_AVCCharString__ABV0_0_Z(local_28);
  local_8._0_1_ = 0x16;
  uVar2 = __H_YI_AVCCharString__ABV0_0_Z(puVar6);
  local_8._0_1_ = 0x17;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8._0_1_ = 0x16;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CCharString__QAE_XZ();
  __0CWideString__QAE_PB_W_Z(&DAT_043c3778);
  local_8._0_1_ = 0x18;
  puVar6 = local_f8;
  puVar7 = local_e0;
  uVar3 = _ToWideString_CCharString__QBE_AVCWideString__XZ(local_e8);
  local_8._0_1_ = 0x19;
  uVar4 = _SetFilenameExtension_CAFile__SI_AVCWideString__ABV2_0_Z(puVar7);
  local_8._0_1_ = 0x1a;
  uVar8 = uVar4;
  uVar5 = _ToCharString_CWideString__QBE_AVCCharString__XZ(puVar6);
  local_8._0_1_ = 0x1b;
  uVar2 = uVar5;
  __4CCharString__QAEABV0_ABV0__Z(uVar5);
  local_8._0_1_ = 0x1a;
  __1CCharString__QAE_XZ(uVar5,uVar2,uVar4,uVar8,uVar3);
  local_8._0_1_ = 0x19;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 0x18;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 9;
  __1CWideString__QAE_XZ();
  local_8._0_1_ = 3;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 1;
  __1CWideString__QAE_XZ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1CCharString__QAE_XZ();
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__InitResolve @ 02b27470 ===

/* [ported from ego_r via strfp] */

void _InitResolve_CEditLevelMerger__IAEXXZ(void)

{
  _clear___list_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std__QAEXXZ
            ();
  _clear___vector_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std__QAEXXZ
            ();
  _clear___vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEXXZ
            ();
  return;
}



//=== CEditLevelMerger__ParseTngFile @ 02b274b0 ===

/* [ported from ego_r via strfp] */

undefined4
_ParseTngFile_CEditLevelMerger__IAE_AV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__AAVCCharString__K_Z
          (undefined4 param_1)

{
  char cVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined8 uVar4;
  undefined4 *puVar5;
  undefined4 uStack_1ec;
  undefined4 uStack_1e8;
  undefined1 *puStack_1e4;
  undefined1 *puStack_1e0;
  undefined8 *puStack_1dc;
  char *pcStack_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  undefined4 local_1b4;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined1 *local_1a4;
  undefined4 local_1a0;
  undefined4 local_19c;
  undefined1 *local_198;
  undefined1 *local_194;
  undefined1 *local_190;
  undefined1 *local_18c;
  int local_188;
  uint local_184;
  undefined1 local_180 [76];
  undefined1 local_134 [15];
  char local_125;
  undefined1 *local_124;
  undefined1 *local_120;
  undefined1 local_11c [16];
  undefined1 local_10c [11];
  undefined1 local_101;
  undefined1 local_100 [20];
  undefined1 local_ec [11];
  undefined1 local_e1;
  undefined1 local_d4 [8];
  CAsyncMonikerFile local_cc [32];
  undefined4 local_ac;
  SafeSQueue<Concurrency::details::SafePointInvocation,Concurrency::details::_HyperNonReentrantLock>
  local_a8 [16];
  undefined8 local_98;
  undefined1 local_8c [20];
  undefined4 local_78;
  undefined1 local_38 [8];
  int local_30;
  int local_2c;
  CAtlWinModule local_28 [23];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e289b1;
  local_10 = ExceptionList;
  local_184 = 0;
  pcStack_1d8 = (char *)0x2b274e9;
  ExceptionList = &local_10;
  __0CDiskFileWin32__QAE_XZ();
  local_8 = 1;
  pcStack_1d8 = (char *)0x2;
  puStack_1dc = (undefined8 *)0x1;
  puStack_1e0 = local_10c;
  puStack_1e4 = (undefined1 *)0x2b27503;
  puStack_1e0 = (undefined1 *)_ToWideString_CCharString__QBE_AVCWideString__XZ();
  local_8._0_1_ = 2;
  puStack_1e4 = (undefined1 *)0x2b2752b;
  local_190 = puStack_1e0;
  local_18c = puStack_1e0;
  _Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z();
  local_8._0_1_ = 1;
  pcStack_1d8 = (char *)0x2b2753a;
  __1CWideString__QAE_XZ();
  pcStack_1d8 = (char *)0x2b27545;
  iVar2 = _GetLength_CDiskFileWin32__UBEJXZ();
  pcStack_1d8 = (char *)(iVar2 + 1);
  puStack_1dc = (undefined8 *)0x2b27551;
  __0__vector_DV__allocator_D_std___std__QAE_I_Z();
  local_8._0_1_ = 3;
  pcStack_1d8 = (char *)0x0;
  puStack_1dc = (undefined8 *)0x2b27562;
  puStack_1dc = (undefined8 *)_GetLength_CDiskFileWin32__UBEJXZ();
  puStack_1e0 = (undefined1 *)0x0;
  puStack_1e4 = (undefined1 *)0x2b2756d;
  puStack_1e0 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
  puStack_1e4 = (undefined1 *)0x2b27579;
  _Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z();
  pcStack_1d8 = (char *)0x2b27584;
  pcStack_1d8 = (char *)_GetLength_CDiskFileWin32__UBEJXZ();
  puStack_1dc = (undefined8 *)0x2b2758d;
  puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
  *puVar3 = 0;
  pcStack_1d8 = (char *)0x2b2759b;
  _Close_CDiskFileWin32__UAEXXZ();
  pcStack_1d8 = (char *)0x2b275a6;
  __0__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
            ();
  local_8._0_1_ = 4;
  pcStack_1d8 = (char *)0x0;
  puStack_1dc = (undefined8 *)0xffffffff;
  puStack_1e0 = &DAT_043c372f;
  puStack_1e4 = (undefined1 *)0x2b275be;
  puStack_1dc = (undefined8 *)__0CCharString__QAE_PBDJ_Z();
  local_8._0_1_ = 5;
  puStack_1e0 = (undefined1 *)0x0;
  puStack_1e4 = (undefined1 *)0x0;
  uStack_1e8 = 0x2b275e7;
  local_198 = (undefined1 *)puStack_1dc;
  local_194 = (undefined1 *)puStack_1dc;
  puStack_1e4 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
  uStack_1e8 = 0x2b275f0;
  __0CStringParser__QAE_PBDP6IXABVCCharString___Z1J_Z();
  local_8._0_1_ = 7;
  pcStack_1d8 = (char *)0x2b275ff;
  __1CCharString__QAE_XZ();
  pcStack_1d8 = (char *)0x2b2760a;
  __0CParsedItem__QAE_XZ();
  local_8._0_1_ = 8;
  pcStack_1d8 = (char *)0x2b27616;
  __0CCharString__QAE_XZ();
  local_8._0_1_ = 9;
  local_11 = '\x01';
  local_ac = 0;
  local_30 = 0;
  local_78 = 0;
  local_2c = -1;
  while( true ) {
    pcStack_1d8 = (char *)0x2b27645;
    cVar1 = _NextItemExists_CStringParser__QAE_NXZ();
    if (cVar1 == '\0') break;
    pcStack_1d8 = (char *)0x2b27658;
    local_78 = _GetStringPos_CStringParser__QBEKXZ();
    pcStack_1d8 = local_d4;
    puStack_1dc = (undefined8 *)0x2b2766a;
    _ReadAsString_CStringParser__QAE_AVCCharString__XZ();
    local_8._0_1_ = 10;
    pcStack_1d8 = "XXXSectionStart";
    puStack_1dc = (undefined8 *)0x2b2767e;
    cVar1 = __8CCharString__QBE_NPBD_Z();
    if (cVar1 != '\0') {
      pcStack_1d8 = local_11c;
      puStack_1dc = (undefined8 *)0x2b27698;
      pcStack_1d8 = (char *)_ReadAsString_CStringParser__QAE_AVCCharString__XZ();
      local_8._0_1_ = 0xb;
      puStack_1dc = (undefined8 *)0x2b276bd;
      local_1a0 = pcStack_1d8;
      local_19c = pcStack_1d8;
      __4CCharString__QAEABV0_ABV0__Z();
      local_8._0_1_ = 10;
      pcStack_1d8 = (char *)0x2b276cc;
      __1CCharString__QAE_XZ();
      local_1a4 = local_38;
      local_120 = (undefined1 *)&puStack_1e0;
      puStack_1e4 = (undefined1 *)&puStack_1e0;
      uStack_1e8 = *(undefined4 *)(local_188 + 0x8c);
      uStack_1ec = 0x2b276ff;
      __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                ();
      uStack_1e8 = 0x2b27706;
      local_1ac = _end___vector_VCCharString__V__allocator_VCCharString___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___2_XZ
                            ();
      local_8._0_1_ = 0xc;
      local_124 = (undefined1 *)&uStack_1ec;
      puVar5 = &uStack_1ec;
      local_1a8 = local_1ac;
      __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                (*(undefined4 *)(local_188 + 0x8c));
      local_1b0 = _begin___vector_VCCharString__V__allocator_VCCharString___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___2_XZ
                            (puVar5);
      local_8._0_1_ = 10;
      local_1b4 = ___find_V___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___std__VCCharString___std__YI_AV___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___0_V10_0ABVCCharString___Z
                            ();
      local_8._0_1_ = 0xd;
      pcStack_1d8 = local_134;
      puStack_1dc = *(undefined8 **)(local_188 + 0x8c);
      puStack_1e0 = (undefined1 *)0x2b27797;
      __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                ();
      puStack_1dc = (undefined8 *)0x2b2779e;
      pcStack_1d8 = (char *)_end___vector_VCCharString__V__allocator_VCCharString___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___2_XZ
                                      ();
      local_8._0_1_ = 0xe;
      puStack_1dc = (undefined8 *)0x2b277c6;
      local_1bc = pcStack_1d8;
      local_1b8 = pcStack_1d8;
      local_125 = FID_conflict_operator__();
      local_8 = CONCAT31(local_8._1_3_,0xd);
      pcStack_1d8 = (char *)0x2b277db;
      __1___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___std__QAE_XZ
                ();
      if (local_125 != '\0') {
        pcStack_1d8 = local_38;
        puStack_1dc = *(undefined8 **)(local_188 + 0x8c);
        puStack_1e0 = (undefined1 *)0x2b27808;
        __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                  ();
        puStack_1dc = (undefined8 *)0x2b2780f;
        _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                  ();
      }
      local_8._0_1_ = 10;
      pcStack_1d8 = (char *)0x2b2781e;
      __1___Vector_iterator_V___Vector_val_VCCharString__V__allocator_VCCharString___std___std___std__QAE_XZ
                ();
    }
    pcStack_1d8 = "NewThing";
    puStack_1dc = (undefined8 *)0x2b2782e;
    cVar1 = __8CCharString__QBE_NPBD_Z();
    if (cVar1 != '\0') {
      if (local_11 != '\0') {
        pcStack_1d8 = (char *)local_78;
        puStack_1dc = (undefined8 *)0x2b27849;
        puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
        local_e1 = *puVar3;
        pcStack_1d8 = (char *)local_78;
        puStack_1dc = (undefined8 *)0x2b2785d;
        puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
        *puVar3 = 0;
        pcStack_1d8 = (char *)0x0;
        puStack_1dc = (undefined8 *)0x2b2786a;
        pcStack_1d8 = (char *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
        puStack_1dc = (undefined8 *)0x2b2787c;
        __4CCharString__QAEABV0_PBD_Z();
        pcStack_1d8 = (char *)local_78;
        puStack_1dc = (undefined8 *)0x2b27888;
        puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
        *puVar3 = local_e1;
      }
      local_ac = local_78;
      local_11 = '\0';
    }
    pcStack_1d8 = "UID";
    puStack_1dc = (undefined8 *)0x2b278ad;
    cVar1 = __8CCharString__QBE_NPBD_Z();
    uVar4 = local_98;
    if (cVar1 != '\0') {
      pcStack_1d8 = (char *)0x2b278bc;
      uVar4 = _ReadAsUHUGE_CStringParser__QAE_KXZ();
    }
    pcStack_1d8 = "EndThing";
    puStack_1dc = (undefined8 *)0x2b278d8;
    local_98 = uVar4;
    cVar1 = __8CCharString__QBE_NPBD_Z();
    if (cVar1 != '\0') {
      pcStack_1d8 = (char *)0x2b278ee;
      __0CThingText_CEditLevelMerger__QAE_XZ();
      local_8._0_1_ = 0xf;
      pcStack_1d8 = (char *)0x2b278fa;
      iVar2 = _GetStringPos_CStringParser__QBEKXZ();
      pcStack_1d8 = (char *)(iVar2 + 1);
      puStack_1dc = (undefined8 *)0x2b27912;
      local_30 = (int)pcStack_1d8;
      puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
      local_101 = *puVar3;
      pcStack_1d8 = (char *)local_30;
      puStack_1dc = (undefined8 *)0x2b27926;
      puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
      *puVar3 = 0;
      pcStack_1d8 = (char *)local_ac;
      puStack_1dc = (undefined8 *)0x2b27938;
      pcStack_1d8 = (char *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
      puStack_1dc = (undefined8 *)0x2b27944;
      __4CCharString__QAEABV0_PBD_Z();
      pcStack_1d8 = (char *)local_30;
      puStack_1dc = (undefined8 *)0x2b27950;
      puVar3 = (undefined1 *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
      *puVar3 = local_101;
      pcStack_1d8 = local_100;
      puStack_1dc = (undefined8 *)0x2b2796a;
      __4CCharString__QAEABV0_ABV0__Z();
      pcStack_1d8 = local_38;
      puStack_1dc = (undefined8 *)0x2b27979;
      __4CCharString__QAEABV0_ABV0__Z();
      pcStack_1d8 = local_100;
      puStack_1dc = (undefined8 *)local_ec;
      puStack_1e0 = (undefined1 *)0x2b27997;
      local_1c4 = __H_YI_AVCCharString__PBDABV0__Z();
      local_8._0_1_ = 0x10;
      puStack_1dc = (undefined8 *)0x2b279be;
      local_1c0 = local_1c4;
      pcStack_1d8 = (char *)__H_YI_AVCCharString__ABV0_0_Z();
      local_8._0_1_ = 0x11;
      puStack_1dc = (undefined8 *)0x2b279e6;
      local_1cc = pcStack_1d8;
      local_1c8 = pcStack_1d8;
      __4CCharString__QAEABV0_ABV0__Z();
      local_8._0_1_ = 0x10;
      pcStack_1d8 = (char *)0x2b279f5;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 0xf;
      pcStack_1d8 = (char *)0x2b27a04;
      __1CCharString__QAE_XZ();
      pcStack_1d8 = local_100;
      puStack_1dc = &local_98;
      puStack_1e0 = (undefined1 *)0x2b27a1d;
      pcStack_1d8 = (char *)____0AA_KAAVCThingText_CEditLevelMerger_____pair__KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
                                      ();
      local_8._0_1_ = 0x12;
      puStack_1dc = (undefined8 *)local_180;
      puStack_1e0 = (undefined1 *)0x2b27a4c;
      local_1d4 = pcStack_1d8;
      local_1d0 = pcStack_1d8;
      ___insert_U__pair__KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1___QAU__pair__KVCThingText_CEditLevelMerger___1__Z
                ();
      pcStack_1d8 = (char *)0x2b27a57;
      __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                ();
      local_8._0_1_ = 0xf;
      pcStack_1d8 = (char *)0x2b27a66;
      __1__pair__KVCThingText_CEditLevelMerger___std__QAE_XZ();
      local_2c = local_30;
      local_8._0_1_ = 10;
      pcStack_1d8 = (char *)0x2b27a7b;
      __1CThingText_CEditLevelMerger__QAE_XZ();
    }
    local_8._0_1_ = 9;
    pcStack_1d8 = (char *)0x2b27a8a;
    __1CCharString__QAE_XZ();
  }
  if (local_2c == -1) {
    pcStack_1d8 = (char *)0x0;
    puStack_1dc = (undefined8 *)0x2b27a9f;
    pcStack_1d8 = (char *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
    puStack_1dc = (undefined8 *)0x2b27ab1;
    __4CCharString__QAEABV0_PBD_Z();
  }
  else {
    pcStack_1d8 = (char *)local_2c;
    puStack_1dc = (undefined8 *)0x2b27abf;
    pcStack_1d8 = (char *)__A__vector_DV__allocator_D_std___std__QAEAADI_Z();
    puStack_1dc = (undefined8 *)0x2b27ad1;
    __4CCharString__QAEABV0_PBD_Z();
  }
  pcStack_1d8 = local_8c;
  puStack_1dc = (undefined8 *)0x2b27ae0;
  __0__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE___QAV01__Z
            ();
  local_184 = local_184 | 1;
  local_8._0_1_ = 8;
  pcStack_1d8 = (char *)0x2b27afb;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 7;
  pcStack_1d8 = (char *)0x2b27b0a;
  Concurrency::details::
  SafeSQueue<Concurrency::details::SafePointInvocation,Concurrency::details::_HyperNonReentrantLock>
  ::Release(local_a8);
  local_8._0_1_ = 4;
  pcStack_1d8 = (char *)0x2b27b16;
  __1CStringParser__UAE_XZ();
  local_8._0_1_ = 3;
  pcStack_1d8 = (char *)0x2b27b25;
  __1__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__QAE_XZ
            ();
  local_8._0_1_ = 1;
  pcStack_1d8 = (char *)0x2b27b31;
  ATL::CAtlWinModule::~CAtlWinModule(local_28);
  local_8 = (uint)local_8._1_3_ << 8;
  pcStack_1d8 = (char *)0x2b27b40;
  CAsyncMonikerFile::~CAsyncMonikerFile(local_cc);
  ExceptionList = local_10;
  return param_1;
}



//=== CEditLevelMerger__0CThingText @ 02b27d00 ===

/* [ported from ego_r via strfp] */

int __0CThingText_CEditLevelMerger__QAE_XZ(void)

{
  int in_ECX;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28a13;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CCharString__QAE_XZ();
  local_8 = 0;
  __0CCharString__QAE_XZ();
  local_8 = CONCAT31(local_8._1_3_,1);
  __0CCharString__QAE_XZ();
  *(undefined4 *)(in_ECX + 0x10) = 3;
  ExceptionList = local_10;
  return in_ECX;
}



//=== CEditLevelMerger__Resolve @ 02b27e80 ===

/* WARNING: Removing unreachable block (ram,0x02b2849d) */
/* WARNING: Removing unreachable block (ram,0x02b27ed3) */
/* WARNING: Removing unreachable block (ram,0x02b28a81) */
/* [ported from ego_r via strfp] */

undefined1
_Resolve_CEditLevelMerger__IAE_NAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__00J_Z
          (void)

{
  undefined1 uVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  int in_ECX;
  undefined8 uVar6;
  undefined4 in_stack_00000010;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_b3c [8];
  undefined1 local_b34 [16];
  undefined1 local_b24 [15];
  char local_b15;
  undefined1 local_b14 [12];
  undefined1 local_b08 [15];
  char local_af9;
  undefined1 local_af8 [12];
  undefined1 local_aec [76];
  undefined1 local_aa0 [8];
  undefined1 local_a98 [56];
  undefined1 local_a60 [16];
  undefined1 local_a50 [15];
  char local_a41;
  undefined1 local_a40 [12];
  undefined1 local_a34 [15];
  char local_a25;
  undefined1 local_a24 [12];
  undefined1 local_a18 [15];
  char local_a09;
  undefined1 local_a08 [12];
  undefined1 local_9fc [12];
  undefined1 local_9f0 [8];
  undefined1 local_9e8 [16];
  undefined1 local_9d8 [8];
  undefined1 local_9d0 [16];
  undefined1 local_9c0 [16];
  undefined1 local_9b0 [15];
  char local_9a1;
  undefined1 local_9a0 [12];
  undefined1 local_994 [15];
  char local_985;
  undefined1 local_984 [12];
  undefined1 local_978 [15];
  char local_969;
  undefined1 local_968 [12];
  undefined1 local_95c [12];
  char local_950 [1000];
  undefined8 local_568;
  undefined8 local_560;
  char local_558 [1000];
  undefined8 local_170;
  undefined1 local_168 [64];
  undefined4 local_128;
  undefined4 local_124;
  char local_120 [104];
  undefined1 local_b8 [8];
  char local_b0 [104];
  undefined1 local_48 [11];
  undefined1 local_3d;
  char local_2f;
  char local_2e;
  char local_2d;
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28b8f;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8 = 0;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8._0_1_ = 1;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8._0_1_ = 2;
  local_3d = 0;
  uVar3 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                    (local_95c);
  local_8._0_1_ = 3;
  __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
            (uVar3);
  local_8 = CONCAT31(local_8._1_3_,2);
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  while( true ) {
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_978);
    local_8._0_1_ = 4;
    local_969 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_969 == '\0') break;
    local_2f = '\0';
    local_2d = '\0';
    local_11 = '\0';
    local_2e = '\0';
    uVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                      ();
    uVar3 = _find____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_AB_K_Z
                      (local_984,uVar3);
    local_8._0_1_ = 5;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
              (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_994);
    local_8._0_1_ = 6;
    local_985 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8 = CONCAT31(local_8._1_3_,2);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_985 == '\0') {
      local_2e = '\x01';
    }
    else {
      iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      iVar4 = iVar4 + 8;
      __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                (iVar4);
      local_2d = __8CCharString__QBE_NABV0__Z(iVar4);
    }
    uVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                      ();
    uVar3 = _find____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_AB_K_Z
                      (local_9a0,uVar3);
    local_8._0_1_ = 7;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
              (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_9b0);
    local_8._0_1_ = 8;
    local_9a1 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8 = CONCAT31(local_8._1_3_,2);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_9a1 == '\0') {
      local_11 = '\x01';
    }
    else {
      iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      iVar4 = iVar4 + 8;
      __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                (iVar4);
      local_2f = __8CCharString__QBE_NABV0__Z(iVar4);
    }
    if ((local_2d == '\0') || (local_2f == '\0')) {
      puVar5 = (undefined4 *)
               __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                         ();
      sprintf(local_120,"Thing #%i conflict...",*puVar5,puVar5[1]);
      __0CCharString__QAE_PBDJ_Z(local_120,0xffffffff);
      local_8._0_1_ = 10;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (local_b8);
      local_3d = 1;
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CCharString__QAE_XZ();
    }
    else {
      iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      *(undefined4 *)(iVar4 + 0x18) = 0;
      uVar3 = __D___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEAAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      puVar9 = local_9c0;
      __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                (*(undefined4 *)(in_ECX + 0x8c));
      ___insert_AAU__pair___CB_KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair___CB_KVCThingText_CEditLevelMerger___1__Z
                (puVar9,uVar3);
      __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                ();
      puVar5 = (undefined4 *)
               __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                         ();
      sprintf(local_b0,"Thing #%i is unchanged...",*puVar5,puVar5[1]);
      __0CCharString__QAE_PBDJ_Z(local_b0,0xffffffff);
      local_8._0_1_ = 9;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (local_48);
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CCharString__QAE_XZ();
    }
    if ((local_2d == '\0') && (local_2f == '\x01')) {
      if (local_2e == '\0') {
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        *(undefined4 *)(iVar4 + 0x18) = 1;
        uVar3 = __D___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEAAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        puVar9 = local_9d0;
        __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                  (*(undefined4 *)(in_ECX + 0x8c));
        ___insert_AAU__pair___CB_KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair___CB_KVCThingText_CEditLevelMerger___1__Z
                  (puVar9,uVar3);
        __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                  ();
      }
      __0CCharString__QAE_PBDJ_Z("Accepting ours...",0xffffffff);
      local_8._0_1_ = 0xb;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (local_9d8);
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CCharString__QAE_XZ();
    }
    if ((local_2d == '\x01') && (local_2f == '\0')) {
      if (local_11 == '\0') {
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        *(undefined4 *)(iVar4 + 0x18) = 2;
        uVar3 = __D___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEAAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        puVar9 = local_9e8;
        __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                  (*(undefined4 *)(in_ECX + 0x8c));
        ___insert_AAU__pair___CB_KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair___CB_KVCThingText_CEditLevelMerger___1__Z
                  (puVar9,uVar3);
        __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                  ();
      }
      __0CCharString__QAE_PBDJ_Z("Accepting theirs...",0xffffffff);
      local_8._0_1_ = 0xc;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (local_9f0);
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CCharString__QAE_XZ();
    }
    if ((local_2d == '\0') && (local_2f == '\0')) {
      __0CConflict_CEditLevelMerger__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,0xd);
      if (local_2e == '\0') {
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(iVar4 + 8);
      }
      else {
        __4CCharString__QAEABV0_PBD_Z(&DAT_043c373b);
        __4CCharString__QAEABV0_PBD_Z(&DAT_043c374d);
      }
      if (local_11 == '\0') {
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(iVar4 + 8);
      }
      else {
        __4CCharString__QAEABV0_PBD_Z(&DAT_043c374e);
        __4CCharString__QAEABV0_PBD_Z(&DAT_043c374f);
      }
      puVar5 = (undefined4 *)
               __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                         ();
      local_128 = *puVar5;
      local_124 = puVar5[1];
      _push_back___vector_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std__QAEXABVCConflict_CEditLevelMerger___Z
                (local_168);
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CConflict_CEditLevelMerger__QAE_XZ();
    }
    __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
              (local_968,0);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
  }
  uVar3 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                    (local_9fc);
  local_8._0_1_ = 0xe;
  __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
            (uVar3);
  local_8._0_1_ = 2;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  while( true ) {
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_a18);
    local_8._0_1_ = 0xf;
    local_a09 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_a09 == '\0') break;
    uVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                      ();
    uVar3 = _find____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_AB_K_Z
                      (local_a24,uVar3);
    local_8._0_1_ = 0x10;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
              (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_a34);
    local_8._0_1_ = 0x11;
    local_a25 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_a25 == '\0') {
      local_3d = 1;
      uVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      uVar3 = _find____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_AB_K_Z
                        (local_a40,uVar3);
      local_8._0_1_ = 0x12;
      __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
                (uVar3);
      local_8._0_1_ = 2;
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                ();
      uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                        (local_a50);
      local_8._0_1_ = 0x13;
      local_a41 = __8___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                            (uVar3);
      local_8._0_1_ = 2;
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                ();
      if (local_a41 == '\0') {
        _Get_CMainGameComponent__SIPAV1_XZ();
        piVar2 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
        puVar5 = (undefined4 *)
                 __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                           ();
        uVar3 = puVar5[1];
        uVar10 = *puVar5;
        uVar8 = 0;
        uVar7 = 0;
        uVar11 = in_stack_00000010;
        (**(code **)(*piVar2 + 0xc))(0,0,in_stack_00000010,uVar10,uVar3);
        uVar6 = _RemapUIDForSerialisation_CThingManager__QBE_K_N0K_K_Z
                          (uVar7,uVar8,uVar11,uVar10,uVar3);
        local_170 = uVar6;
        _Get_CMainGameComponent__SIPAV1_XZ();
        piVar2 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
        uVar11 = 0;
        uVar10 = 0;
        uVar3 = in_stack_00000010;
        (**(code **)(*piVar2 + 0xc))(in_stack_00000010,0,0);
        uVar6 = _AllocateNewThingUID_CThingManager__QAE_KJ_N0_Z(uVar3,uVar10,uVar11);
        local_568 = uVar6;
        local_560 = uVar6;
        _Get_CMainGameComponent__SIPAV1_XZ();
        piVar2 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
        uVar11 = 0;
        uVar10 = 1;
        uVar3 = in_stack_00000010;
        uVar6 = local_568;
        (**(code **)(*piVar2 + 0xc))(1,0,in_stack_00000010,local_568);
        uVar6 = _RemapUIDForSerialisation_CThingManager__QBE_K_N0K_K_Z(uVar10,uVar11,uVar3,uVar6);
        local_568 = uVar6;
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        *(undefined4 *)(iVar4 + 0x18) = 1;
        iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        uVar3 = ____0AA_KAAVCThingText_CEditLevelMerger_____pair__KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
                          (&local_568,iVar4 + 8);
        local_8._0_1_ = 0x14;
        puVar9 = local_a98;
        __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                  (*(undefined4 *)(in_ECX + 0x8c));
        ___insert_U__pair__KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1___QAU__pair__KVCThingText_CEditLevelMerger___1__Z
                  (puVar9,uVar3);
        __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                  ();
        local_8._0_1_ = 2;
        __1__pair__KVCThingText_CEditLevelMerger___std__QAE_XZ();
        puVar5 = (undefined4 *)
                 __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                           (local_568);
        sprintf(local_558,"Adding their %i as %i...",*puVar5,puVar5[1]);
        __0CCharString__QAE_PBDJ_Z(local_558,0xffffffff);
        local_8._0_1_ = 0x15;
        _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                  (local_aa0);
        local_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
        uVar3 = _GFUHUGEToCharString__YI_AVCCharString___K_Z(local_568);
        local_8._0_1_ = 0x16;
        puVar5 = (undefined4 *)
                 __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                           (uVar3);
        uVar10 = _GFUHUGEToCharString__YI_AVCCharString___K_Z(*puVar5,puVar5[1]);
        local_8._0_1_ = 0x17;
        uVar3 = __0__pair_VCCharString__V1__std__QAE___QAVCCharString__0_Z(uVar10,uVar3);
        local_8._0_1_ = 0x18;
        _push_back___list_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std__QAEX__QAU__pair_VCCharString__V1__2__Z
                  (uVar3);
        local_8._0_1_ = 0x17;
        __1__pair_VCCharString__V1__std__QAE_XZ();
        local_8._0_1_ = 0x16;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
        uVar3 = _GFUHUGEToCharString__YI_AVCCharString___K_Z(local_560);
        local_8._0_1_ = 0x19;
        uVar10 = _GFUHUGEToCharString__YI_AVCCharString___K_Z(local_170);
        local_8._0_1_ = 0x1a;
        uVar3 = __0__pair_VCCharString__V1__std__QAE___QAVCCharString__0_Z(uVar10,uVar3);
        local_8._0_1_ = 0x1b;
        _push_back___list_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std__QAEX__QAU__pair_VCCharString__V1__2__Z
                  (uVar3);
        local_8._0_1_ = 0x1a;
        __1__pair_VCCharString__V1__std__QAE_XZ();
        local_8._0_1_ = 0x19;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
      }
      else {
        uVar3 = __D___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEAAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                          ();
        puVar9 = local_a60;
        __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                  (*(undefined4 *)(in_ECX + 0x8c));
        ___insert_AAU__pair___CB_KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair___CB_KVCThingText_CEditLevelMerger___1__Z
                  (puVar9,uVar3);
        __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                  ();
      }
    }
    __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
              (local_a08,0);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
  }
  uVar3 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                    (local_aec);
  local_8._0_1_ = 0x1c;
  __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
            (uVar3);
  local_8._0_1_ = 2;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  while( true ) {
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_b08);
    local_8._0_1_ = 0x1d;
    local_af9 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    uVar1 = local_3d;
    if (local_af9 == '\0') break;
    uVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                      ();
    uVar3 = _find____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_AB_K_Z
                      (local_b14,uVar3);
    local_8._0_1_ = 0x1e;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
              (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (local_b24);
    local_8._0_1_ = 0x1f;
    local_b15 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                          (uVar3);
    local_8._0_1_ = 2;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_b15 == '\0') {
      local_3d = 1;
      iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      *(undefined4 *)(iVar4 + 0x18) = 2;
      uVar3 = __D___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEAAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      puVar9 = local_b34;
      __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                (*(undefined4 *)(in_ECX + 0x8c));
      ___insert_AAU__pair___CB_KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair___CB_KVCThingText_CEditLevelMerger___1__Z
                (puVar9,uVar3);
      __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
                ();
      puVar5 = (undefined4 *)
               __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                         ();
      sprintf(local_950,"Adding our %d...",*puVar5,puVar5[1]);
      __0CCharString__QAE_PBDJ_Z(local_950,0xffffffff);
      local_8._0_1_ = 0x20;
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (local_b3c);
      local_8._0_1_ = 2;
      __1CCharString__QAE_XZ();
    }
    __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
              (local_af8,0);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
  }
  local_8._0_1_ = 1;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  ExceptionList = local_10;
  return uVar1;
}



//=== CEditLevelMerger__0CConflict @ 02b290b0 ===

/* [ported from ego_r via strfp] */

undefined4 __0CConflict_CEditLevelMerger__QAE_XZ(void)

{
  undefined4 in_ECX;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28c08;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CThingText_CEditLevelMerger__QAE_XZ();
  local_8 = 0;
  __0CThingText_CEditLevelMerger__QAE_XZ();
  ExceptionList = local_10;
  return in_ECX;
}



//=== CEditLevelMerger__MakeIDReplacementsForOurs @ 02b291c0 ===

/* WARNING: Removing unreachable block (ram,0x02b292cf) */
/* WARNING: Removing unreachable block (ram,0x02b29225) */
/* [ported from ego_r via strfp] */

void _MakeIDReplacementsForOurs_CEditLevelMerger__IAEXH_N_Z(undefined4 param_1,char param_2)

{
  undefined4 uVar1;
  int iVar2;
  size_t _Size;
  int in_ECX;
  undefined1 *puVar3;
  undefined4 uVar4;
  undefined1 local_9c [16];
  undefined1 local_8c [15];
  char local_7d;
  undefined1 local_7c [12];
  undefined1 local_70 [12];
  undefined1 local_64 [15];
  char local_55;
  undefined1 local_54 [12];
  undefined1 local_48 [12];
  void *local_3c;
  undefined4 local_38;
  void *local_34;
  undefined4 local_30;
  int local_2c;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28c5e;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_XZ
            ();
  local_8 = 0;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8._0_1_ = 1;
  if (param_2 == '\0') {
    local_2c = in_ECX + 0x58;
  }
  else {
    local_2c = in_ECX + 0x68;
  }
  uVar1 = _begin___list_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std__QAE_AV___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___2_XZ
                    (local_48);
  local_8._0_1_ = 2;
  __4___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAEAAV01_ABV01__Z
            (uVar1);
  local_8 = CONCAT31(local_8._1_3_,1);
  __1___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_XZ
            ();
  while( true ) {
    uVar1 = _end___list_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std__QAE_AV___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___2_XZ
                      (local_64);
    local_8._0_1_ = 3;
    local_55 = __9___List_const_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QBE_NABV01__Z
                         (uVar1);
    local_8._0_1_ = 1;
    __1___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_XZ
              ();
    if (local_55 == '\0') break;
    puVar3 = local_70;
    __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
              (param_1);
    uVar1 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                      (puVar3);
    local_8._0_1_ = 4;
    __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
              (uVar1);
    local_8 = CONCAT31(local_8._1_3_,1);
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
              ();
    while( true ) {
      puVar3 = local_8c;
      __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                (param_1);
      uVar1 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                        (puVar3);
      local_8._0_1_ = 5;
      local_7d = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                           (uVar1);
      local_8 = CONCAT31(local_8._1_3_,1);
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                ();
      if (local_7d == '\0') break;
      iVar2 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                        ();
      if (*(int *)(iVar2 + 0x18) == 1) {
        __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                  ();
        local_38 = _GetAsCharArray_CCharString__QBEPBDXZ();
        __C___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QBEPAU__pair_VCCharString__V1__1_XZ
                  ();
        local_30 = _GetAsCharArray_CCharString__QBEPBDXZ();
        __C___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QBEPAU__pair_VCCharString__V1__1_XZ
                  ();
        local_34 = (void *)_GetAsCharArray_CCharString__QBEPBDXZ();
        while (local_3c = (void *)_strstr__YAPADPADPBD_Z(local_38,local_30), local_3c != (void *)0x0
              ) {
          if (param_2 != '\0') {
            uVar4 = 1;
            uVar1 = __A__vector__NV__allocator__N_std___std__QAE_AV___Vb_reference_V__allocator__N_std___1_I_Z
                              (local_9c,param_1);
            local_8._0_1_ = 6;
            __4___Vb_reference_V__allocator__N_std___std__QAEAAV01__N_Z(uVar4);
            local_8 = CONCAT31(local_8._1_3_,1);
            __1___Vb_reference_V__allocator__N_std___std__QAE_XZ(uVar1);
          }
          __C___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QBEPAU__pair_VCCharString__V1__1_XZ
                    ();
          _Size = _GetLength_CCharString__QBEJXZ();
          memcpy(local_3c,local_34,_Size);
        }
      }
      __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
                (local_7c,0);
      __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                ();
    }
    __E___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_AV01_H_Z
              (local_54,0);
    __1___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_XZ
              ();
  }
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  __1___List_iterator_V___List_val_U__pair_VCCharString__V1__std__V__allocator_U__pair_VCCharString__V1__std___2__std___std__QAE_XZ
            ();
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__ConflictsResolved @ 02b29650 ===

/* WARNING: Removing unreachable block (ram,0x02b29693) */
/* [ported from ego_r via strfp] */

void _ConflictsResolved_CEditLevelMerger__QAEXXZ(void)

{
  undefined4 uVar1;
  int iVar2;
  int in_ECX;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined1 local_90 [16];
  undefined1 local_80 [15];
  char local_71;
  undefined1 local_70 [12];
  undefined1 local_64 [12];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_40;
  int local_2c [7];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28ca8;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  local_8 = 0;
  __0___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_XZ
            ();
  local_8._0_1_ = 1;
  uVar1 = _begin___vector_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___2_XZ
                    (local_64);
  local_8._0_1_ = 2;
  __4___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAEAAV01_ABV01__Z
            (uVar1);
  local_8 = CONCAT31(local_8._1_3_,1);
  __1___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_XZ
            ();
  while( true ) {
    uVar1 = _end___vector_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___2_XZ
                      (local_80);
    local_8._0_1_ = 3;
    local_71 = FID_conflict_operator__(uVar1);
    local_8._0_1_ = 1;
    __1___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_XZ
              ();
    if (local_71 == '\0') break;
    __0__pair__KVCThingText_CEditLevelMerger___std__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,4);
    iVar2 = __C___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QBEPAVCConflict_CEditLevelMerger__XZ
                      ();
    local_58 = *(undefined4 *)(iVar2 + 0x40);
    local_54 = *(undefined4 *)(iVar2 + 0x44);
    iVar2 = __C___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QBEPAVCConflict_CEditLevelMerger__XZ
                      ();
    if (*(char *)(iVar2 + 0x38) == '\x01') {
      uVar1 = __C___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QBEPAVCConflict_CEditLevelMerger__XZ
                        ();
      __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(uVar1);
      local_40 = 1;
    }
    else {
      iVar2 = __C___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QBEPAVCConflict_CEditLevelMerger__XZ
                        ();
      __4CThingText_CEditLevelMerger__QAEAAV01_ABV01__Z(iVar2 + 0x1c);
      local_40 = 2;
    }
    puVar4 = &local_58;
    puVar3 = local_90;
    __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
              (*(undefined4 *)(in_ECX + 0x8c));
    ___insert_AAU__pair__KVCThingText_CEditLevelMerger___std______Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_1_AAU__pair__KVCThingText_CEditLevelMerger___1__Z
              (puVar3,puVar4);
    __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std___N_std__QAE_XZ
              ();
    local_8 = CONCAT31(local_8._1_3_,1);
    __1__pair__KVCThingText_CEditLevelMerger___std__QAE_XZ();
    __E___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_AV01_H_Z
              (local_70,0);
    __1___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_XZ
              ();
  }
  _MakeIDReplacementsForOurs_CEditLevelMerger__IAEXH_N_Z(*(undefined4 *)(in_ECX + 0x8c),0);
  _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
            (in_ECX + 0x98);
  local_2c[0] = *(int *)(in_ECX + 0xe4) + -1;
  _push_back___vector_HV__allocator_H_std___std__QAEXABH_Z(local_2c);
  *(int *)(in_ECX + 0x8c) = *(int *)(in_ECX + 0x8c) + 1;
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Vector_iterator_V___Vector_val_VCConflict_CEditLevelMerger__V__allocator_VCConflict_CEditLevelMerger___std___std___std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
            ();
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__LoadMapIntoMemory @ 02b299d0 ===

/* [ported from ego_r via strfp] */

void _LoadMapIntoMemory_CEditLevelMerger__IAEXJ_Z(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  
  _Get_CMainGameComponent__SIPAV1_XZ();
  _GetEditControlCentre_CEditComponent__QAEAAVCEditControlCentre__XZ();
  _GetEditWorldMap_CEditControlCentre__QAEAAVCEditWorldMap__XZ();
  _Get_CMainGameComponent__SIPAV1_XZ();
  piVar1 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  (**(code **)(*piVar1 + 0x30))();
  iVar2 = _GetRegionNumberMapIsIn_CWorldMap__QBEJJ_Z(param_1);
  if (iVar2 < 1) {
    _SetMapAsEditable_CEditWorldMap__QAEXJ_N_Z(param_1,0);
    _SetMapAsEditable_CEditWorldMap__QAEXJ_N_Z(param_1,1);
  }
  else {
    _SetRegionAsEditable_CEditWorldMap__QAEXJ_N_Z(iVar2,0);
    _SetRegionAsEditable_CEditWorldMap__QAEXJ_N_Z(iVar2,1);
  }
  return;
}



//=== CEditLevelMerger__Finito @ 02b29a90 ===

/* WARNING: Removing unreachable block (ram,0x02b29f28) */
/* WARNING: Removing unreachable block (ram,0x02b29d16) */
/* WARNING: Removing unreachable block (ram,0x02b29d87) */
/* WARNING: Removing unreachable block (ram,0x02b29adc) */
/* [ported from ego_r via strfp] */

void _Finito_CEditLevelMerger__IAEXXZ(void)

{
  char cVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  int in_ECX;
  undefined1 *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  undefined1 local_f4 [15];
  char local_e5;
  undefined1 local_e4 [12];
  undefined1 local_d8 [20];
  undefined1 local_c4 [15];
  char local_b5;
  undefined1 local_b4 [12];
  undefined1 local_a8 [12];
  undefined1 local_9c [8];
  undefined1 local_94 [8];
  undefined1 local_8c [11];
  char local_81;
  undefined1 local_80 [19];
  char local_6d;
  undefined1 local_6c [8];
  undefined1 local_64 [19];
  char local_51;
  uint local_50;
  undefined1 local_4c;
  undefined1 local_4b;
  CAsyncMonikerFile local_48 [32];
  undefined1 local_28 [20];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e28d50;
  local_10 = ExceptionList;
  if (*(char *)(in_ECX + 0xe8) != '\x01') {
    ExceptionList = &local_10;
    *(undefined1 *)(in_ECX + 0xe8) = 1;
    local_14 = 0;
    while (uVar2 = _size___vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QBEIXZ
                             (), local_14 < uVar2) {
      _MakeIDReplacementsForOurs_CEditLevelMerger__IAEXH_N_Z(local_14,1);
      __A__vector__NV__allocator__N_std___std__QAE_AV___Vb_reference_V__allocator__N_std___1_I_Z
                (local_80,local_14);
      local_8 = 0;
      cVar1 = __B___Vb_reference_V__allocator__N_std___std__QBE_NXZ();
      local_6d = '\x01' - (cVar1 != '\0');
      local_8 = 0xffffffff;
      __1___Vb_reference_V__allocator__N_std___std__QAE_XZ();
      if (local_6d == '\0') {
        __0___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                  ();
        local_8 = 1;
        __0CDiskFileWin32__QAE_XZ();
        local_8._0_1_ = 2;
        puVar6 = local_8c;
        __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                  (local_14);
        uVar3 = _ToWideString_CCharString__QBE_AVCWideString__XZ(puVar6);
        local_8._0_1_ = 3;
        __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ(uVar3);
        cVar1 = _IsFileOpenForEdit_CP4Api_NP4Api__QBE_NABVCWideString___Z(uVar3);
        local_81 = '\x01' - (cVar1 != '\0');
        local_8._0_1_ = 2;
        __1CWideString__QAE_XZ();
        if (local_81 != '\0') {
          puVar6 = local_94;
          __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                    (local_14);
          uVar3 = _ToWideString_CCharString__QBE_AVCWideString__XZ(puVar6);
          local_8._0_1_ = 4;
          __C__CCountedPointer_VCP4Api_NP4Api____QBEPAVCP4Api_NP4Api__XZ(uVar3);
          _OpenFileForEdit_CP4Api_NP4Api__QAE_NABVCWideString___Z(uVar3);
          local_8._0_1_ = 2;
          __1CWideString__QAE_XZ();
        }
        uVar8 = 2;
        uVar7 = 4;
        puVar6 = local_9c;
        __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                  (local_14);
        uVar3 = _ToWideString_CCharString__QBE_AVCWideString__XZ(puVar6);
        local_8._0_1_ = 5;
        _Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z(uVar3,uVar7,uVar8);
        local_8._0_1_ = 2;
        __1CWideString__QAE_XZ();
        __0CCharString__QAE_PBDJ_Z("Version 2;",0xffffffff);
        local_8 = CONCAT31(local_8._1_3_,6);
        ___WriteVar_VCCharString___CAFile__QAEXABVCCharString___Z(local_28);
        local_4c = 0xd;
        local_4b = 10;
        _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
        _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
        local_50 = 0;
        while( true ) {
          __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                    (local_14);
          uVar2 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ();
          if (uVar2 <= local_50) break;
          local_51 = '\x01';
          uVar2 = local_50;
          __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                    (local_14);
          uVar3 = __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                            (uVar2);
          __0CCharString__QAE_ABV0__Z(uVar3);
          local_8._0_1_ = 7;
          puVar6 = local_a8;
          __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                    (local_14);
          uVar3 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                            (puVar6);
          local_8._0_1_ = 8;
          __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
                    (uVar3);
          local_8 = CONCAT31(local_8._1_3_,7);
          __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                    ();
          while( true ) {
            puVar6 = local_c4;
            __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                      (local_14);
            uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                              (puVar6);
            local_8._0_1_ = 9;
            local_b5 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                                 (uVar3);
            local_8 = CONCAT31(local_8._1_3_,7);
            __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                      ();
            if (local_b5 == '\0') break;
            iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                              ();
            iVar4 = iVar4 + 0x1c;
            uVar2 = local_50;
            __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                      (local_14);
            __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                      (uVar2);
            cVar1 = __8CCharString__QBE_NABV0__Z(iVar4);
            if (cVar1 != '\0') {
              local_51 = '\0';
              break;
            }
            __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
                      (local_b4,0);
            __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                      ();
          }
          if (local_51 == '\0') {
            puVar9 = &DAT_043c384c;
            iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                              (&DAT_043c384c);
            __H_YI_AVCCharString__PBDABV0__Z(iVar4 + 0x1c);
            local_8._0_1_ = 10;
            __H_YI_AVCCharString__ABV0_PBD_Z(puVar9);
            local_8._0_1_ = 0xc;
            __1CCharString__QAE_XZ();
            ___WriteVar_VCCharString___CAFile__QAEXABVCCharString___Z(local_6c);
            _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
            puVar6 = local_d8;
            __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                      (local_14);
            uVar3 = _begin____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                              (puVar6);
            local_8._0_1_ = 0xd;
            __4___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAEAAV01_ABV01__Z
                      (uVar3);
            local_8._0_1_ = 0xc;
            __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                      ();
            while( true ) {
              puVar6 = local_f4;
              __A__vector_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std__V__allocator_V__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__std___2__std__QAEAAV__map__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__1_I_Z
                        (local_14);
              uVar3 = _end____Tree_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___2_XZ
                                (puVar6);
              local_8._0_1_ = 0xe;
              local_e5 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBE_NABV01__Z
                                   (uVar3);
              local_8._0_1_ = 0xc;
              __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                        ();
              if (local_e5 == '\0') break;
              iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                                ();
              iVar4 = iVar4 + 0x1c;
              uVar2 = local_50;
              __A__vector_V__vector_VCCharString__V__allocator_VCCharString___std___std__V__allocator_V__vector_VCCharString__V__allocator_VCCharString___std___std___2__std__QAEAAV__vector_VCCharString__V__allocator_VCCharString___std___1_I_Z
                        (local_14);
              __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                        (uVar2);
              cVar1 = __8CCharString__QBE_NABV0__Z(iVar4);
              if (cVar1 != '\0') {
                iVar4 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QBEPAU__pair___CB_KVCThingText_CEditLevelMerger___1_XZ
                                  ();
                ___WriteVar_VCCharString___CAFile__QAEXABVCCharString___Z(iVar4 + 0x10);
              }
              __E___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_AV01_H_Z
                        (local_e4,0);
              __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                        ();
            }
            __0CCharString__QAE_PBDJ_Z("XXXSectionEnd;",0xffffffff);
            local_8._0_1_ = 0xf;
            _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
            _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
            ___WriteVar_VCCharString___CAFile__QAEXABVCCharString___Z(local_64);
            _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
            _Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z(&local_4c,2,0);
            local_8._0_1_ = 0xc;
            __1CCharString__QAE_XZ();
            local_8 = CONCAT31(local_8._1_3_,7);
            __1CCharString__QAE_XZ();
          }
          local_8 = CONCAT31(local_8._1_3_,6);
          __1CCharString__QAE_XZ();
          local_50 = local_50 + 1;
        }
        _Close_CDiskFileWin32__UAEXXZ();
        puVar5 = (undefined4 *)__A__vector_HV__allocator_H_std___std__QAEAAHI_Z(local_14);
        _LoadMapIntoMemory_CEditLevelMerger__IAEXJ_Z(*puVar5);
        local_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
        local_8 = CONCAT31(local_8._1_3_,1);
        CAsyncMonikerFile::~CAsyncMonikerFile(local_48);
        local_8 = 0xffffffff;
        __1___Tree_iterator_V___Tree_val_V___Tmap_traits__KVCThingText_CEditLevelMerger__U__less__K_std__V__allocator_U__pair___CB_KVCThingText_CEditLevelMerger___std___4__0A__std___std___std__QAE_XZ
                  ();
      }
      local_14 = local_14 + 1;
    }
  }
  ExceptionList = local_10;
  return;
}



//=== CEditLevelMerger__0AA_KAAVCThingText @ 02b2f4a0 ===

/* [ported from ego_r via strfp] */

undefined4
____0AA_KAAVCThingText_CEditLevelMerger_____pair__KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
          (void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 in_ECX;
  
  uVar1 = ___forward_AAVCThingText_CEditLevelMerger___std__YIAAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  uVar2 = ___forward_AA_K_std__YIAA_KAA_K_Z(uVar1);
  ____0AA_KAAVCThingText_CEditLevelMerger______Pair_base__KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
            (uVar2,uVar1);
  return in_ECX;
}



//=== CEditLevelMerger__0VCConflict @ 02b2f7f0 ===

/* [ported from ego_r via strfp] */

undefined4
____0VCConflict_CEditLevelMerger_____allocator_U_Container_proxy_std___std__QAE_ABV__allocator_VCConflict_CEditLevelMerger___1__Z
          (void)

{
  undefined4 in_ECX;
  
  return in_ECX;
}



//=== CEditLevelMerger__0W4CameFrom @ 02b2f8c0 ===

/* [ported from ego_r via strfp] */

undefined4
____0W4CameFrom_CEditLevelMerger_____allocator_U_Container_proxy_std___std__QAE_ABV__allocator_W4CameFrom_CEditLevelMerger___1__Z
          (void)

{
  undefined4 in_ECX;
  
  return in_ECX;
}



//=== CEditLevelMerger__0AA_KAAVCThingText @ 02b30ab0 ===

/* [ported from ego_r via strfp] */

undefined4 *
____0AA_KAAVCThingText_CEditLevelMerger______Pair_base__KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
          (void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *in_ECX;
  
  puVar1 = (undefined4 *)___forward_AA_K_std__YIAA_KAA_K_Z();
  *in_ECX = *puVar1;
  in_ECX[1] = puVar1[1];
  uVar2 = ___forward_AAVCThingText_CEditLevelMerger___std__YIAAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(uVar2);
  return in_ECX;
}



//=== CEditLevelMerger__0CThingText @ 02b31580 ===

/* [ported from ego_r via strfp] */

int __0CThingText_CEditLevelMerger__QAE_ABV01__Z(int param_1)

{
  int in_ECX;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e295d3;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CCharString__QAE_ABV0__Z(param_1);
  local_8 = 0;
  __0CCharString__QAE_ABV0__Z(param_1 + 8);
  local_8 = CONCAT31(local_8._1_3_,1);
  *(undefined4 *)(in_ECX + 0x10) = *(undefined4 *)(param_1 + 0x10);
  __0CCharString__QAE_ABV0__Z(param_1 + 0x14);
  ExceptionList = local_10;
  return in_ECX;
}



//=== CEditLevelMerger__0CConflict @ 02b31620 ===

/* [ported from ego_r via strfp] */

int __0CConflict_CEditLevelMerger__QAE_ABV01__Z(int param_1)

{
  int in_ECX;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e295f8;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(param_1);
  local_8 = 0;
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(param_1 + 0x1c);
  *(undefined1 *)(in_ECX + 0x38) = *(undefined1 *)(param_1 + 0x38);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  ExceptionList = local_10;
  return in_ECX;
}



//=== CEditLevelMerger__0_KVCThingText @ 02b32560 ===

/* [ported from ego_r via strfp] */

undefined4
____0_KVCThingText_CEditLevelMerger_____pair___CB_KVCThingText_CEditLevelMerger___std__QAE___QAU__pair__KVCThingText_CEditLevelMerger___1__Z
          (void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 in_ECX;
  
  uVar1 = ___forward_VCThingText_CEditLevelMerger___std__YI__QAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  uVar2 = ___forward__K_std__YI__QA_KAA_K_Z(uVar1);
  ____0_KVCThingText_CEditLevelMerger______Pair_base___CB_KVCThingText_CEditLevelMerger___std__QAE___QA_K__QAVCThingText_CEditLevelMerger___Z
            (uVar2,uVar1);
  return in_ECX;
}



//=== CEditLevelMerger__0_KVCThingText @ 02b325d0 ===

/* [ported from ego_r via strfp] */

undefined4
____0_KVCThingText_CEditLevelMerger_____pair___CB_KVCThingText_CEditLevelMerger___std__QAE_AAU__pair__KVCThingText_CEditLevelMerger___1__Z
          (int param_1)

{
  undefined4 in_ECX;
  
  ____0AA_KAAVCThingText_CEditLevelMerger______Pair_base___CB_KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
            (param_1,param_1 + 8);
  return in_ECX;
}



//=== CEditLevelMerger__0_KVCThingText @ 02b32840 ===

/* [ported from ego_r via strfp] */

undefined4 *
____0_KVCThingText_CEditLevelMerger______Pair_base___CB_KVCThingText_CEditLevelMerger___std__QAE___QA_K__QAVCThingText_CEditLevelMerger___Z
          (void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *in_ECX;
  
  puVar1 = (undefined4 *)___forward__K_std__YI__QA_KAA_K_Z();
  *in_ECX = *puVar1;
  in_ECX[1] = puVar1[1];
  uVar2 = ___forward_VCThingText_CEditLevelMerger___std__YI__QAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(uVar2);
  return in_ECX;
}



//=== CEditLevelMerger__0AB_KAAVCThingText @ 02b32890 ===

/* [ported from ego_r via strfp] */

undefined4 *
____0AB_KAAVCThingText_CEditLevelMerger______Pair_base___CB_KVCThingText_CEditLevelMerger___std__QAE_AB_KAAVCThingText_CEditLevelMerger___Z
          (void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *in_ECX;
  
  puVar1 = (undefined4 *)___forward_AB_K_std__YIAB_KAB_K_Z();
  *in_ECX = *puVar1;
  in_ECX[1] = puVar1[1];
  uVar2 = ___forward_AAVCThingText_CEditLevelMerger___std__YIAAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(uVar2);
  return in_ECX;
}



//=== CEditLevelMerger__0AA_KAAVCThingText @ 02b328e0 ===

/* [ported from ego_r via strfp] */

undefined4 *
____0AA_KAAVCThingText_CEditLevelMerger______Pair_base___CB_KVCThingText_CEditLevelMerger___std__QAE_AA_KAAVCThingText_CEditLevelMerger___Z
          (void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *in_ECX;
  
  puVar1 = (undefined4 *)___forward_AA_K_std__YIAA_KAA_K_Z();
  *in_ECX = *puVar1;
  in_ECX[1] = puVar1[1];
  uVar2 = ___forward_AAVCThingText_CEditLevelMerger___std__YIAAVCThingText_CEditLevelMerger__AAV12__Z
                    ();
  __0CThingText_CEditLevelMerger__QAE_ABV01__Z(uVar2);
  return in_ECX;
}



