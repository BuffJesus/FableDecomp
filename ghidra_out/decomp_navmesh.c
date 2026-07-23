INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1004 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2358 ms) (AnalyzeHeadless)  
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
INFO  REPORT: Execute script: DecompFuncs.java '0x018d90c0' '0x018d9100' '0x0204b700' '0x01c8a2c0' '0x01c8aec0' '0x027dabf0' '0x027da620' '0x027da930' '0x027db600' '0x027db2c0' '0x027da6a0' '0x01c8e300' '0x03289b50' '0x0328a760' '0x03290030' '0x0328f270' '0x01c89d20' '0x01c8e7d0'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
INFO  DecompFuncs.java> ==================== ?ConsoleGenerateRegionGraph@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z @ 018d90c0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _ConsoleGenerateRegionGraph_NGlobalConsole__YIXABV__vector_VCGenericVar__V__allocator_VCGenericVar___std___std___Z
               (void)

{
  int *piVar1;
  undefined4 extraout_var;
  undefined4 uVar2;
  
  _Get_CMainGameComponent__SIPAV1_XZ();
  uVar2 = extraout_var;
  piVar1 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  (**(code **)(*piVar1 + 0x30))(piVar1,uVar2);
  _GenerateRegionConnectivityGraph_CWorldMap__QAEXXZ();
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?ConsoleLoadRegionGraphData@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z @ 018d9100 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _ConsoleLoadRegionGraphData_NGlobalConsole__YIXABV__vector_VCGenericVar__V__allocator_VCGenericVar___std___std___Z
               (void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  undefined1 local_20 [8];
  undefined1 local_18 [8];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c112e0;
  local_10 = ExceptionList;
  puVar3 = local_18;
  ExceptionList = &local_10;
  __A__vector_VCGenericVar__V__allocator_VCGenericVar___std___std__QBEABVCGenericVar__I_Z(0);
  _GetString_CGenericVar__QBE_AVCCharString__XZ(puVar3);
  local_8 = 0;
  _Get_CMainGameComponent__SIPAV1_XZ();
  piVar1 = (int *)_GetWorld_CMainGameComponent__QAEAAVCWorld__XZ();
  uVar2 = _ToWideString_CCharString__QBE_AVCWideString__XZ(local_20);
  local_8._0_1_ = 1;
  (**(code **)(*piVar1 + 0x30))(uVar2);
  _LoadRegionGraph_CWorldMap__QAEXABVCWideString___Z(uVar2);
  local_8 = (uint)local_8._1_3_ << 8;
  __1CWideString__QAE_XZ();
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?GenerateNavigationInformation@CEditControlCentre@@AAEXXZ @ 0204b700 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x0204b71f) */
/* [ported from ego_r via strfp] */

void _GenerateNavigationInformation_CEditControlCentre__AAEXXZ(void)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int local_c;
  
  _GetEditWorld_CEditControlCentre__QBEAAVCEditWorld__XZ();
  _GetNavigatorManager_CWorld__QAEAAVCNavigatorManager__XZ();
  local_c = 1;
  while( true ) {
    _GetEditWorldMap_CEditControlCentre__QAEAAVCEditWorldMap__XZ();
    iVar2 = _GetNoMaps_CWorldMap__QBEJXZ();
    if (iVar2 <= local_c) break;
    piVar3 = (int *)_GetEditWorldMap_CEditControlCentre__QAEAAVCEditWorldMap__XZ();
    cVar1 = (**(code **)(*piVar3 + 0x28))(local_c);
    if (cVar1 != '\0') {
      _RemoveNavigationMap_CNavigatorManager__QAEXJ_Z(local_c);
      iVar2 = local_c;
      _GetEditWorldMap_CEditControlCentre__QAEAAVCEditWorldMap__XZ(local_c);
      _ActivateNavMap_CWorldMap__QAEXJ_Z(iVar2);
    }
    local_c = local_c + 1;
  }
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?GenerateRegionConnectivityGraph@CWorldMap@@QAEXXZ @ 01c8a2c0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x01c8aa0e) */
/* WARNING: Removing unreachable block (ram,0x01c8a3da) */
/* WARNING: Removing unreachable block (ram,0x01c8a356) */
/* WARNING: Removing unreachable block (ram,0x01c8a68d) */
/* [ported from ego_r via strfp] */

