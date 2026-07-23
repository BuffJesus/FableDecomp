INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1216 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2668 ms) (AnalyzeHeadless)  
INFO  Class searcher loaded 60 extension points (20 false positives) (ClassSearcher)  
INFO  HEADLESS Script Paths:
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SwiftDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\GnuDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BytePatterns\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\JVM\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SystemEmulation\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\8051\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FileFormats\ghidra_scripts
    D:\Documents\FableTLC\tools\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BSim\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\VersionTracking\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PDB\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\WildcardAssembler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\PIC\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Base\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Extensions\XEXLoaderWV\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\Cornelio\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\DATA\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Decompiler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\Atmel\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PyGhidra\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FunctionID\ghidra_scripts (HeadlessAnalyzer)  
INFO  HEADLESS: execution starts (HeadlessAnalyzer)  
INFO  Opening existing project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessAnalyzer)  
INFO  Opening project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessProject)  
INFO  REPORT: Processing project file: /FableWin.exe (HeadlessAnalyzer)  
INFO  REPORT: Execute script: DecompFuncs.java '0x0296d2d0' '0x0296e150' '0x0296e8b0' '0x0204c3c0' '0x02952140' '0x0296d920' '0x0296ade0' '0x0296b5e0' '0x01c9fcf0' '0x01c9fb80' '0x029524a0' '0x0296f070'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
INFO  DecompFuncs.java> ==================== ?CreateAndSaveNewLevel@CEditWorldMap@@QAEJVCWideString@@ABVC2DCoordI@@ABVCMapInit@@@Z @ 0296d2d0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

undefined4 __thiscall
_CreateAndSaveNewLevel_CEditWorldMap__QAEJVCWideString__ABVC2DCoordI__ABVCMapInit___Z
          (int *param_1,undefined4 param_2,undefined4 param_3,int *param_4,int *param_5)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  BOOL BVar6;
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
  local_14 = (undefined1 *)(**(code **)(*param_1 + 0x78))();
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
  piStack_b8 = (int *)(*param_4 + *param_5);
  puStack_bc = (undefined1 *)param_4[1];
  puStack_c0 = (undefined1 *)*param_4;
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
    piStack_b8 = param_4;
    puStack_bc = (undefined1 *)0x296d3d9;
    puStack_bc = (undefined1 *)_GetWorldSeed_CWorld__QBEPAKXZ();
    puStack_c0 = (undefined1 *)param_1[2];
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
  puStack_c4 = (undefined1 *)&param_2;
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

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveSingleLevel@CEditWorldMap@@QAE?AVCEditLevelSaveInfo@@JVCWideString@@_N1@Z @ 0296e150 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

undefined4 * __thiscall
_SaveSingleLevel_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JVCWideString___N1_Z
          (int param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
          char param_6,char param_7)

{
  bool bVar1;
  char cVar2;
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
  if (*(char *)(param_1 + 0x17d) != '\0') {
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
            (&local_1c,param_3);
  local_12 = 1;
  local_11 = '\x01';
  if ((param_7 != '\0') && (cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z(), cVar2 != '\0')) {
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
    pcStack_15c = (char *)&param_4;
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
    if ((param_7 != '\0') && (cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z(), cVar2 != '\0'))
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
  if (param_6 == '\0') {
    uVar3 = _GetFileModifiedTimeAsSomeWackyWin32BigNumber_CAFile__SI_KABVCWideString___Z();
    *(undefined8 *)(local_38 + 0x40) = uVar3;
    uVar3 = _GetFileModifiedTimeAsSomeWackyWin32BigNumber_CAFile__SI_KABVCWideString___Z();
    *(undefined8 *)(local_38 + 0x48) = uVar3;
  }
  *param_2 = local_1c;
  param_2[1] = local_18;
  local_8._0_1_ = 7;
  __1__CCountedPointer_VCMemoryFile____QAE_XZ();
  local_8._0_1_ = 1;
  __1__CCountedPointer_VCMemoryFile____QAE_XZ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1CWideString__QAE_XZ();
  local_8 = 0xffffffff;
  __1CWideString__QAE_XZ();
  ExceptionList = local_10;
  return param_2;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveSingleLevelToFiles@CEditWorldMap@@QAE?AVCEditLevelSaveInfo@@JV?$CCountedPointer@VCAFile@@@@0@Z @ 0296e8b0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

int * __thiscall
_SaveSingleLevelToFiles_CEditWorldMap__QAE_AVCEditLevelSaveInfo__JV__CCountedPointer_VCAFile____0_Z
          (int param_1,int *param_2,undefined4 param_3)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
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
  uVar2 = param_3;
  (**(code **)(**(int **)(param_1 + 8) + 0xc))(puVar6,param_3,pCVar7);
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
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_3);
  __C__CCountedPointer_VCMap____QBEPAVCMap__XZ(uVar2);
  _SaveToFile_CMap__QBEXAAVCAFile__AAVCFileFormatLevel___Z(uVar2,puVar6);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  puVar6 = local_58;
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(puVar6);
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_3);
  __C__CCountedPointer_VCMap____QBEPAVCMap__XZ(uVar2);
  _SaveBrushesToFile_CMap__QBEXAAVCAFile__AAVCFileFormatLevel___Z(uVar2,puVar6);
  piVar1 = (int *)__C__CCountedPointer_VCAFile____QBEPAVCAFile__XZ();
  local_20 = (**(code **)(*piVar1 + 0x1c))();
  pCVar7 = local_34;
  puVar6 = local_58;
  uVar2 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(puVar6,pCVar7);
  _SaveNavigationMapToFile_CWorldMap__QBEXJAAVCAFile__AAVCFileFormatLevel__ABV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
            (param_3,uVar2,puVar6,pCVar7);
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
  *param_2 = iVar5;
  param_2[1] = iVar4;
  local_8._0_1_ = 2;
  ATL::CAtlWinModule::~CAtlWinModule(local_34);
  local_8._0_1_ = 1;
  __1CCharString__QAE_XZ();
  local_8 = (uint)local_8._1_3_ << 8;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  local_8 = 0xffffffff;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  ExceptionList = local_10;
  return param_2;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?CreateAndSaveNewLevel@CEditControlCentre@@QAEXABVCWideString@@ABVC2DCoordI@@ABVCMapInit@@@Z @ 0204c3c0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _CreateAndSaveNewLevel_CEditControlCentre__QAEXABVCWideString__ABVC2DCoordI__ABVCMapInit___Z
               (undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 auStack_30 [8];
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03ce42f8;
  local_10 = ExceptionList;
  uStack_24 = param_3;
  uStack_28 = param_2;
  local_14 = auStack_30;
  ExceptionList = &local_10;
  local_20 = __0CWideString__QAE_ABV0__Z(param_1);
  local_8 = 0;
  local_1c = local_20;
  _GetEditWorldMap_CEditControlCentre__QAEAAVCEditWorldMap__XZ();
  local_8 = 0xffffffff;
  _CreateAndSaveNewLevel_CEditWorldMap__QAEJVCWideString__ABVC2DCoordI__ABVCMapInit___Z();
  uStack_24 = 0x204c425;
  _InitRegionDialog_CEditControlCentre__QAEXXZ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?CreateAndSaveNewLevel@CEditInputProcessMapPlacement@@AAEXABVC2DBoxI@@@Z @ 02952140 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _CreateAndSaveNewLevel_CEditInputProcessMapPlacement__AAEXABVC2DBoxI___Z(void)

{
  char cVar1;
  int *piVar2;
  undefined4 uVar3;
  BOOL BVar4;
  undefined4 *puVar5;
  undefined1 local_494 [8];
  undefined1 local_48c [19];
  char local_479;
  undefined4 local_478;
  undefined4 local_474;
  tagOFNW local_470;
  wchar_t local_410 [512];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03def241;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  piVar2 = (int *)_GetMouse_CInputManager__QAEAAVCMouse__XZ();
  (**(code **)(*piVar2 + 0x28))(1);
  __0CWideString__QAE_XZ();
  local_8 = 0;
  _GetLevelDir_NResourceDirectoryNames__YI_AVCWideString__XZ();
  local_8._0_1_ = 1;
  cVar1 = _ConvertPathToFullPath_CAFile__SI_NABVCWideString__AAV2__Z();
  local_479 = '\x01' - (cVar1 != '\0');
  local_8 = (uint)local_8._1_3_ << 8;
  __1CWideString__QAE_XZ();
  if (local_479 != '\0') {
    __4CWideString__QAEABV0_PB_W_Z(&DAT_043448ac);
  }
  uVar3 = _GetAsCharArray_CWideString__QBEPB_WXZ();
  _swprintf(local_410,L"%s",uVar3);
  local_470.lStructSize = 0x58;
  _GFGetSystemManager__YIPAVCSystemManager__XZ();
  local_470.hwndOwner = (HWND)_GetApplicationWinHandle_CSystemManager__QBEPAUHWND____XZ();
  local_470.hInstance = (HINSTANCE)0x0;
  local_470.lpstrFilter = L"Level Files (*.lev)";
  local_470.lpstrCustomFilter = (LPWSTR)0x0;
  local_470.nMaxCustFilter = 0;
  local_470.nFilterIndex = 1;
  local_470.lpstrFile = u___lev_04a58ed0;
  local_470.nMaxFile = 0x200;
  local_470.lpstrFileTitle = (LPWSTR)&DAT_04ac4fb8;
  local_470.nMaxFileTitle = 0x200;
  local_470.lpstrInitialDir = local_410;
  local_470.lpstrTitle = L"Save Level File";
  local_470.Flags = 0x8000;
  local_470.nFileOffset = 0;
  local_470.nFileExtension = 1;
  local_470.lpstrDefExt = L".Lev";
  local_470.lCustData = 0;
  local_470.lpfnHook = (LPOFNHOOKPROC)0x0;
  local_470.lpTemplateName = (LPCWSTR)0x0;
  local_470.pvReserved = (void *)0x0;
  local_470.dwReserved = 0;
  local_470.FlagsEx = 0;
  _FlipToGDISurface_CDisplayManager__QAEXXZ();
  BVar4 = GetSaveFileNameW(&local_470);
  if (BVar4 != 0) {
    __0CMapInit__QAE_XZ();
    local_478 = _GetWidth_C2DBoxI__QBEJXZ();
    local_474 = _GetHeight_C2DBoxI__QBEJXZ();
    __0CWideString__QAE_PB_W_Z(local_470.lpstrFile);
    local_8._0_1_ = 2;
    puVar5 = &local_478;
    uVar3 = _GetTLPos_C2DBoxI__QBE_AVC2DCoordI__XZ(local_48c);
    _CreateAndSaveNewLevel_CEditControlCentre__QAEXABVCWideString__ABVC2DCoordI__ABVCMapInit___Z
              (local_494,uVar3,puVar5);
    local_8 = (uint)local_8._1_3_ << 8;
    __1CWideString__QAE_XZ();
  }
  piVar2 = (int *)_GetMouse_CInputManager__QAEAAVCMouse__XZ();
  (**(code **)(*piVar2 + 0x28))(0);
  local_8 = 0xffffffff;
  __1CWideString__QAE_XZ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?CreateNewRegion@CEditWorldMap@@QAEJXZ @ 0296d920 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
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

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?AddLevelFromFileWithUID@CEditWorldMap@@QAEJABVC2DCoordI@@ABVCCharString@@K@Z @ 0296ade0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

int _AddLevelFromFileWithUID_CEditWorldMap__QAEJABVC2DCoordI__ABVCCharString__K_Z
              (undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  int iVar4;
  undefined4 extraout_var;
  
  iVar1 = _AddLevelFromFile_CWorldMap__IAEJABVC2DCoordI__ABVCCharString___Z(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ(extraout_var);
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

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveToFile@CEditWorldMap@@UAEXAAVCAFile@@@Z @ 0296b5e0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
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

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveBinary@CMapInfo@@QAEXAAVCDataOutputStream@@@Z @ 01c9fcf0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void __fastcall _SaveBinary_CMapInfo__QAEXAAVCDataOutputStream___Z(undefined4 *param_1)

{
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 6);
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 8);
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0xb));
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0xc));
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0x14));
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(*param_1);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(param_1[2]);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(param_1[1]);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(param_1[3]);
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)((int)param_1 + 0x2d));
  _WriteUHUGE_CDataOutputStream__QAEX_K_Z(param_1[10],0);
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?LoadBinary@CMapInfo@@QAEXAAVCDataInputStream@@@Z @ 01c9fb80 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void __fastcall _LoadBinary_CMapInfo__QAEXAAVCDataInputStream___Z(undefined4 *param_1)