void __fastcall _GenerateRegionConnectivityGraph_CWorldMap__QAEXXZ(int *param_1)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  char cVar10;
  uint uVar11;
  undefined4 uVar12;
  BOOL BVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  char *pcVar16;
  undefined1 *puVar17;
  CAtlWinModule *pCVar18;
  undefined4 *puVar19;
  _ATL_WIN_MODULE70 **pp_Var20;
  undefined *puVar21;
  char local_170;
  char local_15c;
  undefined1 local_140 [24];
  undefined1 local_128 [15];
  char local_119;
  undefined1 local_108 [16];
  undefined1 local_f8 [24];
  undefined1 local_e0 [10];
  char local_d6;
  undefined1 local_d4 [16];
  undefined1 local_c4 [24];
  undefined1 local_ac [11];
  char local_a1;
  undefined1 local_9c [12];
  undefined1 local_90 [16];
  _ATL_WIN_MODULE70 *local_80;
  uint local_7c;
  undefined4 local_78;
  undefined4 local_74;
  uint local_70;
  CAtlWinModule local_6c [40];
  uint local_44;
  undefined4 local_40;
  undefined4 local_3c;
  CAsyncMonikerFile local_38 [40];
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c77bfd;
  local_10 = ExceptionList;
  bVar9 = false;
  bVar8 = false;
  bVar7 = false;
  bVar6 = false;
  bVar5 = false;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  ExceptionList = &local_10;
  _Get_CMainGameComponent__SIPAV1_XZ();
  _GetPlayerManager_CMainGameComponent__QAEAAVCPlayerManager__XZ();
  _GetMainPlayer_CPlayerManager__QAEAAVCPlayer__XZ();
  local_40 = _GetPControlledCreature_CPlayer__QBEPAVCThingPlayerCreature__XZ();
  cVar10 = ___IS_THING_ALIVE_PAVCThingPlayerCreature____YI_NABQAVCThingPlayerCreature___Z();
  if (cVar10 != '\0') {
    _SetInLimbo_CThing__QAEX_N_Z(1);
  }
  local_3c = _GetLoadedRegion_CWorldMap__QBEJXZ();
  _UnloadRegion_CWorldMap__IAEXJ_N_Z(local_3c,0);
  __0CCharString__QAE_XZ();
  local_8 = 0;
  local_44 = 1;
  do {
    uVar11 = _size___vector_VCRegion__V__allocator_VCRegion___std___std__QBEIXZ();
    if (uVar11 <= local_44) {
      __0CWideString__QAE_PB_W_Z(L"RegionGraph.txt");
      local_8._0_1_ = 0x12;
      __0CDiskFileWin32__QAE_ABVCWideString__W4FileAccessType__K_Z(local_140,4,2);
      local_8 = CONCAT31(local_8._1_3_,0x14);
      __1CWideString__QAE_XZ();
      _SaveToFile_CCharString__QBEXAAVCAFile___Z(local_38);
      _LoadRegion_CWorldMap__QAEXJW4ELoadType_1__N_Z(local_3c,0,1);
      cVar10 = ___IS_THING_ALIVE_PAVCThingPlayerCreature____YI_NABQAVCThingPlayerCreature___Z();
      if (cVar10 != '\0') {
        _SetInLimbo_CThing__QAEX_N_Z(0);
      }
      local_8 = local_8 & 0xffffff00;
      CAsyncMonikerFile::~CAsyncMonikerFile(local_38);
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      ExceptionList = local_10;
      return;
    }
    _LoadRegion_CWorldMap__QAEXJW4ELoadType_1__N_Z(local_44,0,0);
    __0__vector_PAVCThing__V__allocator_PAVCThing___std___std__QAE_XZ();
    local_8._0_1_ = 1;
    pCVar18 = local_6c;
    uVar12 = __0CThingFilter_HasTC__QAE_W4ETCInterfaceType___Z(100);
    _GetThingSearchTools_CWorld__QAEAAVCThingSearchTools__XZ(uVar12,pCVar18);
    ___GetAllThings_VCThing__V__vector_PAVCThing__V__allocator_PAVCThing___std___std__VCThingFilter_HasTC___CThingSearchTools__QBEXABVCThingFilter_HasTC__AAV__vector_PAVCThing__V__allocator_PAVCThing___std___std___Z
              (uVar12,pCVar18);
    __0__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__QAE_XZ();
    local_8._0_1_ = 2;
    local_70 = 0;
    while (uVar11 = _size___vector_PAVCThing__V__allocator_PAVCThing___std___std__QBEIXZ(),
          local_70 < uVar11) {
      __A__vector_PAVCThing__V__allocator_PAVCThing___std___std__QAEAAPAVCThing__I_Z(local_70);
      uVar12 = _GetCurrentMapNumber_CThing__QBEJXZ();
      uVar11 = _GetRegionNumberMapIsIn_CWorldMap__QBEJJ_Z(uVar12);
      if (uVar11 == local_44) {
        local_78 = 0;
        puVar19 = &local_78;
        __A__vector_PAVCThing__V__allocator_PAVCThing___std___std__QAEAAPAVCThing__I_Z(local_70);
        cVar10 = ___PeekTC___CBVCTCDRegionExit___CThing__QBE_NPAPBVCTCDRegionExit___Z(puVar19);
        if (cVar10 == '\0') {
          if (DAT_04bac270 == 0) {
LAB_01c8a55e:
            local_15c = '\x01';
          }
          else {
            __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
            local_8 = CONCAT31(local_8._1_3_,3);
            __0CCharString__QAE_PBDJ_Z(&DAT_040b7902,0xffffffff);
            local_8 = 4;
            uVar15 = 1;
            uVar14 = 0x95c;
            puVar17 = local_ac;
            pcVar16 = "region_exits[c1]->PeekTC(&ptc_region_exit)";
            __0CCharString__QAE_PBDJ_Z("Failed success assert on ",0xffffffff);
            local_8 = 5;
            uVar12 = __H_YI_AVCCharString__ABV0_PBD_Z(pcVar16);
            local_8 = 6;
            bVar5 = true;
            bVar4 = true;
            bVar3 = true;
            bVar2 = true;
            cVar10 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                               (local_c4,uVar12,puVar17,uVar14,uVar15);
            if (cVar10 == '\x01') goto LAB_01c8a55e;
            local_15c = '\0';
          }
          local_a1 = local_15c;
          local_8 = 5;
          if (bVar3) {
            bVar3 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 4;
          if (bVar4) {
            bVar4 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 3;
          if (bVar5) {
            bVar5 = false;
            __1CCharString__QAE_XZ();
          }
          local_8._0_1_ = 2;
          local_8._1_3_ = 0;
          if (bVar2) {
            bVar2 = false;
            __1CWideString__QAE_XZ();
          }
          if ((local_a1 != '\0') && (BVar13 = IsDebuggerPresent(), BVar13 != 0)) {
            pcVar1 = (code *)swi(3);
            (*pcVar1)();
            return;
          }
        }
        local_74 = _GetNameOfRegionConnectingTo_CTCDRegionExit__QBEABVCCharString__XZ();
        cVar10 = __9CCharString__QBE_NPBD_Z(&DAT_040b7903);
        if (cVar10 != '\0') {
          _insert____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AU__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_2_ABVCCharString___Z
                    (local_d4,local_74);
          __1__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_std__QAE_XZ
                    ();
        }
      }
      local_70 = local_70 + 1;
    }
    _clear___vector_PAVCThing__V__allocator_PAVCThing___std___std__QAEXXZ();
    pCVar18 = local_6c;
    uVar12 = __0__CThingFilter_HasTCOfLeafType_VCTCActionUseScriptedHook____QAE_XZ(pCVar18);
    _GetThingSearchTools_CWorld__QAEAAVCThingSearchTools__XZ(uVar12);
    ___GetAllThings_VCThing__V__vector_PAVCThing__V__allocator_PAVCThing___std___std__V__CThingFilter_HasTCOfLeafType_VCTCActionUseScriptedHook_____CThingSearchTools__QBEXABV__CThingFilter_HasTCOfLeafType_VCTCActionUseScriptedHook____AAV__vector_PAVCThing__V__allocator_PAVCThing___std___std___Z
              (uVar12,pCVar18);
    local_7c = 0;
    while (uVar11 = _size___vector_PAVCThing__V__allocator_PAVCThing___std___std__QBEIXZ(),
          local_7c < uVar11) {
      __A__vector_PAVCThing__V__allocator_PAVCThing___std___std__QAEAAPAVCThing__I_Z(local_7c);
      uVar12 = _GetCurrentMapNumber_CThing__QBEJXZ();
      uVar11 = _GetRegionNumberMapIsIn_CWorldMap__QBEJJ_Z(uVar12);
      if (uVar11 == local_44) {
        pp_Var20 = &local_80;
        __A__vector_PAVCThing__V__allocator_PAVCThing___std___std__QAEAAPAVCThing__I_Z(local_7c);
        cVar10 = ___PeekTC___CBVCTCActionUseScriptedHook___CThing__QBE_NPAPBVCTCActionUseScriptedHook___Z
                           (pp_Var20);
        if (cVar10 == '\0') {
          if (DAT_04bac270 == 0) {
LAB_01c8a80d:
            local_170 = '\x01';
          }
          else {
            __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
            local_8 = CONCAT31(local_8._1_3_,7);
            __0CCharString__QAE_PBDJ_Z(&DAT_040b7921,0xffffffff);
            local_8 = 8;
            uVar15 = 1;
            uVar14 = 0x96b;
            puVar17 = local_e0;
            pcVar16 = "region_exits[c1]->PeekTC(&ptc_scripted_hook)";
            __0CCharString__QAE_PBDJ_Z("Failed success assert on ",0xffffffff);
            local_8 = 9;
            uVar12 = __H_YI_AVCCharString__ABV0_PBD_Z(pcVar16);
            local_8 = 10;
            bVar9 = true;
            bVar8 = true;
            bVar7 = true;
            bVar6 = true;
            cVar10 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                               (local_f8,uVar12,puVar17,uVar14,uVar15);
            if (cVar10 == '\x01') goto LAB_01c8a80d;
            local_170 = '\0';
          }
          local_d6 = local_170;
          local_8 = 9;
          if (bVar6) {
            bVar6 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 8;
          if (bVar7) {
            bVar7 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 7;
          if (bVar8) {
            bVar8 = false;
            __1CCharString__QAE_XZ();
          }
          local_8._0_1_ = 2;
          local_8._1_3_ = 0;
          if (bVar9) {
            bVar9 = false;
            __1CWideString__QAE_XZ();
          }
          if ((local_d6 != '\0') && (BVar13 = IsDebuggerPresent(), BVar13 != 0)) {
            pcVar1 = (code *)swi(3);
            (*pcVar1)();
            return;
          }
        }
        ATL::_ATL_WIN_MODULE70::~_ATL_WIN_MODULE70(local_80);
        cVar10 = ___IS_THING_ALIVE_PAVCThing____YI_NABQAVCThing___Z();
        if (cVar10 != '\0') {
          uVar12 = _GetPos_CThing__QBEABVC3DVector__XZ();
          uVar12 = (**(code **)(*param_1 + 0x40))(uVar12);
          uVar12 = _GetRegionNumberMapIsIn_CWorldMap__QBEJJ_Z(uVar12);
          _GetRegion_CWorldMap__QAEAAVCRegion__J_Z(uVar12);
          uVar12 = _GetName_CRegion__QBEABVCCharString__XZ();
          _insert____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AU__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_2_ABVCCharString___Z
                    (local_108,uVar12);
          __1__pair_V___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std___N_std__QAE_XZ
                    ();
        }
      }
      local_7c = local_7c + 1;
    }
    cVar10 = _empty____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QBE_NXZ
                       ();
    if (cVar10 == '\0') {
      puVar21 = &DAT_040b9090;
      _GetRegion_CWorldMap__QAEAAVCRegion__J_Z(local_44);
      uVar12 = _GetName_CRegion__QBEABVCCharString__XZ(puVar21);
      __H_YI_AVCCharString__PBDABV0__Z(uVar12);
      local_8._0_1_ = 0xb;
      uVar12 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar21);
      local_8._0_1_ = 0xc;
      __YCCharString__QAEABV0_ABV0__Z(uVar12);
      local_8._0_1_ = 0xb;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 2;
      __1CCharString__QAE_XZ();
      _end____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AV___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___2_XZ
                (local_90);
      local_8._0_1_ = 0xd;
      __F___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QAEAAV01_XZ
                ();
      _begin____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AV___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___2_XZ
                (local_9c);
      local_8._0_1_ = 0xe;
      while( true ) {
        uVar12 = _end____Tree_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std__QAE_AV___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___2_XZ
                           (local_128);
        local_8._0_1_ = 0xf;
        local_119 = __9___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QBE_NABV01__Z
                              (uVar12);
        local_8._0_1_ = 0xe;
        __1___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QAE_XZ
                  ();
        if (local_119 == '\0') break;
        puVar21 = &DAT_040b9098;
        uVar12 = __D___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QBEABVCCharString__XZ
                           (&DAT_040b9098);
        uVar14 = __H_YI_AVCCharString__PBDABV0__Z(uVar12);
        local_8._0_1_ = 0x10;
        uVar15 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar21);
        local_8._0_1_ = 0x11;
        uVar12 = uVar15;
        __YCCharString__QAEABV0_ABV0__Z(uVar15);
        local_8._0_1_ = 0x10;
        __1CCharString__QAE_XZ(uVar15,uVar12,uVar14);
        local_8._0_1_ = 0xe;
        __1CCharString__QAE_XZ();
        cVar10 = __9___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QBE_NABV01__Z
                           (local_90);
        if (cVar10 != '\0') {
          __YCCharString__QAEABV0_PBD_Z(&DAT_040b90a0);
        }
        __E___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QAEAAV01_XZ
                  ();
      }
      local_8._0_1_ = 0xd;
      __1___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QAE_XZ
                ();
      __YCCharString__QAEABV0_PBD_Z(&DAT_040b90a4);
      local_8._0_1_ = 2;
      __1___Tree_const_iterator_V___Tree_val_V___Tset_traits_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__0A__std___std___std__QAE_XZ
                ();
    }
    _UnloadRegion_CWorldMap__IAEXJ_N_Z(local_44,0);
    local_8._0_1_ = 1;
    __1__set_VCCharString__U__less_VCCharString___std__V__allocator_VCCharString___3__std__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    ATL::CAtlWinModule::~CAtlWinModule(local_6c);
    local_44 = local_44 + 1;
  } while( true );
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?LoadRegionGraph@CWorldMap@@QAEXABVCWideString@@@Z @ 01c8aec0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void __thiscall
_LoadRegionGraph_CWorldMap__QAEXABVCWideString___Z(undefined4 param_1,undefined4 param_2)

{
  code *pcVar1;
  char cVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  BOOL BVar5;
  undefined1 *puVar6;
  undefined *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 local_8c;
  undefined1 local_7c [24];
  undefined1 local_64 [8];
  undefined1 local_5c [11];
  char local_51;
  void *local_50;
  undefined4 local_4c;
  CAsyncMonikerFile local_40 [32];
  undefined1 local_20 [8];
  undefined1 local_18 [8];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c77cc3;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _GetMiscDir_NResourceDirectoryNames__YI_AVCWideString__XZ(param_2);
  local_8 = 0;
  __H_YI_AVCWideString__ABV0_0_Z(param_2);
  local_8._0_1_ = 2;
  __1CWideString__QAE_XZ();
  cVar2 = _PathExists_CAFile__SI_NABVCWideString___Z();
  if (cVar2 == '\0') {
    __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
    local_8._0_1_ = 6;
    __0CCharString__QAE_PBDJ_Z(&DAT_040b7922,0xffffffff);
    local_8._0_1_ = 7;
    uVar10 = 2;
    uVar9 = 0x9a0;
    puVar8 = local_5c;
    puVar7 = &DAT_040b90e4;
    uVar3 = _ToCharString_CWideString__QBE_AVCCharString__XZ(local_64);
    local_8._0_1_ = 8;
    uVar3 = __H_YI_AVCCharString__PBDABV0__Z(uVar3);
    local_8._0_1_ = 9;
    uVar4 = __H_YI_AVCCharString__ABV0_PBD_Z(puVar7);
    local_8._0_1_ = 10;
    puVar6 = local_7c;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,uVar4,puVar8,uVar9,uVar10,uVar4,uVar4,uVar3)
    ;
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar6,uVar4,puVar8,uVar9,uVar10);
    local_51 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 9;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 8;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 7;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 6;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,2);
    __1CWideString__QAE_XZ();
    if (local_51 != '\0') {
      BVar5 = IsDebuggerPresent();
      if (BVar5 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  else {
    __0CDiskFileWin32__QAE_ABVCWideString__W4FileAccessType__K_Z(local_18,1,2);
    local_8._0_1_ = 3;
    cVar2 = _IsNull___CCountedPointer_VCRegionConnectionGraph____QBE_NXZ();
    if (cVar2 != '\0') {
      local_50 = operator_new(0x18);
      local_8._0_1_ = 4;
      if (local_50 == (void *)0x0) {
        local_8c = 0;
      }
      else {
        local_8c = __0CRegionConnectionGraph__QAE_XZ();
      }
      local_4c = local_8c;
      local_8._0_1_ = 3;
      _Reset___CCountedPointer_VCRegionConnectionGraph____QAEXPAVCRegionConnectionGraph___Z
                (local_8c);
    }
    __0CCharString__QAE_XZ();
    local_8._0_1_ = 5;
    _LoadFromFile_CCharString__QAEXAAVCAFile__PAVCASuspendableProcess__J_Z(local_40,0,0xffffffff);
    puVar8 = local_20;
    __C__CCountedPointer_VCRegionConnectionGraph____QBEPAVCRegionConnectionGraph__XZ(puVar8,param_1)
    ;
    _InitialiseGraph_CRegionConnectionGraph__QAEXABVCCharString__ABVCWorldMap___Z(puVar8,param_1);
    local_8._0_1_ = 3;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,2);
    CAsyncMonikerFile::~CAsyncMonikerFile(local_40);
  }
  local_8 = 0xffffffff;
  __1CWideString__QAE_XZ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?InitialiseGraph@CRegionConnectionGraph@@QAEXABVCCharString@@ABVCWorldMap@@@Z @ 027dabf0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _InitialiseGraph_CRegionConnectionGraph__QAEXABVCCharString__ABVCWorldMap___Z
               (undefined4 param_1)

{
  code *pcVar1;
  bool bVar2;
  char cVar3;
  undefined4 uVar4;
  BOOL BVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined *puVar10;
  CAtlWinModule *pCVar11;
  undefined4 local_11c;
  char local_118;
  undefined1 local_fc [8];
  undefined1 local_f4 [8];
  undefined1 local_ec [8];
  undefined1 local_e4 [11];
  char local_d9;
  undefined1 local_d8 [11];
  char local_cd;
  undefined1 local_cc [8];
  undefined1 local_c4 [11];
  char local_b9;
  undefined1 local_b8 [11];
  char local_ad;
  void *local_ac;
  undefined4 local_a8;
  undefined1 local_9c [16];
  undefined1 local_8c [11];
  char local_81;
  int local_78;
  undefined1 local_74 [8];
  CAtlWinModule local_6c [20];
  undefined1 local_58 [8];
  int local_50 [16];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03dbe091;
  local_10 = ExceptionList;
  bVar2 = false;
  ExceptionList = &local_10;
  _clear___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAEXXZ
            ();
  _reserve___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAEXI_Z
            (0x50);
  uVar9 = 0;
  uVar4 = __0CCharString__QAE_PBDJ_Z(&DAT_042eee04,0xffffffff);
  local_8 = 0;
  __0CStringParser__QAE_ABVCCharString__P6IX0_Z0J_Z(param_1,0,uVar4,uVar9);
  local_8 = CONCAT31(local_8._1_3_,2);
  __1CCharString__QAE_XZ();
  do {
    cVar3 = _AtEnd_CStringParser__QAE_NXZ();
    if (cVar3 == '\0') {
      puVar10 = &DAT_042eee08;
      _ReadAsSymbol_CStringParser__QAE_AVCCharString__XZ(local_8c);
      local_8 = CONCAT31(local_8._1_3_,3);
      bVar2 = true;
      cVar3 = __8CCharString__QBE_NPBD_Z(puVar10);
      if (cVar3 == '\0') goto LAB_027dace7;
      local_118 = '\x01';
    }
    else {
LAB_027dace7:
      local_118 = '\0';
    }
    local_81 = local_118;
    local_8 = 2;
    if (bVar2) {
      bVar2 = false;
      __1CCharString__QAE_XZ();
    }
    if (local_81 == '\0') {
      _Update___CVectorMap_JV__CCountedPointer_VCRegionConnectionNode____V__CKeyPairCompareLess_JV__CCountedPointer_VCRegionConnectionNode________QAEXXZ
                ();
      local_8 = 0xffffffff;
      __1CStringParser__UAE_XZ();
      ExceptionList = local_10;
      return;
    }
    __0CCharString__QAE_PBDJ_Z("Initialise Region Graph",0xffffffff);
    local_8._0_1_ = 4;
    _DisplayProgress_NProgressDisplay__YIXABVCCharString__M_N1_Z(_DAT_0401de00,0);
    local_8._0_1_ = 2;
    __1CCharString__QAE_XZ();
    __0CCharString__QAE_PBDJ_Z(&DAT_042eee24,0xffffffff);
    local_8._0_1_ = 5;
    _ReadAsStringUntilString_CStringParser__QAE_AVCCharString__ABV2__Z(local_58,local_9c);
    local_8._0_1_ = 7;
    __1CCharString__QAE_XZ();
    local_50[0] = _GetRegionNumberFromName_CWorldMap__QBEJABVCCharString___Z(local_58);
    if (0 < local_50[0]) {
      local_ac = operator_new(0x20);
      local_8._0_1_ = 8;
      if (local_ac == (void *)0x0) {
        local_11c = 0;
      }
      else {
        local_11c = __0CRegionConnectionNode__QAE_J_Z(local_50[0]);
      }
      local_a8 = local_11c;
      local_8._0_1_ = 7;
      uVar4 = __0__CCountedPointer_VCRegionConnectionNode____QAE_PAVCRegionConnectionNode___Z
                        (local_11c);
      local_8._0_1_ = 9;
      _Insert___CVectorMap_JV__CCountedPointer_VCRegionConnectionNode____V__CKeyPairCompareLess_JV__CCountedPointer_VCRegionConnectionNode________QAEXABJABV__CCountedPointer_VCRegionConnectionNode_____Z
                (local_50,uVar4);
      local_8._0_1_ = 7;
      __1__CCountedPointer_VCRegionConnectionNode____QAE_XZ();
      __0__vector_JV__allocator_J_std___std__QAE_XZ();
      local_8._0_1_ = 10;
      __0CCharString__QAE_PBDJ_Z(&DAT_042eee28,0xffffffff);
      local_8._0_1_ = 0xb;
      local_ad = _SkipPastString_CStringParser__QAE_NABVCCharString___Z(local_b8);
      local_8._0_1_ = 10;
      __1CCharString__QAE_XZ();
      if (local_ad != '\0') {
        while( true ) {
          __0CCharString__QAE_PBDJ_Z(&DAT_042eee2c,0xffffffff);
          local_8._0_1_ = 0xc;
          local_b9 = _SkipPastNextItemIfIs_CStringParser__QAE_NABVCCharString___Z(local_c4);
          local_8._0_1_ = 10;
          __1CCharString__QAE_XZ();
          if (local_b9 == '\0') break;
          __0CCharString__QAE_PBDJ_Z(&DAT_042eee30,0xffffffff);
          local_8._0_1_ = 0xd;
          _ReadAsStringUntilString_CStringParser__QAE_AVCCharString__ABV2__Z(local_74,local_cc);
          local_8 = CONCAT31(local_8._1_3_,0xf);
          __1CCharString__QAE_XZ();
          local_78 = _GetRegionNumberFromName_CWorldMap__QBEJABVCCharString___Z(local_74);
          if (0 < local_78) {
            _push_back___vector_JV__allocator_J_std___std__QAEXABJ_Z(&local_78);
          }
          __0CCharString__QAE_PBDJ_Z(&DAT_042eee34,0xffffffff);
          local_8._0_1_ = 0x10;
          cVar3 = _SkipPastString_CStringParser__QAE_NABVCCharString___Z(local_d8);
          local_cd = '\x01' - (cVar3 != '\0');
          local_8._0_1_ = 0xf;
          __1CCharString__QAE_XZ();
          if (local_cd != '\0') {
            __0CWideString__QAE_PB_W_Z(L"region_connection_graph.cpp");
            local_8._0_1_ = 0x11;
            __0CCharString__QAE_PBDJ_Z("Corrupt graph data!",0xffffffff);
            local_8._0_1_ = 0x12;
            __0CCharString__QAE_PBDJ_Z(&DAT_042eee05,0xffffffff);
            local_8._0_1_ = 0x13;
            uVar9 = 2;
            uVar4 = 0x86;
            puVar8 = local_e4;
            puVar7 = local_ec;
            puVar6 = local_f4;
            _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0x86,2);
            _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
            cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                              (puVar6,puVar7,puVar8,uVar4,uVar9);
            local_d9 = '\x01' - (cVar3 != '\x01');
            local_8._0_1_ = 0x12;
            __1CCharString__QAE_XZ();
            local_8._0_1_ = 0x11;
            __1CCharString__QAE_XZ();
            local_8 = CONCAT31(local_8._1_3_,0xf);
            __1CWideString__QAE_XZ();
            if ((local_d9 != '\0') && (BVar5 = IsDebuggerPresent(), BVar5 != 0)) {
              pcVar1 = (code *)swi(3);
              (*pcVar1)();
              return;
            }
            _clear___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAEXXZ
                      ();
            local_8._0_1_ = 10;
            __1CCharString__QAE_XZ();
            local_8._0_1_ = 7;
            ATL::CAtlWinModule::~CAtlWinModule(local_6c);
            local_8 = CONCAT31(local_8._1_3_,2);
            __1CCharString__QAE_XZ();
            local_8 = 0xffffffff;
            __1CStringParser__UAE_XZ();
            ExceptionList = local_10;
            return;
          }
          __0CCharString__QAE_PBDJ_Z(&DAT_042eee84,0xffffffff);
          local_8._0_1_ = 0x14;
          _SkipPastNextItemIfIs_CStringParser__QAE_NABVCCharString___Z(local_fc);
          local_8._0_1_ = 0xf;
          __1CCharString__QAE_XZ();
          local_8._0_1_ = 10;
          __1CCharString__QAE_XZ();
        }
        pCVar11 = local_6c;
        _back___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAEAAU__pair_JV__CCountedPointer_VCRegionConnectionNode_____2_XZ
                  (pCVar11);
        __C__CCountedPointer_VCRegionConnectionNode____QBEPAVCRegionConnectionNode__XZ();
        _InitialiseConnections_CRegionConnectionNode__QAEXABV__vector_JV__allocator_J_std___std___Z
                  (pCVar11);
      }
      local_8._0_1_ = 7;
      ATL::CAtlWinModule::~CAtlWinModule(local_6c);
    }
    _SkipPastNewline_CStringParser__QAEJXZ();
    local_8 = CONCAT31(local_8._1_3_,2);
    __1CCharString__QAE_XZ();
  } while( true );
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ??0CRegionConnectionNode@@QAE@J@Z @ 027da620 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

undefined4 * __thiscall __0CRegionConnectionNode__QAE_J_Z(undefined4 *param_1,undefined4 param_2)

{
  *param_1 = param_2;
  __0__vector_JV__allocator_J_std___std__QAE_XZ();
  param_1[6] = 0;
  param_1[7] = 0;
  return param_1;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SetAsConnectedToRegion@CRegionConnectionNode@@QAEXJ_N@Z @ 027da930 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _SetAsConnectedToRegion_CRegionConnectionNode__QAEXJ_N_Z(undefined4 param_1,char param_2)

{
  undefined4 uStack_e4;
  undefined1 *puStack_e0;
  undefined1 *puStack_dc;
  undefined4 uStack_d8;
  undefined1 *puStack_d4;
  undefined4 uStack_d0;
  undefined1 auStack_cc [4];
  undefined4 uStack_c8;
  undefined1 *puStack_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined1 *local_b8;
  undefined1 *local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined1 *local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  char local_8d;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined1 *local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined1 local_68 [24];
  undefined1 *local_50;
  undefined1 *local_4c;
  undefined1 *local_48;
  undefined1 local_44 [12];
  undefined1 *local_38;
  undefined1 local_34 [24];
  undefined1 *local_1c;
  undefined1 *local_18;
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03dbdf98;
  local_10 = ExceptionList;
  if (param_2 == '\0') {
    puStack_c4 = local_44;
    uStack_c8 = 0x27daa43;
    ExceptionList = &local_10;
    uStack_d0 = _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                          ();
    local_8 = 3;
    local_38 = auStack_cc;
    puStack_d4 = (undefined1 *)0x27daa70;
    local_98 = uStack_d0;
    local_94 = uStack_d0;
    local_a0 = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<unsigned_short>_>_>
                         ();
    local_8._0_1_ = 4;
    local_a4 = (undefined1 *)&param_1;
    local_4c = (undefined1 *)&uStack_d8;
    puStack_dc = (undefined1 *)&uStack_d8;
    puStack_e0 = (undefined1 *)0x27daaa3;
    local_9c = local_a0;
    local_ac = _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                         ();
    local_8._0_1_ = 5;
    local_50 = (undefined1 *)&uStack_e4;
    local_a8 = local_ac;
    local_b0 = _begin___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                         (&uStack_e4);
    local_8._0_1_ = 4;
    puStack_dc = (undefined1 *)
                 ___find_V___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__J_std__YI_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___0_V10_0ABJ_Z
                           ();
    local_8._0_1_ = 6;
    local_48 = (undefined1 *)&uStack_d8;
    puStack_e0 = (undefined1 *)0x27dab0f;
    local_b8 = puStack_dc;
    local_b4 = puStack_dc;
    local_bc = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<unsigned_short>_>_>
                         ();
    puStack_dc = local_68;
    local_8._0_1_ = 7;
    puStack_e0 = (undefined1 *)0x27dab28;
    local_c0 = _erase___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_V___Vector_const_iterator_V___Vector_val_JV__allocator_J_std___std___2_0_Z
                         ();
    puStack_c4 = (undefined1 *)0x27dab36;
    __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,3);
    puStack_c4 = (undefined1 *)0x27dab42;
    __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
    local_8 = 0xffffffff;
    puStack_c4 = (undefined1 *)0x27dab51;
    __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
  }
  else {
    puStack_c4 = local_34;
    uStack_c8 = 0x27da96c;
    ExceptionList = &local_10;
    puStack_c4 = (undefined1 *)
                 _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                           ();
    local_8 = 0;
    local_78 = (undefined1 *)&param_1;
    local_18 = (undefined1 *)&uStack_d0;
    puStack_d4 = (undefined1 *)&uStack_d0;
    uStack_d8 = 0x27da99a;
    local_74 = puStack_c4;
    local_70 = puStack_c4;
    local_80 = _end___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                         ();
    local_8._0_1_ = 1;
    local_1c = (undefined1 *)&puStack_dc;
    puStack_e0 = (undefined1 *)&puStack_dc;
    uStack_e4 = 0x27da9bb;
    local_7c = local_80;
    local_84 = _begin___vector_JV__allocator_J_std___std__QAE_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___2_XZ
                         ();
    local_8._0_1_ = 0;
    puStack_e0 = (undefined1 *)0x27da9cd;
    local_8c = ___find_V___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__J_std__YI_AV___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___0_V10_0ABJ_Z
                         ();
    local_8._0_1_ = 2;
    uStack_c8 = 0x27da9ee;
    local_88 = local_8c;
    local_8d = FID_conflict_operator__();
    local_8 = (uint)local_8._1_3_ << 8;
    puStack_c4 = (undefined1 *)0x27daa09;
    local_11 = local_8d;
    __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
    local_8 = 0xffffffff;
    puStack_c4 = (undefined1 *)0x27daa18;
    __1___Vector_iterator_V___Vector_val_JV__allocator_J_std___std___std__QAE_XZ();
    if (local_11 != '\0') {
      puStack_c4 = (undefined1 *)&param_1;
      uStack_c8 = 0x27daa2f;
      _push_back___vector_JV__allocator_J_std___std__QAEXABJ_Z();
    }
  }
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SetRegionsAsConnected@CRegionConnectionGraph@@QAEXJJ_N@Z @ 027db600 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _SetRegionsAsConnected_CRegionConnectionGraph__QAEXJJ_N_Z
               (undefined4 param_1,undefined4 param_2,byte param_3)

{
  code *pcVar1;
  char cVar2;
  undefined4 uVar3;
  BOOL BVar4;
  uint uVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined1 local_80 [8];
  undefined1 local_78 [8];
  undefined1 local_70 [11];
  char local_65;
  undefined1 local_64 [15];
  char local_55;
  undefined1 local_54 [8];
  undefined1 local_4c [8];
  undefined1 local_44 [11];
  char local_39;
  undefined1 local_38 [15];
  char local_29;
  undefined1 local_28 [12];
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03dbe190;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Find___CVectorMap_JV__CCountedPointer_VCRegionConnectionNode____V__CKeyPairCompareLess_JV__CCountedPointer_VCRegionConnectionNode________QAE_AV___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__ABJ_Z
            (local_1c,&param_1);
  local_8 = 0;
  uVar3 = _end___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAE_AV___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___2_XZ
                    (local_38);
  local_8._0_1_ = 1;
  local_29 = FID_conflict_operator__(uVar3);
  local_8._0_1_ = 0;
  __1___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
            ();
  if (local_29 != '\0') {
    __C___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPAU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              ();
    cVar2 = _IsNull___CCountedPointer_VCRegionConnectionNode____QBE_NXZ();
    if (cVar2 != '\0') {
      __0CWideString__QAE_PB_W_Z(L"region_connection_graph.cpp");
      local_8._0_1_ = 2;
      __0CCharString__QAE_PBDJ_Z(&DAT_042eee07,0xffffffff);
      local_8._0_1_ = 3;
      __0CCharString__QAE_PBDJ_Z("NOT region1_it->second.IsNull()",0xffffffff);
      local_8._0_1_ = 4;
      uVar9 = 1;
      uVar3 = 0xb1;
      puVar8 = local_44;
      puVar7 = local_4c;
      puVar6 = local_54;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0xb1,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar6,puVar7,puVar8,uVar3,uVar9);
      local_39 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 3;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 2;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 0;
      __1CWideString__QAE_XZ();
      if (local_39 != '\0') {
        BVar4 = IsDebuggerPresent();
        if (BVar4 != 0) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
    }
    uVar5 = (uint)param_3;
    uVar3 = param_2;
    __C___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPAU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              (param_2,uVar5);
    __C__CCountedPointer_VCRegionConnectionNode____QBEPAVCRegionConnectionNode__XZ();
    _SetAsConnectedToRegion_CRegionConnectionNode__QAEXJ_N_Z(uVar3,uVar5);
  }
  _Find___CVectorMap_JV__CCountedPointer_VCRegionConnectionNode____V__CKeyPairCompareLess_JV__CCountedPointer_VCRegionConnectionNode________QAE_AV___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__ABJ_Z
            (local_28,&param_2);
  local_8._0_1_ = 5;
  uVar3 = _end___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QAE_AV___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___2_XZ
                    (local_64);
  local_8._0_1_ = 6;
  local_55 = FID_conflict_operator__(uVar3);
  local_8._0_1_ = 5;
  __1___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
            ();
  if (local_55 != '\0') {
    __C___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPAU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              ();
    cVar2 = _IsNull___CCountedPointer_VCRegionConnectionNode____QBE_NXZ();
    if (cVar2 != '\0') {
      __0CWideString__QAE_PB_W_Z(L"region_connection_graph.cpp");
      local_8._0_1_ = 7;
      __0CCharString__QAE_PBDJ_Z(&DAT_042eee0a,0xffffffff);
      local_8._0_1_ = 8;
      __0CCharString__QAE_PBDJ_Z("NOT region2_it->second.IsNull()",0xffffffff);
      local_8._0_1_ = 9;
      uVar9 = 1;
      uVar3 = 0xb7;
      puVar8 = local_70;
      puVar7 = local_78;
      puVar6 = local_80;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar7,puVar8,0xb7,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar6,puVar7,puVar8,uVar3,uVar9);
      local_65 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 8;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 7;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 5;
      __1CWideString__QAE_XZ();
      if (local_65 != '\0') {
        BVar4 = IsDebuggerPresent();
        if (BVar4 != 0) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
    }
    uVar5 = (uint)param_3;
    uVar3 = param_1;
    __C___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPAU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              (param_1,uVar5);
    __C__CCountedPointer_VCRegionConnectionNode____QBEPAVCRegionConnectionNode__XZ();
    _SetAsConnectedToRegion_CRegionConnectionNode__QAEXJ_N_Z(uVar3,uVar5);
  }
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  __1___Vector_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
            ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?GetNextRegionOnWayToRegion@CRegionConnectionGraph@@QBEJJJ@Z @ 027db2c0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

undefined4 __thiscall
_GetNextRegionOnWayToRegion_CRegionConnectionGraph__QBEJJJ_Z
          (undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  code *pcVar1;
  char cVar2;
  undefined4 uVar3;
  BOOL BVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined1 local_48 [8];
  undefined1 local_40 [8];
  undefined1 local_38 [11];
  char local_2d;
  undefined1 local_2c [15];
  char local_1d;
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03dbe108;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Find___CVectorMap_JV__CCountedPointer_VCRegionConnectionNode____V__CKeyPairCompareLess_JV__CCountedPointer_VCRegionConnectionNode________QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__ABJ_Z
            (local_1c,&param_2);
  local_8 = 0;
  uVar3 = _end___vector_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std__QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___2_XZ
                    (local_2c);
  local_8._0_1_ = 1;
  local_1d = FID_conflict_operator__(uVar3);
  local_8._0_1_ = 0;
  __1___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
            ();
  if (local_1d == '\0') {
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
              ();
    uVar3 = 0xffffffff;
  }
  else {
    __C___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPBU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              ();
    cVar2 = _IsNull___CCountedPointer_VCRegionConnectionNode____QBE_NXZ();
    if (cVar2 != '\0') {
      __0CWideString__QAE_PB_W_Z(L"region_connection_graph.cpp");
      local_8._0_1_ = 2;
      __0CCharString__QAE_PBDJ_Z(&DAT_042eee06,0xffffffff);
      local_8._0_1_ = 3;
      __0CCharString__QAE_PBDJ_Z("NOT region_it->second.IsNull()",0xffffffff);
      local_8._0_1_ = 4;
      uVar8 = 1;
      uVar3 = 0x9a;
      puVar7 = local_38;
      puVar6 = local_40;
      puVar5 = local_48;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar5,puVar6,puVar7,0x9a,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar5,puVar6,puVar7,uVar3,uVar8);
      local_2d = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 3;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 2;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 0;
      __1CWideString__QAE_XZ();
      if (local_2d != '\0') {
        BVar4 = IsDebuggerPresent();
        if (BVar4 != 0) {
          pcVar1 = (code *)swi(3);
          uVar3 = (*pcVar1)();
          return uVar3;
        }
      }
    }
    uVar3 = param_3;
    __C___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QBEPBU__pair_JV__CCountedPointer_VCRegionConnectionNode_____1_XZ
              (param_3,param_1);
    __C__CCountedPointer_VCRegionConnectionNode____QBEPAVCRegionConnectionNode__XZ();
    uVar3 = _GetNextNodeOnWayToRegion_CRegionConnectionNode__QBEJJABVCRegionConnectionGraph___Z
                      (uVar3,param_1);
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std__V__allocator_U__pair_JV__CCountedPointer_VCRegionConnectionNode_____std___2__std___std__QAE_XZ
              ();
  }
  ExceptionList = local_10;
  return uVar3;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?GetNextNodeOnWayToRegion@CRegionConnectionNode@@QBEJJABVCRegionConnectionGraph@@@Z @ 027da6a0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x027da708) */
/* WARNING: Removing unreachable block (ram,0x027da7c5) */
/* [ported from ego_r via strfp] */

int __thiscall
_GetNextNodeOnWayToRegion_CRegionConnectionNode__QBEJJABVCRegionConnectionGraph___Z
          (int *param_1,int param_2)

{
  int iVar1;
  char cVar2;
  int *piVar3;
  uint uVar4;
  undefined4 *puVar5;
  int *local_34;
  uint local_30;
  int local_2c;
  int *local_28;
  uint local_24;
  CAtlWinModule local_20 [16];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03dbdf48;
  local_10 = ExceptionList;
  if (*param_1 != param_2) {
    DAT_04a55e14 = DAT_04a55e14 + 1;
    ExceptionList = &local_10;
    __0__list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QAE_XZ();
    local_8 = 0;
    param_1[6] = DAT_04a55e14;
    param_1[7] = 0;
    local_24 = 0;
    while (uVar4 = _size___vector_JV__allocator_J_std___std__QBEIXZ(), local_24 < uVar4) {
      puVar5 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QBEABJI_Z(local_24);
      local_28 = (int *)_GetRegionNode_CRegionConnectionGraph__ABEPBVCRegionConnectionNode__J_Z
                                  (*puVar5);
      if (local_28 != (int *)0x0) {
        if (*local_28 == param_2) {
          local_8 = 0xffffffff;
          ATL::CAtlWinModule::~CAtlWinModule(local_20);
          ExceptionList = local_10;
          return param_2;
        }
        local_28[6] = DAT_04a55e14;
        local_28[7] = *local_28;
        _push_back___list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QAEXABQBVCRegionConnectionNode___Z
                  (&local_28);
      }
      local_24 = local_24 + 1;
    }
    while (cVar2 = _empty___list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QBE_NXZ
                             (), cVar2 == '\0') {
      piVar3 = (int *)_front___list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QAEAAPBVCRegionConnectionNode__XZ
                                ();
      local_2c = *piVar3;
      local_30 = 0;
      while (uVar4 = _size___vector_JV__allocator_J_std___std__QBEIXZ(), local_30 < uVar4) {
        puVar5 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QBEABJI_Z(local_30);
        local_34 = (int *)_GetRegionNode_CRegionConnectionGraph__ABEPBVCRegionConnectionNode__J_Z
                                    (*puVar5);
        if ((local_34 != (int *)0x0) && (local_34[6] != DAT_04a55e14)) {
          if (*local_34 == param_2) {
            iVar1 = *(int *)(local_2c + 0x1c);
            local_8 = 0xffffffff;
            ATL::CAtlWinModule::~CAtlWinModule(local_20);
            ExceptionList = local_10;
            return iVar1;
          }
          local_34[6] = DAT_04a55e14;
          local_34[7] = *(int *)(local_2c + 0x1c);
          _push_back___list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QAEXABQBVCRegionConnectionNode___Z
                    (&local_34);
        }
        local_30 = local_30 + 1;
      }
      _pop_front___list_PBVCRegionConnectionNode__V__allocator_PBVCRegionConnectionNode___std___std__QAEXXZ
                ();
    }
    local_8 = 0xffffffff;
    ATL::CAtlWinModule::~CAtlWinModule(local_20);
  }
  ExceptionList = local_10;
  return -1;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveNavigationMapToFile@CWorldMap@@QBEXJAAVCAFile@@AAVCFileFormatLevel@@ABV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z @ 01c8e300 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x01c8e4e0) */
/* WARNING: Removing unreachable block (ram,0x01c8e36f) */
/* WARNING: Removing unreachable block (ram,0x01c8e400) */
/* WARNING: Removing unreachable block (ram,0x01c8e5ff) */
/* [ported from ego_r via strfp] */

void _SaveNavigationMapToFile_CWorldMap__QBEXJAAVCAFile__AAVCFileFormatLevel__ABV__vector_VCCharString__V__allocator_VCCharString___std___std___Z
               (undefined4 param_1,int *param_2,int param_3)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined1 *puVar7;
  int *piVar8;
  undefined4 local_ec;
  undefined4 local_e8;
  uint local_e4;
  undefined1 local_e0 [148];
  uint local_4c;
  uint local_48;
  uint local_44;
  undefined4 local_40;
  CAtlWinModule local_3c [20];
  CAtlWinModule local_28 [20];
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c7832b;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  uVar2 = (**(code **)(*param_2 + 0x1c))();
  *(undefined4 *)(param_3 + 0x15) = uVar2;
  local_40 = 0;
  ___WriteVar_J_CAFile__QAEXABJ_Z(&local_40);
  __0__vector_VCCharString__V__allocator_VCCharString___std___std__QAE_XZ();
  local_8 = 0;
  uVar2 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ();
  _reserve___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXI_Z(uVar2);
  local_44 = 0;
  while (uVar4 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ(),
        local_44 < uVar4) {
    uVar2 = param_1;
    uVar3 = __A__vector_VCCharString__V__allocator_VCCharString___std___std__QBEABVCCharString__I_Z
                      (local_44);
    cVar1 = _IsSectionToHaveNavigationMap_CWorldMap__IBE_NABVCCharString__J_Z(uVar3,uVar2);
    if (cVar1 != '\0') {
      uVar2 = __A__vector_VCCharString__V__allocator_VCCharString___std___std__QBEABVCCharString__I_Z
                        (local_44);
      _push_back___vector_VCCharString__V__allocator_VCCharString___std___std__QAEXABVCCharString___Z
                (uVar2);
    }
    local_44 = local_44 + 1;
  }
  local_e8 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ();
  ___WriteVar_I_CAFile__QAEXABI_Z(&local_e8);
  uVar2 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ();
  __0__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAE_I_Z(uVar2);
  local_8 = CONCAT31(local_8._1_3_,1);
  local_48 = 0;
  while (uVar4 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ(),
        local_48 < uVar4) {
    __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
              (local_48);
    local_ec = _GetLength_CCharString__QBEJXZ();
    ___WriteVar_J_CAFile__QAEXABJ_Z(&local_ec);
    uVar2 = __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                      (local_48);
    ___WriteVar_VCCharString___CAFile__QAEXABVCCharString___Z(uVar2);
    uVar2 = (**(code **)(*param_2 + 0x1c))();
    puVar5 = (undefined4 *)
             __A__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAEAAU__pair_JJ_1_I_Z
                       (local_48);
    *puVar5 = uVar2;
    iVar6 = __A__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAEAAU__pair_JJ_1_I_Z
                      (local_48);
    ___WriteVar_J_CAFile__QAEXABJ_Z(iVar6 + 4);
    local_48 = local_48 + 1;
  }
  local_40 = (**(code **)(*param_2 + 0x1c))();
  (**(code **)(*param_2 + 0x14))(*(undefined4 *)(param_3 + 0x15));
  ___WriteVar_J_CAFile__QAEXABJ_Z(&local_40);
  (**(code **)(*param_2 + 0x14))(local_40);
  local_4c = 0;
  while (uVar4 = _size___vector_VCCharString__V__allocator_VCCharString___std___std__QBEIXZ(),
        local_4c < uVar4) {
    uVar2 = (**(code **)(*param_2 + 0x1c))();
    iVar6 = __A__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAEAAU__pair_JJ_1_I_Z
                      (local_4c);
    *(undefined4 *)(iVar6 + 4) = uVar2;
    __0CNavigationAreaInit__QAE_XZ();
    local_8._0_1_ = 2;
    puVar7 = local_e0;
    uVar2 = __A__vector_VCCharString__V__allocator_VCCharString___std___std__QAEAAVCCharString__I_Z
                      (local_4c);
    uVar3 = _GetMapBox_CWorldMap__QBEABVC2DBoxI__J_Z(param_1);
    _GetMapNavigationAreaInit_CWorldMap__IBEXJPBVC2DBoxI__ABVCCharString__AAVCNavigationAreaInit___Z
              (param_1,uVar3,uVar2,puVar7);
    uVar2 = param_1;
    _GetNavigatorManager_CWorld__QAEAAVCNavigatorManager__XZ(param_1);
    _RemoveNavigationMap_CNavigatorManager__QAEXJ_Z(uVar2);
    puVar7 = local_e0;
    uVar2 = param_1;
    _GetNavigatorManager_CWorld__QAEAAVCNavigatorManager__XZ(param_1,puVar7);
    _AddNavigationMap_CNavigatorManager__QAEXJABVCNavigationAreaInit___Z(uVar2,puVar7);
    uVar2 = param_1;
    piVar8 = param_2;
    _PeekNavigatorManager_CWorld__QBEABVCNavigatorManager__XZ(param_1,param_2);
    _SaveNavigationMapToFile_CNavigatorManager__QBEXJAAVCAFile___Z(uVar2,piVar8);
    uVar2 = param_1;
    _GetNavigatorManager_CWorld__QAEAAVCNavigatorManager__XZ(param_1);
    _ActivateNavigationMap_CNavigatorManager__QAEXJ_Z(uVar2);
    local_8 = CONCAT31(local_8._1_3_,1);
    __1CNavigationAreaInit__QAE_XZ();
    local_4c = local_4c + 1;
  }
  local_14 = (**(code **)(*param_2 + 0x1c))();
  local_e4 = 0;
  while (uVar4 = _size___vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QBEIXZ(),
        local_e4 < uVar4) {
    puVar5 = (undefined4 *)
             __A__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAEAAU__pair_JJ_1_I_Z
                       (local_e4);
    (**(code **)(*param_2 + 0x14))(*puVar5);
    iVar6 = __A__vector_U__pair_JJ_std__V__allocator_U__pair_JJ_std___2__std__QAEAAU__pair_JJ_1_I_Z
                      (local_e4);
    ___WriteVar_J_CAFile__QAEXABJ_Z(iVar6 + 4);
    local_e4 = local_e4 + 1;
  }
  (**(code **)(*param_2 + 0x14))(local_14);
  local_8 = local_8 & 0xffffff00;
  ATL::CAtlWinModule::~CAtlWinModule(local_3c);
  local_8 = 0xffffffff;
  ATL::CAtlWinModule::~CAtlWinModule(local_28);
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SaveToFile@CNavQuadTree@@QBEXAAVCAFile@@@Z @ 03289b50 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x03289e72) */
/* WARNING: Removing unreachable block (ram,0x03289f04) */
/* WARNING: Removing unreachable block (ram,0x03289fd7) */
/* WARNING: Removing unreachable block (ram,0x0328a069) */
/* [ported from ego_r via strfp] */

void __thiscall _SaveToFile_CNavQuadTree__QBEXAAVCAFile___Z(int param_1,int *param_2)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  undefined4 *puVar4;
  undefined4 uStack_200;
  undefined4 uStack_1fc;
  char *pcStack_1f8;
  undefined4 uStack_1f4;
  int *piStack_1f0;
  int *piStack_1ec;
  char *pcStack_1e8;
  int *piStack_1e4;
  wchar_t *pwStack_1e0;
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
  undefined4 local_188;
  undefined4 local_184;
  undefined4 local_180;
  undefined4 local_17c;
  undefined4 local_178;
  undefined4 local_174;
  undefined4 local_170;
  undefined4 local_16c;
  undefined4 local_168;
  int local_164;
  uint local_160;
  undefined1 local_114 [15];
  char local_105;
  undefined1 local_104 [15];
  char local_f5;
  int local_f4;
  undefined1 local_f0 [15];
  char local_e1;
  undefined1 local_e0 [15];
  char local_d1;
  undefined1 local_d0 [24];
  undefined *local_b8;
  undefined4 local_b4;
  undefined4 local_b0 [4];
  undefined1 *local_a0;
  undefined1 *local_9c;
  undefined1 *local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined1 local_8c [8];
  undefined1 local_84 [8];
  char local_7c [11];
  char local_71;
  int *local_70;
  undefined1 local_6c [12];
  undefined4 local_60;
  undefined1 local_5c [12];
  int *local_50;
  undefined1 local_4c [12];
  undefined4 local_40;
  undefined1 local_3c [12];
  char local_30 [20];
  undefined4 local_1c;
  int local_18;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03efd842;
  local_10 = ExceptionList;
  local_160 = 0;
  ExceptionList = &local_10;
  local_164 = param_1;
  if (*(char *)(param_1 + 4) != '\0') goto LAB_03289cce;
  ExceptionList = &local_10;
  if (DAT_04bac270 == 0) {
LAB_03289c42:
    local_168 = 1;
  }
  else {
    pwStack_1e0 = L"lib_nav_quad_tree.cpp";
    piStack_1e4 = (int *)0x3289baa;
    ExceptionList = &local_10;
    __0CWideString__QAE_PB_W_Z();
    local_8 = 0;
    local_160 = local_160 | 1;
    pwStack_1e0 = (wchar_t *)0xffffffff;
    piStack_1e4 = (int *)&DAT_044f3dcb;
    pcStack_1e8 = (char *)0x3289bcf;
    __0CCharString__QAE_PBDJ_Z();
    local_8 = 1;
    local_160 = local_160 | 2;
    pwStack_1e0 = (wchar_t *)0xffffffff;
    piStack_1e4 = (int *)0x44f45d0;
    pcStack_1e8 = (char *)0x3289bf7;
    __0CCharString__QAE_PBDJ_Z();
    local_8 = 2;
    local_160 = local_160 | 4;
    pwStack_1e0 = (wchar_t *)0x1;
    piStack_1e4 = (int *)0x59a;
    pcStack_1e8 = local_7c;
    piStack_1ec = (int *)local_84;
    piStack_1f0 = (int *)local_8c;
    uStack_1f4 = 0x3289c2e;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      ();
    if (cVar2 == '\x01') goto LAB_03289c42;
    local_168 = 0;
  }
  local_71 = (char)local_168;
  local_8 = 1;
  if ((local_160 & 4) != 0) {
    local_160 = local_160 & 0xfffffffb;
    pwStack_1e0 = L"䗇ü";
    __1CCharString__QAE_XZ();
  }
  local_8 = 0;
  if ((local_160 & 2) != 0) {
    local_160 = local_160 & 0xfffffffd;
    pwStack_1e0 = 
    L"䗇￼\xffff诿ꒅ\xfffe菿Ǡུꖃﺤ\xffff跾衍磨嘆࿾䶶薓瓉＋怕뱀蔄瓀찁喋謈謂ࡍ傋＜角\xe845䗇ð"
    ;
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if ((local_160 & 1) != 0) {
    local_160 = local_160 & 0xfffffffe;
    pwStack_1e0 = L"똏鍍즅୴ᗿ䁠Ҽ삅Ŵ诌ࡕʋ䶋謈᱐틿䖉쟨\xf045";
    __1CWideString__QAE_XZ();
  }
  if (local_71 != '\0') {
    pwStack_1e0 = L"삅Ŵ诌ࡕʋ䶋謈᱐틿䖉쟨\xf045";
    BVar3 = IsDebuggerPresent();
    if (BVar3 != 0) {
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
  }
LAB_03289cce:
  pwStack_1e0 = L"䖉쟨\xf045";
  local_1c = (**(code **)(*param_2 + 0x1c))();
  local_14 = 0;
  pwStack_1e0 = (wchar_t *)&local_14;
  piStack_1e4 = (int *)0x3289cf1;
  ___WriteVar_J_CAFile__QAEXABJ_Z();
  pwStack_1e0 = L"\b";
  piStack_1e4 = (int *)0x3289cfe;
  ___WriteVar_J_CAFile__QAEXABJ_Z();
  pwStack_1e0 = (wchar_t *)(local_164 + 0x10);
  piStack_1e4 = (int *)0x3289d10;
  ___WriteVar_M_CAFile__QAEXABM_Z();
  pwStack_1e0 = (wchar_t *)(local_164 + 0x14);
  piStack_1e4 = (int *)0x3289d22;
  ___WriteVar_M_CAFile__QAEXABM_Z();
  pwStack_1e0 = (wchar_t *)0x3289d33;
  local_90 = _size___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QBEIXZ();
  pwStack_1e0 = (wchar_t *)&local_90;
  piStack_1e4 = (int *)0x3289d48;
  ___WriteVar_I_CAFile__QAEXABI_Z();
  pwStack_1e0 = 
  L"薉ｰ\xffff趍ｰ\xffff譑ࡍ䫨墳课ꂕ\xfffe菿߂\xec83謌觌沥\xffff勿䖋倈싨嬤菾೬첋ꖉｨ\xffff譑ꂍ\xfffe菿烁ꛨ噝觾颅\xfffe诿颕\xfffe觿钕\xfffe쟿ﱅ\x03"
  ;
  local_94 = _size___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QBEIXZ
                       ();
  pwStack_1e0 = (wchar_t *)&local_94;
  piStack_1e4 = (int *)0x3289d6b;
  ___WriteVar_I_CAFile__QAEXABI_Z();
  piStack_1ec = (int *)(local_164 + 7);
  local_98 = (undefined1 *)&pcStack_1e8;
  piStack_1f0 = param_2;
  uStack_1f4 = 0x3289d89;
  __0CSaveNavigationPositionToFile__QAE_AAVCAFile__ABVC2DVector___Z();
  local_9c = (undefined1 *)&uStack_1f4;
  pcStack_1f8 = (char *)&uStack_1f4;
  uStack_1fc = 0x3289da3;
  local_170 = _end___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
                        ();
  local_8 = 3;
  local_a0 = (undefined1 *)&uStack_200;
  local_16c = local_170;
  local_174 = _begin___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
                        (&uStack_200);
  local_8 = 0xffffffff;
  ___for_each_V___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__VCSaveNavigationPositionToFile___std__YI_AVCSaveNavigationPositionToFile__V___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___0_0V1__Z
            ();
  pwStack_1e0 = L"薉ｔ\xffff趍ｔ\xffff譑ࡍꓨ墲跾푍የ吏쟾ﱅ\x04";
  local_b0[0] = _size___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBEIXZ
                          ();
  pwStack_1e0 = (wchar_t *)local_b0;
  piStack_1e4 = (int *)0x3289e11;
  ___WriteVar_I_CAFile__QAEXABI_Z();
  pwStack_1e0 = L"䗇Ӽ";
  __0__map_PBVCANavQuadTreeNode__JU__less_PBVCANavQuadTreeNode___std__V__allocator_U__pair_QBVCANavQuadTreeNode__J_std___3__std__QAE_XZ
            ();
  local_8 = 4;
  local_b4 = 0;
  local_b8 = &DAT_04bbfa8c;
  pwStack_1e0 = (wchar_t *)&local_b4;
  piStack_1e4 = (int *)0x3289e4c;
  pwStack_1e0 = (wchar_t *)
                ___make_pair_PAVCNavBlockedQuadTreeNode__H_std__YI_AU__pair_PAVCNavBlockedQuadTreeNode__H_0___QAPAVCNavBlockedQuadTreeNode____QAH_Z
                          ();
  piStack_1e4 = (int *)local_d0;
  pcStack_1e8 = (char *)0x3289e5c;
  ___insert_U__pair_PAVCNavBlockedQuadTreeNode__H_std______Tree_V___Tmap_traits_PBVCANavQuadTreeNode__JU__less_PBVCANavQuadTreeNode___std__V__allocator_U__pair_QBVCANavQuadTreeNode__J_std___3__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_PBVCANavQuadTreeNode__JU__less_PBVCANavQuadTreeNode___std__V__allocator_U__pair_QBVCANavQuadTreeNode__J_std___3__0A__std___std___std___N_1___QAU__pair_PAVCNavBlockedQuadTreeNode__H_1__Z
            ();
  pwStack_1e0 = L"䗇Ǭ";
  __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_PBVCANavQuadTreeNode__JU__less_PBVCANavQuadTreeNode___std__V__allocator_U__pair_QBVCANavQuadTreeNode__J_std___3__0A__std___std___std___N_std__QAE_XZ
            ();
  local_18 = 1;
  pwStack_1e0 = (wchar_t *)local_3c;
  piStack_1e4 = (int *)0x3289e89;
  _begin___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
            ();
  local_8._0_1_ = 5;
  while( true ) {
    pwStack_1e0 = (wchar_t *)local_e0;
    piStack_1e4 = (int *)0x3289eac;
    pwStack_1e0 = (wchar_t *)
                  _end___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
                            ();
    local_8._0_1_ = 6;
    piStack_1e4 = (int *)0x3289ed1;
    local_17c = pwStack_1e0;
    local_178 = pwStack_1e0;
    local_d1 = FID_conflict_operator__();
    local_8._0_1_ = 5;
    pwStack_1e0 = L"똏㎅\xffff藿࿀랄";
    __1___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
              ();
    if (local_d1 == '\0') break;
    pwStack_1e0 = L"䖉㏄瓉\xe905\x9e";
    local_40 = __D___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QBEABVCNavigationLayer_CNavQuadTree__XZ
                         ();
    pwStack_1e0 = (wchar_t *)local_4c;
    piStack_1e4 = (int *)0x3289f15;
    _begin___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
              ();
    local_8._0_1_ = 7;
    while( true ) {
      pwStack_1e0 = (wchar_t *)local_f0;
      piStack_1e4 = (int *)0x3289f32;
      pwStack_1e0 = (wchar_t *)
                    _end___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                              ();
      local_8._0_1_ = 8;
      piStack_1e4 = (int *)0x3289f57;
      local_184 = pwStack_1e0;
      local_180 = pwStack_1e0;
      local_e1 = FID_conflict_operator__();
      local_8._0_1_ = 7;
      pwStack_1e0 = 
      L"똏⎅\xffff藿瓀贤롍⃨卭课褈둍喍勔䖍僬䶋讴謑둍䊋ｐ\xebd0욀ﱅ贅롍\xefe8僰\xe9feﻣ\xffff䗆Ӽ䶍\xe8c8眮﹗䶋菬ǩ趉０\xffff閍０\xffff譒ࡍ냨劧㏾瓀\xe905Ő"
      ;
      __1___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                ();
      if (local_e1 == '\0') break;
      pwStack_1e0 = 
      L"ࢋ䶉趴푕赒\xec45譐둍ᆋ䶋讴偂탿胫䗆׼䶍\xe8b8\xf0ef﹐\xe3e9\xfffe웿ﱅ贄졍⻨坷课\xec4d\xe983褁ႍ\xffff跿႕\xffff勿䶋\xe808Ʞ﹒쀳մ僩\x01贀ꡍ譑ꂍ\xfffe菿᳁秨儮웾ﱅ\xeb09贈ꡍ㟨坰跾\x95\xffff勿趋ﺠ\xffff솃\xe81c纡﹑薉ﹼ\xffff薋ﹼ\xffff薉ﹸ\xffff䗆ૼ趋ﹸ\xffff赑ꡍࣨ嘌裾྅\xffff웿ﱅ贉\x8d\xffff\xe8ff皛﹗똏ྕ\xffff藿࿒욄"
      ;
      puVar4 = (undefined4 *)
               __D___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBEABQAVCANavQuadTreeNode__XZ
                         ();
      local_50 = (int *)*puVar4;
      pwStack_1e0 = (wchar_t *)local_30;
      piStack_1e4 = &local_18;
      pcStack_1e8 = (char *)0x3289f99;
      (**(code **)(*local_50 + 0x50))();
      pwStack_1e0 = 
      L"薍４\xffff譐쑍\xe8e8啲觾蒅\xfffe诿蒍\xfffe觿肍\xfffe웿ﱅ謈肕\xfffe勿䶍\xe8b8쿲﹕薈Ｃ\xffff䗆߼趍４\xffff⫨僱࿾薶Ｃ\xffff삅⑴䶍\xe8b8洠﹓ࢋ䶉趴푕赒\xec45譐둍ᆋ䶋讴偂탿胫䗆׼䶍\xe8b8\xf0ef﹐\xe3e9\xfffe웿ﱅ贄졍⻨坷课\xec4d\xe983褁ႍ\xffff跿႕\xffff勿䶋\xe808Ʞ﹒쀳մ僩\x01贀ꡍ譑ꂍ\xfffe菿᳁秨儮웾ﱅ\xeb09贈ꡍ㟨坰跾\x95\xffff勿趋ﺠ\xffff솃\xe81c纡﹑薉ﹼ\xffff薋ﹼ\xffff薉ﹸ\xffff䗆ૼ趋ﹸ\xffff赑ꡍࣨ嘌裾྅\xffff웿ﱅ贉\x8d\xffff\xe8ff皛﹗똏ྕ\xffff藿࿒욄"
      ;
      __E___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAEAAV01_XZ
                ();
    }
    local_8._0_1_ = 5;
    pwStack_1e0 = 
    L"\xe3e9\xfffe웿ﱅ贄졍⻨坷课\xec4d\xe983褁ႍ\xffff跿႕\xffff勿䶋\xe808Ʞ﹒쀳մ僩\x01贀ꡍ譑ꂍ\xfffe菿᳁秨儮웾ﱅ\xeb09贈ꡍ㟨坰跾\x95\xffff勿趋ﺠ\xffff솃\xe81c纡﹑薉ﹼ\xffff薋ﹼ\xffff薉ﹸ\xffff䗆ૼ趋ﹸ\xffff赑ꡍࣨ嘌裾྅\xffff웿ﱅ贉\x8d\xffff\xe8ff皛﹗똏ྕ\xffff藿࿒욄"
    ;
    __1___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
              ();
    pwStack_1e0 = 
    L"薍Ｄ\xffff譐ꂍ\xfffe菿᳁ۨ冀觾貅\xfffe诿貍\xfffe觿袍\xfffe웿ﱅ謆袕\xfffe勿䶍\xe8c8൭﹖薈Ｓ\xffff䗆׼趍Ｄ\xffffè坸࿾薶Ｓ\xffff삅萏·"
    ;
    __E___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAEAAV01_XZ
              ();
  }
  local_8._0_1_ = 4;
  pwStack_1e0 = L"䶋菬ǩ趉０\xffff閍０\xffff譒ࡍ냨劧㏾瓀\xe905Ő";
  __1___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
            ();
  local_f4 = local_18 + -1;
  pwStack_1e0 = (wchar_t *)&local_f4;
  piStack_1e4 = (int *)0x3289fd3;
  ___WriteVar_J_CAFile__QAEXABJ_Z();
  pwStack_1e0 = (wchar_t *)local_5c;
  piStack_1e4 = (int *)0x3289fee;
  _begin___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
            ();
  local_8._0_1_ = 9;
  while( true ) {
    pwStack_1e0 = (wchar_t *)local_104;
    piStack_1e4 = (int *)0x328a011;
    pwStack_1e0 = (wchar_t *)
                  _end___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
                            ();
    local_8._0_1_ = 10;
    piStack_1e4 = (int *)0x328a036;
    local_18c = pwStack_1e0;
    local_188 = pwStack_1e0;
    local_f5 = FID_conflict_operator__();
    local_8._0_1_ = 9;
    pwStack_1e0 = L"똏ྕ\xffff藿࿒욄";
    __1___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
              ();
    if (local_f5 == '\0') break;
    pwStack_1e0 = L"䖉㎤瓀\xe905­";
    local_60 = __D___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QBEABVCNavigationLayer_CNavQuadTree__XZ
                         ();
    pwStack_1e0 = (wchar_t *)local_6c;
    piStack_1e4 = (int *)0x328a07a;
    _begin___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
              ();
    local_8._0_1_ = 0xb;
    while( true ) {
      pwStack_1e0 = (wchar_t *)local_114;
      piStack_1e4 = (int *)0x328a097;
      pwStack_1e0 = (wchar_t *)
                    _end___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBE_AV___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                              ();
      local_8._0_1_ = 0xc;
      piStack_1e4 = (int *)0x328a0bc;
      local_194 = pwStack_1e0;
      local_190 = pwStack_1e0;
      local_105 = FID_conflict_operator__();
      local_8._0_1_ = 0xb;
      pwStack_1e0 = (wchar_t *)0x328a0d1;
      __1___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                ();
      if (local_105 == '\0') break;
      pwStack_1e0 = (wchar_t *)0x328a0e4;
      puVar4 = (undefined4 *)
               __D___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBEABQAVCANavQuadTreeNode__XZ
                         ();
      local_70 = (int *)*puVar4;
      pwStack_1e0 = (wchar_t *)(local_164 + 7);
      piStack_1e4 = (int *)0x1;
      pcStack_1e8 = local_30;
      piStack_1ec = param_2;
      piStack_1f0 = (int *)0x328a10a;
      (**(code **)(*local_70 + 0x54))();
      pwStack_1e0 = (wchar_t *)0x328a088;
      __E___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAEAAV01_XZ
                ();
    }
    local_8._0_1_ = 9;
    pwStack_1e0 = (wchar_t *)0x328a11b;
    __1___Vector_const_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
              ();
    pwStack_1e0 = 
    L"閍＀\xffff譒ꂍ\xfffe菿᳁ꇨ兾觾粅\xfffe诿粅\xfffe觿碅\xfffe웿ﱅ謊碍\xfffe凿䶍\xe8a8ఈ﹖薈／\xffff䗆ৼ趍＀\xffff鯨坶࿾閶／\xffff튅萏Æ"
    ;
    __E___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAEAAV01_XZ
              ();
  }
  local_8._0_1_ = 4;
  pwStack_1e0 = (wchar_t *)0x328a12c;
  __1___Vector_const_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
            ();
  pwStack_1e0 = L"\xef12경lib_nav_quad_tree.cpp";
  piStack_1e4 = (int *)0x328a139;
  ___WriteVar_J_CAFile__QAEXABJ_Z();
  pwStack_1e0 = (wchar_t *)0x328a146;
  local_14 = (**(code **)(*param_2 + 0x1c))();
  pwStack_1e0 = (wchar_t *)local_1c;
  piStack_1e4 = (int *)0x328a15a;
  (**(code **)(*param_2 + 0x14))();
  piStack_1e4 = &local_14;
  pcStack_1e8 = (char *)0x328a166;
  ___WriteVar_J_CAFile__QAEXABJ_Z();
  piStack_1e4 = (int *)local_14;
  pcStack_1e8 = (char *)0x328a177;
  (**(code **)(*param_2 + 0x14))();
  pcStack_1e8 = " kB) on disk.";
  piStack_1ec = (int *)0x328a19b;
  piStack_1ec = (int *)_GFIntToCharString__YI_AVCCharString__J_Z();
  local_8._0_1_ = 0xd;
  piStack_1f0 = (int *)0x44f45ec;
  uStack_1f4 = 0x328a1ce;
  local_19c = piStack_1ec;
  local_198 = piStack_1ec;
  uStack_1f4 = _GFIntToCharString__YI_AVCCharString__J_Z();
  local_8._0_1_ = 0xe;
  pcStack_1f8 = " nav info takes ";
  uStack_1fc = 0x328a204;
  local_1a4 = uStack_1f4;
  local_1a0 = uStack_1f4;
  uStack_1fc = _GFIntToCharString__YI_AVCCharString__J_Z();
  local_8._0_1_ = 0xf;
  uStack_200 = 0x328a231;
  local_1ac = uStack_1fc;
  local_1a8 = uStack_1fc;
  local_1b4 = __H_YI_AVCCharString__PBDABV0__Z();
  local_8._0_1_ = 0x10;
  uStack_1fc = 0x328a258;
  local_1b0 = local_1b4;
  local_1bc = __H_YI_AVCCharString__ABV0_PBD_Z();
  local_8._0_1_ = 0x11;
  pcStack_1f8 = (char *)0x328a27f;
  local_1b8 = local_1bc;
  local_1c4 = __H_YI_AVCCharString__ABV0_0_Z();
  local_8._0_1_ = 0x12;
  uStack_1f4 = 0x328a2a6;
  local_1c0 = local_1c4;
  local_1cc = __H_YI_AVCCharString__ABV0_PBD_Z();
  local_8._0_1_ = 0x13;
  piStack_1f0 = (int *)0x328a2cd;
  local_1c8 = local_1cc;
  local_1d4 = __H_YI_AVCCharString__ABV0_0_Z();
  local_8._0_1_ = 0x14;
  piStack_1ec = (int *)0x328a2f4;
  local_1d0 = local_1d4;
  local_1dc = __H_YI_AVCCharString__ABV0_PBD_Z();
  local_8._0_1_ = 0x15;
  pcStack_1e8 = (char *)0x328a315;
  local_1d8 = local_1dc;
  __BCCharString__QBEPBDXZ();
  pcStack_1e8 = (char *)0x328a31c;
  _GFOutputDebugString__YIXPBD_Z();
  local_8._0_1_ = 0x14;
  pcStack_1e8 = (char *)0x328a32b;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0x13;
  pcStack_1e8 = (char *)0x328a33a;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0x12;
  pcStack_1e8 = (char *)0x328a349;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0x11;
  pcStack_1e8 = (char *)0x328a358;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0x10;
  pcStack_1e8 = (char *)0x328a367;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0xf;
  pcStack_1e8 = (char *)0x328a376;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0xe;
  pcStack_1e8 = (char *)0x328a385;
  __1CCharString__QAE_XZ();
  local_8._0_1_ = 0xd;
  pcStack_1e8 = (char *)0x328a394;
  __1CCharString__QAE_XZ();
  local_8 = CONCAT31(local_8._1_3_,4);
  pcStack_1e8 = (char *)0x328a3a3;
  __1CCharString__QAE_XZ();
  pcStack_1e8 = (char *)0x328a3ad;
  _GFOutputDebugString__YIXPBD_Z();
  local_8 = 0xffffffff;
  pcStack_1e8 = (char *)0x328a3bc;
  __1__map_PBVCANavQuadTreeNode__JU__less_PBVCANavQuadTreeNode___std__V__allocator_U__pair_QBVCANavQuadTreeNode__J_std___3__std__QAE_XZ
            ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?LoadFromFile@CNavQuadTree@@QAE_NAAVCAFile@@JABVC2DBoxI@@AAVCASuspendableProcess@@@Z @ 0328a760 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x0328bd34) */
/* WARNING: Removing unreachable block (ram,0x0328b5d8) */
/* WARNING: Removing unreachable block (ram,0x0328b267) */
/* WARNING: Removing unreachable block (ram,0x0328b1b5) */
/* WARNING: Removing unreachable block (ram,0x0328b388) */
/* WARNING: Removing unreachable block (ram,0x0328b404) */
/* WARNING: Removing unreachable block (ram,0x0328bc88) */
/* [ported from ego_r via strfp] */

undefined1 __thiscall
_LoadFromFile_CNavQuadTree__QAE_NAAVCAFile__JABVC2DBoxI__AAVCASuspendableProcess___Z
          (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  char cVar12;
  undefined1 uVar13;
  BOOL BVar14;
  undefined4 *puVar15;
  undefined4 uVar16;
  int iVar17;
  uint uVar18;
  float10 fVar19;
  char *pcVar20;
  undefined1 *puVar21;
  undefined4 uVar22;
  undefined **ppuVar23;
  undefined4 uVar24;
  char local_30c;
  char local_308;
  char local_304;
  undefined *local_300;
  undefined *local_2fc;
  undefined *local_2f8;
  char local_2ec;
  char local_2e8;
  char local_2dc;
  char local_2c0;
  char local_2a4;
  undefined1 local_294 [15];
  char local_285;
  undefined1 local_284 [15];
  char local_275;
  undefined1 local_274 [8];
  undefined1 local_26c [8];
  undefined1 local_264 [11];
  char local_259;
  undefined1 local_258 [8];
  undefined1 local_250 [8];
  undefined1 local_248 [11];
  char local_23d;
  undefined1 local_23c [8];
  undefined1 local_234 [8];
  undefined1 local_22c [11];
  char local_221;
  undefined1 local_220 [24];
  void *local_208;
  undefined *local_204;
  void *local_200;
  undefined *local_1fc;
  undefined *local_1f8;
  void *local_1f4;
  undefined *local_1f0;
  undefined1 local_1ec [24];
  undefined4 local_1d4;
  undefined *local_1d0 [7];
  undefined1 local_1b4 [8];
  undefined1 local_1ac [8];
  undefined1 local_1a4 [11];
  char local_199;
  undefined1 local_198 [8];
  undefined1 local_190 [8];
  undefined1 local_188 [11];
  char local_17d;
  undefined1 local_17c [32];
  undefined1 local_15c [10];
  char local_152;
  undefined1 local_151;
  undefined1 local_150 [32];
  undefined1 local_130 [11];
  char local_125;
  undefined1 local_124 [8];
  undefined1 local_11c [8];
  undefined1 local_114 [8];
  undefined1 local_10c [8];
  undefined1 local_104 [11];
  char local_f9;
  int *local_f8;
  undefined1 local_f4 [12];
  undefined4 local_e8;
  undefined1 local_e4 [12];
  uint local_d8;
  undefined *local_d4;
  char local_cd;
  undefined1 local_cc [10];
  undefined1 local_c2;
  undefined1 local_c1;
  undefined1 local_c0 [8];
  undefined1 local_b8 [6];
  char local_b2;
  char local_b1;
  undefined *local_b0;
  char local_a9;
  int local_a8;
  uint local_a4;
  undefined1 local_a0 [12];
  uint local_94;
  uint local_90;
  uint local_8c;
  int local_88;
  undefined1 local_84 [20];
  uint local_70;
  int local_6c;
  int local_68;
  undefined1 local_64 [52];
  double local_30;
  undefined1 local_24 [4];
  uint local_20;
  uint local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03efdd32;
  local_10 = ExceptionList;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  ExceptionList = &local_10;
  if (*(char *)(param_1 + 4) != '\0') {
    ExceptionList = &local_10;
    if (DAT_04bac270 == 0) {
LAB_0328a85e:
      local_2a4 = '\x01';
    }
    else {
      ExceptionList = &local_10;
      __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
      local_8 = 0;
      __0CCharString__QAE_PBDJ_Z(&DAT_044f3e0e,0xffffffff);
      local_8 = 1;
      __0CCharString__QAE_PBDJ_Z("NOT Initialised",0xffffffff);
      local_8 = 2;
      bVar4 = true;
      bVar3 = true;
      bVar2 = true;
      cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                         (local_114,local_10c,local_104,0x5e9,1);
      if (cVar12 == '\x01') goto LAB_0328a85e;
      local_2a4 = '\0';
    }
    local_f9 = local_2a4;
    local_8 = 1;
    if (bVar3) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 0;
    if (bVar4) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 0xffffffff;
    if (bVar2) {
      __1CWideString__QAE_XZ();
    }
    if ((local_f9 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar13 = (*pcVar1)();
      return uVar13;
    }
  }
  bVar11 = false;
  bVar10 = false;
  bVar9 = false;
  bVar8 = false;
  bVar7 = false;
  bVar6 = false;
  bVar5 = false;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  *(undefined4 *)(param_1 + 0x18) = param_3;
  puVar21 = local_124;
  _GetTLPos_C2DBoxI__QBE_AVC2DCoordI__XZ(local_11c);
  puVar15 = (undefined4 *)_ToC2DCoordF_C2DCoordI__QBE_AVC2DVector__XZ(puVar21);
  uVar16 = puVar15[1];
  *(undefined4 *)(param_1 + 7) = *puVar15;
  *(undefined4 *)(param_1 + 0xb) = uVar16;
  __0CCachedFile__QAE_J_Z(0x4b000);
  local_8 = 3;
  _OpenFromFile_CCachedFile__QAEXAAVCAFile__PAVCASuspendableProcess___Z(param_2,param_5);
  ___ReadVar_J_CAFile__QAEXAAJPAVCASuspendableProcess___Z(local_24,param_5);
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_6c,4,0);
  if (local_6c < 5) {
    if (DAT_04bac270 != 0) {
      __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
      local_8 = CONCAT31(local_8._1_3_,4);
      __0CCharString__QAE_PBDJ_Z(&DAT_044f3e0f,0xffffffff);
      local_8 = 5;
      uVar24 = 3;
      uVar22 = 0x5fb;
      puVar21 = local_130;
      pcVar20 = " will have to be  saved again.";
      uVar16 = _GFIntToCharString__YI_AVCCharString__J_Z
                         (" will have to be  saved again.",puVar21,0x5fb,3);
      local_8 = 6;
      __H_YI_AVCCharString__PBDABV0__Z(uVar16);
      local_8 = 7;
      uVar16 = __H_YI_AVCCharString__ABV0_PBD_Z(pcVar20);
      local_8 = 8;
      bVar6 = true;
      bVar5 = true;
      bVar4 = true;
      bVar3 = true;
      bVar2 = true;
      cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                         (local_150,uVar16,puVar21,uVar22,uVar24);
      if (cVar12 != '\x01') {
        local_2c0 = '\0';
        goto LAB_0328aad0;
      }
    }
    local_2c0 = '\x01';
LAB_0328aad0:
    local_125 = local_2c0;
    local_8 = 7;
    if (bVar2) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 6;
    if (bVar3) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 5;
    if (bVar4) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 4;
    if (bVar5) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 3;
    if (bVar6) {
      __1CWideString__QAE_XZ();
    }
    if ((local_125 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar13 = (*pcVar1)();
      return uVar13;
    }
    local_151 = 0;
    local_8 = 0xffffffff;
    __1CCachedFile__UAE_XZ();
    ExceptionList = local_10;
    return local_151;
  }
  if (local_6c != 8) {
    if (DAT_04bac270 == 0) {
LAB_0328ad29:
      local_2dc = '\x01';
    }
    else {
      __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
      local_8 = CONCAT31(local_8._1_3_,9);
      __0CCharString__QAE_PBDJ_Z("version_number==QUAD_TREE_VERSION_NUMBER",0xffffffff);
      local_8 = 10;
      uVar24 = 3;
      uVar22 = 0x5fe;
      puVar21 = local_15c;
      pcVar20 = " will have to be saved again.";
      uVar16 = _GFIntToCharString__YI_AVCCharString__J_Z
                         (" will have to be saved again.",puVar21,0x5fe,3);
      local_8 = 0xb;
      __H_YI_AVCCharString__PBDABV0__Z(uVar16);
      local_8 = 0xc;
      uVar16 = __H_YI_AVCCharString__ABV0_PBD_Z(pcVar20);
      local_8 = 0xd;
      bVar11 = true;
      bVar10 = true;
      bVar9 = true;
      bVar8 = true;
      bVar7 = true;
      cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                         (local_17c,uVar16,puVar21,uVar22,uVar24);
      if (cVar12 == '\x01') goto LAB_0328ad29;
      local_2dc = '\0';
    }
    local_152 = local_2dc;
    local_8 = 0xc;
    if (bVar7) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 0xb;
    if (bVar8) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 10;
    if (bVar9) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 9;
    if (bVar10) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 3;
    if (bVar11) {
      __1CWideString__QAE_XZ();
    }
    if ((local_152 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar13 = (*pcVar1)();
      return uVar13;
    }
  }
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  fVar19 = (float10)_GFGetTime__YINXZ();
  local_30 = (double)fVar19;
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(param_1 + 0x10,4,0);
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(param_1 + 0x14,4,0);
  iVar17 = _GetWidth_C2DBoxI__QBEJXZ();
  if ((*(float *)(param_1 + 0x10) != (float)iVar17) ||
     (iVar17 = _GetHeight_C2DBoxI__QBEJXZ(), *(float *)(param_1 + 0x14) != (float)iVar17)) {
    if (DAT_04bac270 == 0) {
LAB_0328af6d:
      local_2e8 = '\x01';
    }
    else {
      __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
      local_8 = CONCAT31(local_8._1_3_,0xe);
      __0CCharString__QAE_PBDJ_Z(&DAT_044f3e52,0xffffffff);
      local_8 = 0xf;
      __0CCharString__QAE_PBDJ_Z
                ("MapWidth==map_area.GetWidth() && MapHeight==map_area.GetHeight()",0xffffffff);
      local_8 = 0x10;
      bVar4 = true;
      bVar3 = true;
      bVar2 = true;
      cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                         (local_198,local_190,local_188,0x606,1);
      if (cVar12 == '\x01') goto LAB_0328af6d;
      local_2e8 = '\0';
    }
    local_17d = local_2e8;
    local_8 = 0xf;
    if (bVar2) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 0xe;
    if (bVar3) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 3;
    if (bVar4) {
      __1CWideString__QAE_XZ();
    }
    if ((local_17d != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar13 = (*pcVar1)();
      return uVar13;
    }
  }
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_8c,4,0);
  cVar12 = _empty___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QBE_NXZ();
  if (cVar12 == '\0') {
    if (DAT_04bac270 == 0) {
LAB_0328b108:
      local_2ec = '\x01';
    }
    else {
      __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
      local_8 = CONCAT31(local_8._1_3_,0x11);
      __0CCharString__QAE_PBDJ_Z(&DAT_044f3e53,0xffffffff);
      local_8 = 0x12;
      __0CCharString__QAE_PBDJ_Z("Regions.empty()",0xffffffff);
      local_8 = 0x13;
      bVar4 = true;
      bVar3 = true;
      bVar2 = true;
      cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                         (local_1b4,local_1ac,local_1a4,0x60b,1);
      if (cVar12 == '\x01') goto LAB_0328b108;
      local_2ec = '\0';
    }
    local_199 = local_2ec;
    local_8 = 0x12;
    if (bVar2) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 0x11;
    if (bVar3) {
      __1CCharString__QAE_XZ();
    }
    local_8 = 3;
    if (bVar4) {
      __1CWideString__QAE_XZ();
    }
    if ((local_199 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar13 = (*pcVar1)();
      return uVar13;
    }
  }
  bVar7 = false;
  bVar6 = false;
  bVar5 = false;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  for (local_90 = 0; local_90 < local_8c; local_90 = local_90 + 1) {
    uVar16 = __0CNavRegion__QAE_JJ_Z(*(undefined4 *)(param_1 + 0x18),local_90 + 1);
    local_8._0_1_ = 0x14;
    _push_back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEX__QAVCNavRegion___Z
              (uVar16);
    local_8 = CONCAT31(local_8._1_3_,3);
    __1CNavRegion__QAE_XZ();
  }
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_20,4,0);
  _reserve___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAEXI_Z
            (local_20);
  for (local_94 = 0; local_94 < local_20; local_94 = local_94 + 1) {
    _GFProcessBreath__YIXAAVCASuspendableProcess__AAN_Z();
    __0CNavigationPosition__QAE_XZ();
    _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(local_a0,0xc,0);
    __YC2DVector__QAEABV0_ABV0__Z(param_1 + 7);
    _push_back___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAEXABVCNavigationPosition___Z
              (local_a0);
  }
  __0__map_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__std__QAE_XZ
            ();
  local_8 = CONCAT31(local_8._1_3_,0x15);
  local_1d0[0] = &DAT_04bbfa8c;
  local_1d4 = 0;
  uVar16 = ___make_pair_HPAVCNavBlockedQuadTreeNode___std__YI_AU__pair_HPAVCNavBlockedQuadTreeNode___0___QAH__QAPAVCNavBlockedQuadTreeNode___Z
                     (local_1d0);
  ___insert_U__pair_HPAVCNavBlockedQuadTreeNode___std______Tree_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std___std___N_1___QAU__pair_HPAVCNavBlockedQuadTreeNode___1__Z
            (local_1ec,uVar16);
  __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std___std___N_std__QAE_XZ
            ();
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_1c,4,0);
  _resize___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEXI_Z
            (local_1c);
  local_88 = __ftol2_sse();
  for (local_a4 = 0; local_a4 < local_1c; local_a4 = local_a4 + 1) {
    _GFProcessBreath__YIXAAVCASuspendableProcess__AAN_Z();
    uVar16 = 0;
    iVar17 = local_88;
    __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
              (local_a4);
    _resize___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAEXIPAVCANavQuadTreeNode___Z
              (iVar17,uVar16);
  }
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_68,4,0);
  local_70 = 0;
  local_18 = 0;
  local_a8 = 0;
  while( true ) {
    bVar10 = false;
    bVar9 = false;
    bVar8 = false;
    if (local_68 <= local_a8) break;
    _GFProcessBreath__YIXAAVCASuspendableProcess__AAN_Z();
    local_b2 = '\0';
    if (5 < local_6c) {
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_b2,1,0);
    }
    _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_a9,1,0);
    _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_b1,1,0);
    local_b0 = (undefined *)0x0;
    if ((local_b2 == '\0') && (local_b1 != '\0')) {
      if (local_a9 == '\0') {
        if (DAT_04bac270 == 0) {
LAB_0328b85a:
          local_304 = '\x01';
        }
        else {
          __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
          local_8 = CONCAT31(local_8._1_3_,0x19);
          __0CCharString__QAE_PBDJ_Z(&DAT_044f3e96,0xffffffff);
          local_8 = 0x1a;
          __0CCharString__QAE_PBDJ_Z("top_level",0xffffffff);
          local_8 = 0x1b;
          bVar4 = true;
          bVar3 = true;
          bVar2 = true;
          cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                             (local_23c,local_234,local_22c,0x66a,1);
          if (cVar12 == '\x01') goto LAB_0328b85a;
          local_304 = '\0';
        }
        local_221 = local_304;
        local_8 = 0x1a;
        if (bVar2) {
          bVar2 = false;
          __1CCharString__QAE_XZ();
        }
        local_8 = 0x19;
        if (bVar3) {
          bVar3 = false;
          __1CCharString__QAE_XZ();
        }
        local_8 = 0x15;
        if (bVar4) {
          bVar4 = false;
          __1CWideString__QAE_XZ();
        }
        if ((local_221 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
          pcVar1 = (code *)swi(3);
          uVar13 = (*pcVar1)();
          return uVar13;
        }
      }
      DAT_04bbfa99 = (undefined1)local_70;
      local_b0 = &DAT_04bbfa8c;
    }
    else {
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_cd,1,0);
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_c2,1,0);
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_c1,1,0);
      __0C2DVector__QAE_XZ();
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(local_c0,8,0);
      __HC2DVector__QBE_AV0_ABV0__Z(local_cc,param_1 + 7);
      _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(local_b8,4,0);
      if (local_cd == '\0') {
        local_208 = operator_new(0x20);
        local_8._0_1_ = 0x18;
        if (local_208 == (void *)0x0) {
          local_300 = (undefined *)0x0;
        }
        else {
          local_300 = (undefined *)
                      __0CNavQuadTreeNode__QAE_ABVC2DVector__EEAAVCAFile___Z
                                (local_cc,local_c2,local_c1,local_64);
        }
        local_204 = local_300;
        local_8 = CONCAT31(local_8._1_3_,0x15);
        local_b0 = local_300;
      }
      else if (local_b2 == '\0') {
        local_200 = operator_new(0x20);
        local_8._0_1_ = 0x17;
        if (local_200 == (void *)0x0) {
          local_2fc = (undefined *)0x0;
        }
        else {
          local_2fc = (undefined *)
                      __0CNavNavigableLeafNode__QAE_ABVC2DVector__EEAAVCAFile__ABV__list_VCNavRegion__V__allocator_VCNavRegion___std___std___N_Z
                                (local_cc,local_c2,local_c1,local_64,param_1 + 0xac,6 < local_6c);
        }
        local_1fc = local_2fc;
        local_8 = CONCAT31(local_8._1_3_,0x15);
        local_b0 = local_2fc;
      }
      else {
        local_1f4 = operator_new(0x38);
        local_8._0_1_ = 0x16;
        if (local_1f4 == (void *)0x0) {
          local_2f8 = (undefined *)0x0;
        }
        else {
          local_2f8 = (undefined *)
                      __0CNavSwitchableLeafNode__QAE_ABVC2DVector__EEAAVCAFile__ABV__list_VCNavRegion__V__allocator_VCNavRegion___std___std___N_Z
                                (local_cc,local_c2,local_c1,local_64,param_1 + 0xac,6 < local_6c);
        }
        local_1f0 = local_2f8;
        local_8 = CONCAT31(local_8._1_3_,0x15);
        local_d4 = local_2f8;
        local_d8 = 0;
        while (uVar18 = _size___vector_U__pair__K_N_std__V__allocator_U__pair__K_N_std___2__std__QBEIXZ
                                  (), local_d8 < uVar18) {
          local_1f8 = local_d4;
          ppuVar23 = &local_1f8;
          uVar16 = __A__CArray_U__pair__K_N_std____QAEAAU__pair__K_N_std__I_Z(local_d8);
          _Insert___CVectorMap__KPAVCANavQuadTreeNode__V__CKeyPairCompareLess__KPAVCANavQuadTreeNode______QAEXAB_KABQAVCANavQuadTreeNode___Z
                    (uVar16,ppuVar23);
          local_d8 = local_d8 + 1;
        }
        local_b0 = local_d4;
      }
      uVar16 = ___make_pair_AAJAAPAVCANavQuadTreeNode___std__YI_AU__pair_JPAVCANavQuadTreeNode___0_AAJAAPAVCANavQuadTreeNode___Z
                         (&local_b0);
      ___insert_U__pair_JPAVCANavQuadTreeNode___std______Tree_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std__QAE_AU__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std___std___N_1___QAU__pair_JPAVCANavQuadTreeNode___1__Z
                (local_220,uVar16);
      __1__pair_V___Tree_iterator_V___Tree_val_V___Tmap_traits_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__0A__std___std___std___N_std__QAE_XZ
                ();
    }
    if (local_a9 != '\0') {
      if ((local_1c <= local_70) ||
         (uVar18 = _GetNavLayer_CANavQuadTreeNode__QBEJXZ(), uVar18 != local_70)) {
        if (DAT_04bac270 == 0) {
LAB_0328ba04:
          local_308 = '\x01';
        }
        else {
          __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
          local_8 = CONCAT31(local_8._1_3_,0x1c);
          __0CCharString__QAE_PBDJ_Z(&DAT_044f3e97,0xffffffff);
          local_8 = 0x1d;
          __0CCharString__QAE_PBDJ_Z
                    ("current_nav_layer<number_of_nav_layers && pnode->GetNavLayer()==current_nav_layer"
                     ,0xffffffff);
          local_8 = 0x1e;
          bVar7 = true;
          bVar6 = true;
          bVar5 = true;
          cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                             (local_258,local_250,local_248,0x672,1);
          if (cVar12 == '\x01') goto LAB_0328ba04;
          local_308 = '\0';
        }
        local_23d = local_308;
        local_8 = 0x1d;
        if (bVar5) {
          bVar5 = false;
          __1CCharString__QAE_XZ();
        }
        local_8 = 0x1c;
        if (bVar6) {
          bVar6 = false;
          __1CCharString__QAE_XZ();
        }
        local_8 = 0x15;
        if (bVar7) {
          bVar7 = false;
          __1CWideString__QAE_XZ();
        }
        if ((local_23d != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
          pcVar1 = (code *)swi(3);
          uVar13 = (*pcVar1)();
          return uVar13;
        }
      }
      iVar17 = local_18;
      __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                (local_70);
      puVar15 = (undefined4 *)
                __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z(iVar17);
      *puVar15 = local_b0;
      local_18 = local_18 + 1;
      if (local_88 <= local_18) {
        local_70 = local_70 + 1;
        local_18 = 0;
      }
    }
    local_a8 = local_a8 + 1;
  }
  _Read_CCachedFile__UAEXPAXJPAVCASuspendableProcess___Z(&local_14,4,0);
  if (local_14 == -0x534210ee) goto LAB_0328bc84;
  if (DAT_04bac270 == 0) {
LAB_0328bbdb:
    local_30c = '\x01';
  }
  else {
    __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
    local_8 = CONCAT31(local_8._1_3_,0x1f);
    __0CCharString__QAE_PBDJ_Z(&DAT_044f3ed1,0xffffffff);
    local_8 = 0x20;
    __0CCharString__QAE_PBDJ_Z("test_end_symbol==END_QUAD_TREE_SYMBOL",0xffffffff);
    local_8 = 0x21;
    bVar10 = true;
    bVar9 = true;
    bVar8 = true;
    cVar12 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                       (local_274,local_26c,local_264,0x67f,1);
    if (cVar12 == '\x01') goto LAB_0328bbdb;
    local_30c = '\0';
  }
  local_259 = local_30c;
  local_8 = 0x20;
  if (bVar8) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0x1f;
  if (bVar9) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0x15;
  if (bVar10) {
    __1CWideString__QAE_XZ();
  }
  if ((local_259 != '\0') && (BVar14 = IsDebuggerPresent(), BVar14 != 0)) {
    pcVar1 = (code *)swi(3);
    uVar13 = (*pcVar1)();
    return uVar13;
  }
LAB_0328bc84:
  _begin___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
            (local_e4);
  local_8._0_1_ = 0x22;
  while( true ) {
    uVar16 = _end___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___2_XZ
                       (local_284);
    local_8._0_1_ = 0x23;
    local_275 = FID_conflict_operator__(uVar16);
    local_8._0_1_ = 0x22;
    __1___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
              ();
    if (local_275 == '\0') break;
    _GFProcessBreath__YIXAAVCASuspendableProcess__AAN_Z();
    local_e8 = __D___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QBEAAVCNavigationLayer_CNavQuadTree__XZ
                         ();
    _begin___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
              (local_f4);
    local_8._0_1_ = 0x24;
    while( true ) {
      uVar16 = _end___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                         (local_294);
      local_8._0_1_ = 0x25;
      local_285 = FID_conflict_operator__(uVar16);
      local_8._0_1_ = 0x24;
      __1___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                ();
      if (local_285 == '\0') break;
      puVar15 = (undefined4 *)
                __D___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBEAAPAVCANavQuadTreeNode__XZ
                          ();
      local_f8 = (int *)*puVar15;
      (**(code **)(*local_f8 + 0x58))(local_84);
      __E___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAEAAV01_XZ
                ();
    }
    local_8._0_1_ = 0x22;
    __1___Vector_iterator_V___Vector_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
              ();
    __E___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAEAAV01_XZ
              ();
  }
  local_8._0_1_ = 0x15;
  __1___Vector_iterator_V___Vector_val_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std___std__QAE_XZ
            ();
  _GFProcessBreath__YIXAAVCASuspendableProcess__AAN_Z();
  _Update___CVectorMap__KPAVCANavQuadTreeNode__V__CKeyPairCompareLess__KPAVCANavQuadTreeNode______QAEXXZ
            ();
  _UnblockAllSwitchableNodes_CNavQuadTree__AAEXXZ();
  *(undefined1 *)(param_1 + 4) = 1;
  local_8 = CONCAT31(local_8._1_3_,3);
  __1__map_JPAVCANavQuadTreeNode__U__less_J_std__V__allocator_U__pair___CBJPAVCANavQuadTreeNode___std___3__std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  __1CCachedFile__UAE_XZ();
  ExceptionList = local_10;
  return 1;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?Initialise@CNavQuadTree@@QAEXABVCNavigationAreaInit@@@Z @ 03290030 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x032902e1) */