{
  undefined1 uVar1;
  undefined4 uVar2;
  undefined1 local_20 [8];
  undefined1 local_18 [8];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c7a190;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_18);
  local_8 = 0;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_20);
  local_8 = 1;
  __4CCharString__QAEABV0_ABV0__Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  *(undefined1 *)(param_1 + 0xb) = uVar1;
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  *(undefined1 *)(param_1 + 0xc) = uVar1;
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  *(undefined1 *)(param_1 + 0x14) = uVar1;
  uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
  *param_1 = uVar2;
  uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
  param_1[2] = uVar2;
  uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
  param_1[1] = uVar2;
  uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
  param_1[3] = uVar2;
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  *(undefined1 *)((int)param_1 + 0x2d) = uVar1;
  uVar2 = _ReadUHUGE_CDataInputStream__QAE_KXZ();
  param_1[10] = uVar2;
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ??0CMapInit@@QAE@XZ @ 029524a0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

undefined4 * __fastcall __0CMapInit__QAE_XZ(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  return param_1;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?EditLoadLevel@CEditWorldMap@@AAEXJ@Z @ 0296f070 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _EditLoadLevel_CEditWorldMap__AAEXJ_Z(undefined4 param_1)

{
  _LoadLevel_CWorldMap__IAEXJW4ELoadType_1__Z(param_1,0);
  return;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /FableWin.exe (HeadlessAnalyzer)  
EXIT=0