/* WARNING: Removing unreachable block (ram,0x032903ef) */
/* WARNING: Removing unreachable block (ram,0x03290bb2) */
/* WARNING: Removing unreachable block (ram,0x032904ab) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void __fastcall _Initialise_CNavQuadTree__QAEXABVCNavigationAreaInit___Z(int param_1)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  char cVar11;
  BOOL BVar12;
  uint uVar13;
  int iVar14;
  undefined4 *puVar15;
  int *piVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  float10 fVar20;
  char *pcVar21;
  char *pcVar22;
  undefined8 uStack_2f4;
  char local_23c;
  undefined4 local_238;
  char local_230;
  char local_228;
  undefined4 local_224;
  char local_210;
  undefined1 local_174 [12];
  undefined1 *local_168;
  char local_155;
  undefined1 local_154 [8];
  undefined1 local_14c [8];
  undefined1 local_144 [11];
  char local_139;
  void *local_138;
  undefined4 local_134;
  undefined4 *local_130;
  undefined4 *local_12c;
  undefined1 local_128 [8];
  undefined1 local_120 [8];
  undefined1 local_118 [11];
  char local_10d;
  undefined4 *local_10c;
  undefined4 *local_108;
  undefined1 local_104 [8];
  undefined1 local_fc [8];
  undefined1 local_f4 [11];
  char local_e9;
  void *local_e8;
  undefined4 local_e4;
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [11];
  char local_85;
  undefined1 local_84 [15];
  undefined1 local_75;
  CAtlWinModule local_74 [20];
  int local_60;
  uint local_5c;
  undefined4 local_58;
  uint local_54;
  undefined4 local_50;
  float local_4c;
  float local_48;
  uint local_44;
  int local_40;
  undefined1 local_3c [4];
  undefined4 local_38;
  int local_34;
  undefined4 local_2c;
  undefined1 local_28 [4];
  undefined4 local_24;
  double local_20;
  undefined1 local_14 [4];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03efea14;
  local_10 = ExceptionList;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  local_2c = 0;
  ExceptionList = &local_10;
  *(undefined1 *)(param_1 + 6) = 1;
  fVar20 = (float10)_GFGetTime__YINXZ();
  local_20 = (double)fVar20;
  if (*(char *)(param_1 + 4) != '\0') goto LAB_032901e0;
  if (DAT_04bac270 == 0) {
LAB_03290148:
    local_210 = '\x01';
  }
  else {
    __0CWideString__QAE_PB_W_Z();
    local_8 = 0;
    uStack_2f4 = (double)CONCAT44(0x32900cf,(char *)uStack_2f4);
    __0CCharString__QAE_PBDJ_Z();
    local_8 = 1;
    uStack_2f4 = (double)CONCAT44(0x32900f7,(char *)uStack_2f4);
    __0CCharString__QAE_PBDJ_Z();
    local_8 = 2;
    bVar4 = true;
    bVar3 = true;
    bVar2 = true;
    uStack_2f4 = (double)CONCAT44(local_90,local_98);
    cVar11 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                       (local_a0);
    if (cVar11 == '\x01') goto LAB_03290148;
    local_210 = '\0';
  }
  local_85 = local_210;
  local_8 = 1;
  if (bVar3) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0;
  if (bVar4) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if (bVar2) {
    __1CWideString__QAE_XZ();
  }
  if ((local_85 != '\0') && (BVar12 = IsDebuggerPresent(), BVar12 != 0)) {
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
LAB_032901e0:
  bVar10 = false;
  bVar9 = false;
  bVar8 = false;
  bVar7 = false;
  bVar6 = false;
  bVar5 = false;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  __4__vector_EV__allocator_E_std___std__QAEAAV01_ABV01__Z();
  __4__vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAEAAV01_ABV01__Z
            ();
  __4__vector_VC2DBoxF__V__allocator_VC2DBoxF___std___std__QAEAAV01_ABV01__Z();
  __4__vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAEAAV01_ABV01__Z
            ();
  __4__vector_VCNavigationTransitionPoint__V__allocator_VCNavigationTransitionPoint___std___std__QAEAAV01_ABV01__Z
            ();
  _clear___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEXXZ
            ();
  cVar11 = _empty___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QBE_NXZ
                     ();
  if (cVar11 == '\0') {
    local_40 = __ftol2_sse();
    local_34 = __ftol2_sse();
    _size___vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEIXZ
              ();
    _reserve___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEXI_Z
              ();
    local_44 = 0;
    while ((uVar13 = _size___vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEIXZ
                               (), local_44 < uVar13 || (local_44 == 0))) {
      uStack_2f4 = (double)CONCAT44(_DAT_044f344c * (float)_DAT_04022eb8 + *(float *)(param_1 + 7),
                                    0x3290361);
      __0C2DVector__QAE_MM_Z();
      __0CNavigationLayer_CNavQuadTree__QAE_XZ();
      local_8 = 3;
      uStack_2f4 = (double)CONCAT44(0x329039a,(char *)uStack_2f4);
      _push_back___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEX__QAVCNavigationLayer_CNavQuadTree___Z
                ();
      local_8 = 0xffffffff;
      __1CNavigationLayer_CNavQuadTree__QAE_XZ();
      uVar13 = _size___vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEIXZ
                         ();
      if (local_44 < uVar13) {
        uStack_2f4 = (double)CONCAT44(0x32903c9,(char *)uStack_2f4);
        __A__vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEABVCNavigationLayerUpdate_CNavigationAreaUpdate__I_Z
                  ();
        uStack_2f4 = (double)CONCAT44(local_44,0x32903dc);
        iVar14 = __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                           ();
        uStack_2f4 = (double)CONCAT44(iVar14 + 0x14,0x32903eb);
        _InitialiseLines_CNavQuadTree__ABEXAAV__vector_V__list_VC2DLineF__V__allocator_VC2DLineF___std___std__V__allocator_V__list_VC2DLineF__V__allocator_VC2DLineF___std___std___2__std__ABV__vector_VC2DLineF__V__allocator_VC2DLineF___std___3__Z
                  ();
        local_54 = 0;
        while( true ) {
          uStack_2f4 = (double)CONCAT44(0x329040f,(char *)uStack_2f4);
          __A__vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEABVCNavigationLayerUpdate_CNavigationAreaUpdate__I_Z
                    ();
          uVar13 = _size___vector_VCSwitchableBlockage_CNavigationAreaUpdate__V__allocator_VCSwitchableBlockage_CNavigationAreaUpdate___std___std__QBEIXZ
                             ();
          if (uVar13 <= local_54) break;
          uStack_2f4 = (double)CONCAT44(local_44,0x329042e);
          __A__vector_VCNavigationLayerUpdate_CNavigationAreaUpdate__V__allocator_VCNavigationLayerUpdate_CNavigationAreaUpdate___std___std__QBEABVCNavigationLayerUpdate_CNavigationAreaUpdate__I_Z
                    ();
          uStack_2f4 = (double)CONCAT44(0x3290438,(char *)uStack_2f4);
          local_58 = __A__vector_VCSwitchableBlockage_CNavigationAreaUpdate__V__allocator_VCSwitchableBlockage_CNavigationAreaUpdate___std___std__QBEABVCSwitchableBlockage_CNavigationAreaUpdate__I_Z
                               ();
          uStack_2f4._0_4_ = (char *)local_44;
          uStack_2f4._4_4_ = local_58;
          __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                    ();
          uStack_2f4 = (double)CONCAT44(uStack_2f4._4_4_,0x3290462);
          _Insert___CVectorMap__KV__list_VC2DLineF__V__allocator_VC2DLineF___std___std__V__CKeyPairCompareLess__KV__list_VC2DLineF__V__allocator_VC2DLineF___std___std______QAEXAB_KABV__list_VC2DLineF__V__allocator_VC2DLineF___std___std___Z
                    ();
          local_54 = local_54 + 1;
        }
        uStack_2f4 = (double)CONCAT44(0x3290476,(char *)uStack_2f4);
        __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                  ();
        _Update___CVectorMap__KV__list_VC2DLineF__V__allocator_VC2DLineF___std___std__V__CKeyPairCompareLess__KV__list_VC2DLineF__V__allocator_VC2DLineF___std___std______QAEXXZ
                  ();
      }
      uStack_2f4 = (double)CONCAT44(0x3290492,(char *)uStack_2f4);
      local_50 = __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                           ();
      uStack_2f4 = (double)CONCAT44(local_40 * local_34,0x32904a7);
      _resize___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAEXIPAVCANavQuadTreeNode___Z
                ();
      local_5c = 0;
      while (uVar13 = _size___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBEIXZ
                                (), local_5c < uVar13) {
        local_60 = 0;
        __0__vector__KV__allocator__K_std___std__QAE_XZ();
        local_8 = 4;
        uStack_2f4 = (double)CONCAT44(0x32904e5,(char *)uStack_2f4);
        local_e8 = operator_new(0x20);
        local_8._0_1_ = 5;
        if (local_e8 == (void *)0x0) {
          local_224 = 0;
        }
        else {
          uStack_2f4 = (double)CONCAT44(&local_60,param_1);
          local_224 = __0CNavQuadTreeNode__QAE_ABVC2DVector__EEABVCNavQuadTree__AAW4ENavQuadTreeNodeType__AAV__vector__KV__allocator__K_std___std___Z
                                (&local_4c,0,local_44 & 0xff);
        }
        local_e4 = local_224;
        local_8._0_1_ = 4;
        uStack_2f4 = (double)CONCAT44(0x329054e,(char *)uStack_2f4);
        puVar15 = (undefined4 *)__A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z()
        ;
        *puVar15 = local_e4;
        if (local_60 == 2) {
          uStack_2f4 = (double)CONCAT44(0x329056c,(char *)uStack_2f4);
          piVar16 = (int *)__A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          iVar14 = _GetBlockedNode_CNavQuadTree__SIPAVCNavBlockedQuadTreeNode__XZ();
          if (*piVar16 == iVar14) {
            if (DAT_04bac270 == 0) {
LAB_03290636:
              local_228 = '\x01';
            }
            else {
              uStack_2f4 = (double)CONCAT44(0x3290598,(char *)uStack_2f4);
              __0CWideString__QAE_PB_W_Z();
              local_8 = CONCAT31(local_8._1_3_,6);
              uStack_2f4 = 6.414030855073125e-288;
              __0CCharString__QAE_PBDJ_Z();
              local_8 = 7;
              uStack_2f4 = 6.425839790831895e-288;
              __0CCharString__QAE_PBDJ_Z();
              local_8 = 8;
              bVar4 = true;
              bVar3 = true;
              bVar2 = true;
              uStack_2f4 = (double)CONCAT44(0x83f,local_f4);
              cVar11 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                                 (local_104,local_fc);
              if (cVar11 == '\x01') goto LAB_03290636;
              local_228 = '\0';
            }
            local_e9 = local_228;
            local_8 = 7;
            if (bVar2) {
              bVar2 = false;
              __1CCharString__QAE_XZ();
            }
            local_8 = 6;
            if (bVar3) {
              bVar3 = false;
              __1CCharString__QAE_XZ();
            }
            local_8._0_1_ = 4;
            local_8._1_3_ = 0;
            if (bVar4) {
              bVar4 = false;
              __1CWideString__QAE_XZ();
            }
            if ((local_e9 != '\0') && (BVar12 = IsDebuggerPresent(), BVar12 != 0)) {
              pcVar1 = (code *)swi(3);
              (*pcVar1)();
              return;
            }
          }
          uStack_2f4 = (double)CONCAT44(0x32906da,(char *)uStack_2f4);
          piVar16 = (int *)__A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          local_10c = (undefined4 *)*piVar16;
          local_108 = local_10c;
          if (local_10c != (undefined4 *)0x0) {
            uStack_2f4 = (double)CONCAT44(0x329070b,(char *)uStack_2f4);
            (**(code **)*local_10c)();
          }
          uVar17 = _GetBlockedNode_CNavQuadTree__SIPAVCNavBlockedQuadTreeNode__XZ();
          uStack_2f4 = (double)CONCAT44(0x3290730,(char *)uStack_2f4);
          puVar15 = (undefined4 *)
                    __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          *puVar15 = uVar17;
        }
        else if (local_60 == 1) {
          uStack_2f4 = (double)CONCAT44(0x329074d,(char *)uStack_2f4);
          piVar16 = (int *)__A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          iVar14 = _GetBlockedNode_CNavQuadTree__SIPAVCNavBlockedQuadTreeNode__XZ();
          if (*piVar16 == iVar14) {
            if (DAT_04bac270 == 0) {
LAB_0329081d:
              local_230 = '\x01';
            }
            else {
              uStack_2f4 = (double)CONCAT44(0x3290779,(char *)uStack_2f4);
              __0CWideString__QAE_PB_W_Z();
              local_8 = CONCAT31(local_8._1_3_,9);
              uStack_2f4 = 6.4140339865865385e-288;
              __0CCharString__QAE_PBDJ_Z();
              local_8 = 10;
              uStack_2f4 = 6.426065259772749e-288;
              __0CCharString__QAE_PBDJ_Z();
              local_8 = 0xb;
              bVar7 = true;
              bVar6 = true;
              bVar5 = true;
              uStack_2f4 = (double)CONCAT44(0x845,local_118);
              cVar11 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                                 (local_128,local_120);
              if (cVar11 == '\x01') goto LAB_0329081d;
              local_230 = '\0';
            }
            local_10d = local_230;
            local_8 = 10;
            if (bVar5) {
              bVar5 = false;
              __1CCharString__QAE_XZ();
            }
            local_8 = 9;
            if (bVar6) {
              bVar6 = false;
              __1CCharString__QAE_XZ();
            }
            local_8._0_1_ = 4;
            local_8._1_3_ = 0;
            if (bVar7) {
              bVar7 = false;
              __1CWideString__QAE_XZ();
            }
            if ((local_10d != '\0') && (BVar12 = IsDebuggerPresent(), BVar12 != 0)) {
              pcVar1 = (code *)swi(3);
              (*pcVar1)();
              return;
            }
          }
          uStack_2f4 = (double)CONCAT44(0x32908cc,(char *)uStack_2f4);
          __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          uStack_2f4 = (double)CONCAT44(0x32908da,(char *)uStack_2f4);
          local_75 = _GetPreferabilityCostForNode_CNavQuadTree__QBEEPBVCANavQuadTreeNode___Z();
          uStack_2f4 = (double)CONCAT44(0x32908e9,(char *)uStack_2f4);
          piVar16 = (int *)__A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          local_130 = (undefined4 *)*piVar16;
          local_12c = local_130;
          if (local_130 != (undefined4 *)0x0) {
            uStack_2f4 = (double)CONCAT44(0x329091a,(char *)uStack_2f4);
            (**(code **)*local_130)();
          }
          uStack_2f4 = (double)CONCAT44(0x3290933,(char *)uStack_2f4);
          local_138 = operator_new(0x20);
          local_8._0_1_ = 0xc;
          if (local_138 == (void *)0x0) {
            local_238 = 0;
          }
          else {
            uStack_2f4 = (double)((ulonglong)(local_44 & 0xff) << 0x20);
            local_238 = __0CNavNavigableLeafNode__QAE_ABVC2DVector__EEE_Z(&local_4c);
          }
          local_134 = local_238;
          local_8 = CONCAT31(local_8._1_3_,4);
          uStack_2f4 = (double)CONCAT44(0x3290992,(char *)uStack_2f4);
          puVar15 = (undefined4 *)
                    __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z();
          *puVar15 = local_134;
        }
        else if (local_60 != 0) {
          if (DAT_04bac270 == 0) {
LAB_03290a6c:
            local_23c = '\x01';
          }
          else {
            uStack_2f4 = (double)CONCAT44(0x32909c6,(char *)uStack_2f4);
            __0CWideString__QAE_PB_W_Z();
            local_8 = CONCAT31(local_8._1_3_,0xd);
            uStack_2f4 = 6.414037118100032e-288;
            __0CCharString__QAE_PBDJ_Z();
            local_8 = 0xe;
            uStack_2f4 = 6.4262907287136806e-288;
            __0CCharString__QAE_PBDJ_Z();
            local_8 = 0xf;
            bVar10 = true;
            bVar9 = true;
            bVar8 = true;
            uStack_2f4 = (double)CONCAT44(0x84c,local_144);
            cVar11 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                               (local_154,local_14c);
            if (cVar11 == '\x01') goto LAB_03290a6c;
            local_23c = '\0';
          }
          local_139 = local_23c;
          local_8 = 0xe;
          if (bVar8) {
            bVar8 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 0xd;
          if (bVar9) {
            bVar9 = false;
            __1CCharString__QAE_XZ();
          }
          local_8 = 4;
          if (bVar10) {
            bVar10 = false;
            __1CWideString__QAE_XZ();
          }
          if ((local_139 != '\0') && (BVar12 = IsDebuggerPresent(), BVar12 != 0)) {
            pcVar1 = (code *)swi(3);
            (*pcVar1)();
            return;
          }
        }
        local_5c = local_5c + 1;
        if ((int)local_5c % local_40 == 0) {
          local_4c = _DAT_044f344c * (float)_DAT_04022eb8 + *(float *)(param_1 + 7);
          local_48 = local_48 + _DAT_044f344c;
        }
        else {
          local_4c = local_4c + _DAT_044f344c;
        }
        local_8 = 0xffffffff;
        ATL::CAtlWinModule::~CAtlWinModule(local_74);
      }
      uStack_2f4 = (double)CONCAT44(0x3290b7e,(char *)uStack_2f4);
      _CalculateNeighbours_CNavQuadTree__ABEXAAV__vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___Z
                ();
      local_44 = local_44 + 1;
    }
    _JoinNavigationLayers_CNavQuadTree__AAEXXZ();
    uStack_2f4 = (double)CONCAT44(0x3290ba3,(char *)uStack_2f4);
    _SetUpRegions_CNavQuadTree__AAEXJ_Z();
    _VerifyTransitionPoints_CNavQuadTree__ABEXXZ();
    uStack_2f4 = (double)CONCAT44(0x3290bc9,(char *)uStack_2f4);
    _begin___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
              ();
    local_8._0_1_ = 0x10;
    local_8._1_3_ = 0;
    while( true ) {
      uStack_2f4 = (double)CONCAT44(0x3290be5,(char *)uStack_2f4);
      _end___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
                ();
      local_8._0_1_ = 0x11;
      uStack_2f4 = (double)CONCAT44(0x3290c0a,(char *)uStack_2f4);
      local_155 = FID_conflict_operator__();
      local_8._0_1_ = 0x10;
      __1___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
                ();
      if (local_155 == '\0') break;
      __D___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QBEAAVCNavigationPosition__XZ
                ();
      uStack_2f4 = (double)CONCAT44(0x3290c42,(char *)uStack_2f4);
      cVar11 = _IsActionPointReachable_CNavQuadTree__ABE_NABVCNavigationPosition___Z();
      if (cVar11 == '\0') {
        local_168 = (undefined1 *)&uStack_2f4;
        FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<wchar_t>_>_>
                  (local_84);
        _erase___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_V___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2__Z
                  (local_174);
        local_8._0_1_ = 0x12;
        uStack_2f4 = (double)CONCAT44(0x3290c9d,(char *)uStack_2f4);
        __4___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAEAAV01_ABV01__Z
                  ();
        local_8._0_1_ = 0x10;
        __1___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
                  ();
      }
      else {
        __E___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAEAAV01_XZ
                  ();
      }
    }
    local_8 = 0xffffffff;
    __1___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
              ();
    uStack_2f4 = (double)CONCAT44(local_28,local_3c);
    local_24 = _GetMemoryUsage_CNavQuadTree__QBEKAAK000_Z(local_14);
    uStack_2f4 = (double)CONCAT44(0x3290cfb,(char *)uStack_2f4);
    uStack_2f4._4_4_ = _GFIntToCharString__YI_AVCCharString__J_Z();
    local_8 = 0x13;
    uStack_2f4._0_4_ = " are non-blocked leaf nodes and ";
    uVar17 = _GFIntToCharString__YI_AVCCharString__J_Z();
    local_8._0_1_ = 0x14;
    pcVar22 = " bytes. Of these, ";
    uVar18 = _GFIntToCharString__YI_AVCCharString__J_Z(" bytes. Of these, ",uVar17);
    local_8._0_1_ = 0x15;
    pcVar21 = " quad tree nodes, which take up ";
    uVar19 = _GFIntToCharString__YI_AVCCharString__J_Z(" quad tree nodes, which take up ",uVar18);
    local_8._0_1_ = 0x16;
    __H_YI_AVCCharString__PBDABV0__Z(uVar19);
    local_8._0_1_ = 0x17;
    __H_YI_AVCCharString__ABV0_PBD_Z(pcVar21);
    local_8._0_1_ = 0x18;
    __H_YI_AVCCharString__ABV0_0_Z(uVar18);
    local_8._0_1_ = 0x19;
    __H_YI_AVCCharString__ABV0_PBD_Z(pcVar22);
    local_8._0_1_ = 0x1a;
    __H_YI_AVCCharString__ABV0_0_Z(uVar17);
    local_8._0_1_ = 0x1b;
    __H_YI_AVCCharString__ABV0_PBD_Z();
    local_8._0_1_ = 0x1c;
    uStack_2f4 = (double)CONCAT44(uStack_2f4._4_4_,0x3290ea5);
    __H_YI_AVCCharString__ABV0_0_Z();
    local_8._0_1_ = 0x1d;
    uStack_2f4 = (double)CONCAT44(0x3290ecc,(char *)uStack_2f4);
    __H_YI_AVCCharString__ABV0_PBD_Z();
    local_8._0_1_ = 0x1e;
    __BCCharString__QBEPBDXZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_8._0_1_ = 0x1d;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x1c;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x1b;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x1a;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x19;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x18;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x17;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x16;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x15;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 0x14;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,0x13);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
    _GFOutputDebugString__YIXPBD_Z();
    uStack_2f4 = (double)CONCAT44(0x3290fbf,(char *)uStack_2f4);
    fVar20 = (float10)_GFGetTime__YINXZ();
    uStack_2f4 = (double)(fVar20 - (float10)local_20);
    uVar17 = _GFFloatToCharString__YI_AVCCharString__NJ_Z();
    local_8 = 0x1f;
    uStack_2f4 = (double)CONCAT44(uVar17,0x3291008);
    __H_YI_AVCCharString__PBDABV0__Z();
    local_8._0_1_ = 0x20;
    uStack_2f4 = (double)CONCAT44(0x329102f,(char *)uStack_2f4);
    __H_YI_AVCCharString__ABV0_PBD_Z();
    local_8._0_1_ = 0x21;
    __BCCharString__QBEPBDXZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_8._0_1_ = 0x20;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,0x1f);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_38 = 0;
    uStack_2f4 = (double)CONCAT44(0x32910ae,(char *)uStack_2f4);
    uVar17 = _GFUIntToCharString__YI_AVCCharString__K_Z();
    local_8 = 0x22;
    uStack_2f4 = (double)CONCAT44(uVar17,0x32910de);
    __H_YI_AVCCharString__PBDABV0__Z();
    local_8._0_1_ = 0x23;
    uStack_2f4 = (double)CONCAT44(0x3291105,(char *)uStack_2f4);
    __H_YI_AVCCharString__ABV0_PBD_Z();
    local_8._0_1_ = 0x24;
    __BCCharString__QBEPBDXZ();
    _GFOutputDebugString__YIXPBD_Z();
    local_8._0_1_ = 0x23;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,0x22);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
    _GFOutputDebugString__YIXPBD_Z();
    *(undefined1 *)(param_1 + 6) = 0;
  }
  else {
    _GFOutputDebugString__YIXPBD_Z();
    _GFOutputDebugString__YIXPBD_Z();
    *(undefined1 *)(param_1 + 6) = 0;
  }
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?SetUpRegions@CNavQuadTree@@AAEXJ@Z @ 0328f270 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* WARNING: Removing unreachable block (ram,0x0328f6c8) */
/* WARNING: Removing unreachable block (ram,0x0328f796) */
/* WARNING: Removing unreachable block (ram,0x0328f58e) */
/* WARNING: Removing unreachable block (ram,0x0328f463) */
/* WARNING: Removing unreachable block (ram,0x0328f6fa) */
/* WARNING: Removing unreachable block (ram,0x0328f8d6) */
/* WARNING: Removing unreachable block (ram,0x0328f908) */
/* [ported from ego_r via strfp] */

void _SetUpRegions_CNavQuadTree__AAEXJ_Z(undefined4 param_1)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  BOOL BVar6;
  undefined4 uVar7;
  uint uVar8;
  undefined4 *puVar9;
  int *piVar10;
  char local_12c;
  undefined1 local_100 [15];
  char local_f1;
  undefined1 local_d8 [15];
  char local_c9;
  undefined1 local_c8 [15];
  char local_b9;
  undefined1 local_b8 [36];
  undefined1 local_94 [8];
  undefined1 local_8c [8];
  undefined1 local_84 [11];
  char local_79;
  int *local_78;
  uint local_74;
  uint local_70;
  undefined1 local_6c [12];
  CAtlWinModule local_60 [16];
  int *local_50;
  uint local_4c;
  uint local_48;
  undefined1 local_44 [12];
  CAtlWinModule local_38 [16];
  int *local_28;
  int local_24;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03efe6b8;
  local_10 = ExceptionList;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  ExceptionList = &local_10;
  cVar5 = _empty___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QBE_NXZ();
  if (cVar5 != '\0') goto LAB_0328f402;
  if (DAT_04bac270 == 0) {
LAB_0328f373:
    local_12c = '\x01';
  }
  else {
    __0CWideString__QAE_PB_W_Z(L"lib_nav_quad_tree.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_044f408a,0xffffffff);
    local_8 = 1;
    __0CCharString__QAE_PBDJ_Z("Regions.empty()",0xffffffff);
    local_8 = 2;
    bVar4 = true;
    bVar3 = true;
    bVar2 = true;
    cVar5 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (local_94,local_8c,local_84,0x7a7,1);
    if (cVar5 == '\x01') goto LAB_0328f373;
    local_12c = '\0';
  }
  local_79 = local_12c;
  local_8 = 1;
  if (bVar3) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0;
  if (bVar4) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if (bVar2) {
    __1CWideString__QAE_XZ();
  }
  if ((local_79 != '\0') && (BVar6 = IsDebuggerPresent(), BVar6 != 0)) {
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
LAB_0328f402:
  local_14 = 1;
  uVar7 = __0CNavRegion__QAE_JJ_Z(param_1,1);
  local_8 = 3;
  _push_back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEX__QAVCNavRegion___Z
            (uVar7);
  local_8 = 0xffffffff;
  __1CNavRegion__QAE_XZ();
  uVar7 = _begin___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
                    (local_b8);
  local_8 = 4;
  FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<wchar_t>_>_>(uVar7);
  local_8._0_1_ = 6;
  __1___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
            ();
  while( true ) {
    uVar7 = _end___vector_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std__QAE_AV___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___2_XZ
                      (local_c8);
    local_8._0_1_ = 7;
    local_b9 = FID_conflict_operator__(uVar7);
    local_8._0_1_ = 6;
    __1___Vector_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
              ();
    if (local_b9 == '\0') break;
    local_24 = __D___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QBEABVCNavigationPosition__XZ
                         ();
    local_28 = (int *)_GetNodeAt_CNavQuadTree__QBEPBVCANavQuadTreeNode__ABVC2DVector__J_Z
                                (local_24,*(undefined4 *)(local_24 + 8));
    if (local_28 != (int *)0x0) {
      __0__list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_XZ();
      local_8._0_1_ = 8;
      uVar7 = _back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEAAVCNavRegion__XZ
                        (local_38);
      cVar5 = (**(code **)(*local_28 + 0x30))(uVar7);
      if (cVar5 != '\0') {
        _begin___list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                  (local_44);
        local_8._0_1_ = 9;
        while( true ) {
          uVar7 = _end___list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                            (local_d8);
          local_8._0_1_ = 10;
          local_c9 = __9___List_const_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBE_NABV01__Z
                               (uVar7);
          local_8._0_1_ = 9;
          __1___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                    ();
          if (local_c9 == '\0') break;
          puVar9 = (undefined4 *)
                   __D___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBEAAPAVCANavQuadTreeNode__XZ
                             ();
          piVar10 = (int *)*puVar9;
          uVar7 = _back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEAAVCNavRegion__XZ
                            (local_38);
          (**(code **)(*piVar10 + 0x30))(uVar7);
          __E___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAEAAV01_XZ
                    ();
        }
        local_8._0_1_ = 8;
        __1___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                  ();
        local_14 = local_14 + 1;
        uVar7 = __0CNavRegion__QAE_JJ_Z(param_1,local_14);
        local_8._0_1_ = 0xb;
        _push_back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEX__QAVCNavRegion___Z
                  (uVar7);
        local_8._0_1_ = 8;
        __1CNavRegion__QAE_XZ();
      }
      local_8._0_1_ = 6;
      ATL::CAtlWinModule::~CAtlWinModule(local_38);
    }
    __E___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAEAAV01_XZ
              ();
  }
  local_8 = 0xffffffff;
  __1___Vector_const_iterator_V___Vector_val_VCNavigationPosition__V__allocator_VCNavigationPosition___std___std___std__QAE_XZ
            ();
  local_48 = 0;
  while (uVar8 = _size___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBEIXZ
                           (), local_48 < uVar8) {
    local_4c = 0;
    while( true ) {
      __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                (local_48);
      uVar8 = _size___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBEIXZ
                        ();
      if (uVar8 <= local_4c) break;
      uVar8 = local_4c;
      __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                (local_48);
      puVar9 = (undefined4 *)
               __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z(uVar8);
      local_50 = (int *)*puVar9;
      __0__list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_XZ();
      local_8 = 0xc;
      while( true ) {
        uVar7 = _back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEAAVCNavRegion__XZ
                          (local_60);
        cVar5 = (**(code **)(*local_50 + 0x34))(uVar7);
        if (cVar5 == '\0') break;
        _begin___list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                  (local_6c);
        local_8._0_1_ = 0xd;
        while( true ) {
          uVar7 = _end___list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAE_AV___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___2_XZ
                            (local_100);
          local_8._0_1_ = 0xe;
          local_f1 = __9___List_const_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBE_NABV01__Z
                               (uVar7);
          local_8._0_1_ = 0xd;
          __1___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                    ();
          if (local_f1 == '\0') break;
          puVar9 = (undefined4 *)
                   __D___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QBEAAPAVCANavQuadTreeNode__XZ
                             ();
          piVar10 = (int *)*puVar9;
          uVar7 = _back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEAAVCNavRegion__XZ
                            (local_60);
          (**(code **)(*piVar10 + 0x30))(uVar7);
          __E___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAEAAV01_XZ
                    ();
        }
        local_8._0_1_ = 0xc;
        __1___List_iterator_V___List_val_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std___std__QAE_XZ
                  ();
        local_14 = local_14 + 1;
        uVar7 = __0CNavRegion__QAE_JJ_Z(param_1,local_14);
        local_8._0_1_ = 0xf;
        _push_back___list_VCNavRegion__V__allocator_VCNavRegion___std___std__QAEX__QAVCNavRegion___Z
                  (uVar7);
        local_8 = CONCAT31(local_8._1_3_,0xc);
        __1CNavRegion__QAE_XZ();
        _clear___list_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QAEXXZ();
      }
      local_8 = 0xffffffff;
      ATL::CAtlWinModule::~CAtlWinModule(local_60);
      local_4c = local_4c + 1;
    }
    local_48 = local_48 + 1;
  }
  local_70 = 0;
  while (uVar8 = _size___vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QBEIXZ
                           (), local_70 < uVar8) {
    local_74 = 0;
    while( true ) {
      __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                (local_70);
      uVar8 = _size___vector_PAVCANavQuadTreeNode__V__allocator_PAVCANavQuadTreeNode___std___std__QBEIXZ
                        ();
      if (uVar8 <= local_74) break;
      uVar8 = local_74;
      __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                (local_70);
      puVar9 = (undefined4 *)
               __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z(uVar8);
      local_78 = (int *)*puVar9;
      cVar5 = (**(code **)(*local_78 + 0x38))();
      if ((cVar5 != '\0') &&
         (piVar10 = (int *)_GetBlockedNode_CNavQuadTree__SIPAVCNavBlockedQuadTreeNode__XZ(),
         local_78 != piVar10)) {
        if (local_78 != (int *)0x0) {
          (**(code **)*local_78)(1);
        }
        local_78 = (int *)0x0;
        uVar7 = _GetBlockedNode_CNavQuadTree__SIPAVCNavBlockedQuadTreeNode__XZ();
        uVar8 = local_74;
        __A__vector_VCNavigationLayer_CNavQuadTree__V__allocator_VCNavigationLayer_CNavQuadTree___std___std__QAEAAVCNavigationLayer_CNavQuadTree__I_Z
                  (local_70);
        puVar9 = (undefined4 *)
                 __A__CArray_PAVCANavQuadTreeNode____QAEAAPAVCANavQuadTreeNode__I_Z(uVar8);
        *puVar9 = uVar7;
      }
      local_74 = local_74 + 1;
    }
    local_70 = local_70 + 1;
  }
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?LoadTopologyFromFile@CWorldMap@@UAEXJV?$CCountedPointer@VCAFile@@@@AAVCASuspendableProcess@@@Z @ 01c89d20 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void __thiscall
_LoadTopologyFromFile_CWorldMap__UAEXJV__CCountedPointer_VCAFile____AAVCASuspendableProcess___Z
          (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int *param_5)

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
  undefined1 *puVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  int *piVar15;
  undefined4 local_58;
  undefined1 local_48 [8];
  undefined1 local_40 [8];
  undefined1 local_38 [11];
  char local_2d;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_03c779f3;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  iVar3 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_2);
  if (*(char *)(iVar3 + 0x2c) == '\0') {
    __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_040b78f2,0xffffffff);
    local_8._0_1_ = 2;
    __0CCharString__QAE_PBDJ_Z("Maps[index].Used",0xffffffff);
    local_8._0_1_ = 3;
    uVar14 = 1;
    uVar13 = 0x92f;
    puVar12 = local_1c;
    puVar11 = local_24;
    puVar10 = local_2c;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar10,puVar11,puVar12,0x92f,1);
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
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_2);
  iVar3 = _get___CCountedPointer_VCMap____QBEPAVCMap__XZ();
  if (iVar3 != 0) {
    __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
    local_8._0_1_ = 4;
    __0CCharString__QAE_PBDJ_Z(&DAT_040b78f3,0xffffffff);
    local_8._0_1_ = 5;
    __0CCharString__QAE_PBDJ_Z("Maps[index].PMap.get()==NULL",0xffffffff);
    local_8._0_1_ = 6;
    uVar14 = 1;
    uVar13 = 0x930;
    puVar12 = local_38;
    puVar11 = local_40;
    puVar10 = local_48;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar10,puVar11,puVar12,0x930,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar10,puVar11,puVar12,uVar13,uVar14);
    local_2d = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 5;
    __1CCharString__QAE_XZ();
    local_8._0_1_ = 4;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CWideString__QAE_XZ();
    if (local_2d != '\0') {
      BVar4 = IsDebuggerPresent();
      if (BVar4 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  pvVar5 = operator_new(0x1d60);
  local_8._0_1_ = 7;
  if (pvVar5 == (void *)0x0) {
    local_58 = 0;
  }
  else {
    piVar15 = param_5;
    uVar6 = __D__CCountedPointer_VCAFile____QBEAAVCAFile__XZ(param_5);
    uVar7 = _GetMapBox_CWorldMap__QBEABVC2DBoxI__J_Z(param_2);
    uVar14 = param_2;
    uVar8 = _GetWorldSeed_CWorld__QBEPAKXZ(param_2,uVar7,uVar6);
    uVar13 = *(undefined4 *)(param_1 + 8);
    uVar9 = _PeekDefinitionManager_CWorld__QBEABVCGameDefinitionManager__XZ(uVar13,uVar8);
    local_58 = __0CMap__QAE_ABVCGameDefinitionManager__AAVCWorld__PAKJABVC2DBoxI__AAVCAFile__PAVCASuspendableProcess___Z
                         (uVar9,uVar13,uVar8,uVar14,uVar7,uVar6,piVar15);
  }
  local_8 = (uint)local_8._1_3_ << 8;
  __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_2);
  _reset___CCountedPointer_VCMap____QAEXPAVCMap___Z(local_58);
  _push_back___CArray_J__QAEXABJ_Z(&param_2);
  (**(code **)(*param_5 + 8))();
  _UpdateAllEdges_CWorldMap__IAEXXZ();
  local_8 = 0xffffffff;
  __1__CCountedPointer_VCAFile____QAE_XZ();
  ExceptionList = local_10;
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?ActivateTopology@CWorldMap@@QAEXJ@Z @ 01c8e7d0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [ported from ego_r via strfp] */

void _ActivateTopology_CWorldMap__QAEXJ_Z(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(param_1);
  *(undefined1 *)(iVar1 + 0x2e) = 1;
  return;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /FableWin.exe (HeadlessAnalyzer)  
EXIT=0
