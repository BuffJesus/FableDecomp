param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release',

    [string]$RetailFrontendBank = ''
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$workspaceRoot = Split-Path -Parent $rebuildRoot
$vcRoot = 'D:\Tools\vc71'
$windowsSdkLibRoot = 'C:\Program Files (x86)\Windows Kits\10\Lib'
$outDir = Join-Path $rebuildRoot "build\bootstrap-$Configuration"
$crcCalcSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_CCRC_Calc_004014a0.cpp'
$crcCalcBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_CCRC_Calc_004014a0_test.cpp'
$bootstrapSource = Join-Path $rebuildRoot 'integration\bootstrap_main.cpp'
$retailSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_MemCmpUnsigned16_00403c60.cpp'
$winMainSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_WinMain_00403480.cpp'
$winMainBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_WinMain_00403480_test.cpp'
$gfInitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\40\global_GFInitialise_004022b0.cpp'
$gfInitialiseBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\global_GFInitialise_004022b0_test.cpp'
$gfUninitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_GFUninitialise_00401b80.cpp'
$gfUninitialiseBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_GFUninitialise_00401b80_test.cpp'
$gfHandleSystemInitErrorSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_GFHandleSystemInitError_00401c00.cpp'
$gfHandleSystemInitErrorBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_GFHandleSystemInitError_00401c00_test.cpp'
$gamePlaySource = Join-Path $rebuildRoot 'src\compiled\00\41\CGame_Play_00412f90.cpp'
$gamePlayBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\CGame_Play_00412f90_test.cpp'
$initialiseConsoleVariablesSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_InitialiseConsoleVariables_00413c50.cpp'
$initialiseConsoleVariablesBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_InitialiseConsoleVariables_00413c50_test.cpp'
$getWindowTitleSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_GetWindowTitle_004023f0.cpp'
$getWindowTitleBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_GetWindowTitle_004023f0_test.cpp'
$imeInitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\40\CIME_Initialise_00405650.cpp'
$imeInitialiseBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CIME_Initialise_00405650_test.cpp'
$generateMetFilesSource = Join-Path $rebuildRoot 'src\compiled\00\41\CMainGameComponent_GenerateMetFilesFromLugFiles_00418c3b.cpp'
$generateMetFilesBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\CMainGameComponent_GenerateMetFilesFromLugFiles_00418c3b_test.cpp'
$freeConfigDetectionSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_GFFreeConfigDetection_00401b20.cpp'
$freeConfigDetectionBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_GFFreeConfigDetection_00401b20_test.cpp'
$systemManagerInitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CSystemManager_Initialise_009a6610.cpp'
$systemManagerInitialiseBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CSystemManager_Initialise_009a6610_test.cpp'
$exitThunkSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_Exit_00401284.cpp'
$exitThunkBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_Exit_00401284_test.cpp'
$doEulaSource = Join-Path $rebuildRoot 'src\compiled\00\40\EULA_DoEULAThings_00401fe0.cpp'
$doEulaBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\EULA_DoEULAThings_00401fe0_test.cpp'
$configDetectionSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_GFConfigDetection_004017b0.cpp'
$configDetectionBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_GFConfigDetection_004017b0_test.cpp'
$persistSignedCharSource = Join-Path $rebuildRoot 'src\compiled\00\40\CPersistContext_TransferSignedChar_004045c0.cpp'
$persistSignedCharBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CPersistContext_TransferSignedChar_004045c0_test.cpp'
$stringParserCommentListDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\40\CStringParserCommentList_Destructor_004046b0.cpp'
$stringParserCommentListDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CStringParserCommentList_Destructor_004046b0_test.cpp'
$imeSettingsSource = Join-Path $rebuildRoot 'src\compiled\00\40\CIME_LoadIMESettings_00405350.cpp'
$imeSettingsBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CIME_LoadIMESettings_00405350_test.cpp'
$bankHeaderDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\9a\NBankFileManager_SetHeaderDir_009a76c0.cpp'
$bankHeaderDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\NBankFileManager_SetHeaderDir_009a76c0_test.cpp'
$myDocumentsDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\NResourceDirectoryNames_GetMyDocumentsDir_0041a430.cpp'
$myDocumentsDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\NResourceDirectoryNames_GetMyDocumentsDir_0041a430_test.cpp'
$saveDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\NResourceDirectoryNames_GetSaveDir_0041a540.cpp'
$saveDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\NResourceDirectoryNames_GetSaveDir_0041a540_test.cpp'
$physicsFacesCountedResetSource = Join-Path $rebuildRoot 'src\compiled\00\40\CCountedPointer_PhysicsFacesSegment_Reset_00403e40.cpp'
$physicsFacesCountedResetBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CCountedPointer_PhysicsFacesSegment_Reset_00403e40_test.cpp'
$wideStringAssignLiteralSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_AssignLiteral_0099b800.cpp'
$wideStringAssignLiteralBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_AssignLiteral_0099b800_test.cpp'
$wideStringAddWideSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_AddWideStrings_0099be70.cpp'
$wideStringAddWideBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_AddWideStrings_0099be70_test.cpp'
$defStringTableLoadSource = Join-Path $rebuildRoot 'src\compiled\00\9d\CDefStringTable_LoadTable_009d4c30.cpp'
$defStringTableLoadBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\CDefStringTable_LoadTable_009d4c30_test.cpp'
$gameTextDataBankConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\9c\NGameText_CDataBank_Constructor_009c85a0.cpp'
$gameTextDataBankConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\9c\NGameText_CDataBank_Constructor_009c85a0_test.cpp'
$gameTextGetGroupEntrySource = Join-Path $rebuildRoot 'src\compiled\00\9c\NGameText_CDataBank_GetPGroupEntry_009c9070.cpp'
$gameTextGetGroupEntryBehaviorSource = Join-Path $rebuildRoot 'tests\00\9c\NGameText_CDataBank_GetPGroupEntry_009c9070_test.cpp'
$myDocumentsWritePermissionsSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_MyDocuments_CheckWritePermissions_00402130.cpp'
$myDocumentsWritePermissionsBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_MyDocuments_CheckWritePermissions_00402130_test.cpp'
$retailBankConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\9a\NBankFileManager_CRetailBank_Constructor_009ac4d0.cpp'
$retailBankConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\NBankFileManager_CRetailBank_Constructor_009ac4d0_test.cpp'
$threadedFileConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\98\CThreadedFile_Constructor_0098dfd0.cpp'
$threadedFileConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\98\CThreadedFile_Constructor_0098dfd0_test.cpp'
$threadedFileCountedResetSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CCountedPointer_CThreadedFile_Reset_009a9c80.cpp'
$threadedFileCountedResetBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CCountedPointer_CThreadedFile_Reset_009a9c80_test.cpp'
$threadedFileOpenSource = Join-Path $rebuildRoot 'src\compiled\00\98\CThreadedFile_Open_0098e1e0.cpp'
$threadedFileOpenBehaviorSource = Join-Path $rebuildRoot 'tests\00\98\CThreadedFile_Open_0098e1e0_test.cpp'
$containedBankMapIndexSource = Join-Path $rebuildRoot 'src\compiled\00\9a\NBankFileManager_CContainedBankMap_Index_009ac530.cpp'
$containedBankMapIndexBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\NBankFileManager_CContainedBankMap_Index_009ac530_test.cpp'
$openRetailBankSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CBankFileManager_OpenRetailBank_009a8840.cpp'
$openRetailBankBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CBankFileManager_OpenRetailBank_009a8840_test.cpp'
$openIniFileSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CBankFileManager_OpenIniFile_009a8170.cpp'
$openIniFileBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CBankFileManager_OpenIniFile_009a8170_test.cpp'
$userProfileSingletonGetSource = Join-Path $rebuildRoot 'src\compiled\00\40\CUserProfileManagerSingleton_Get_0040d2a0.cpp'
$userProfileSingletonGetBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CUserProfileManagerSingleton_Get_0040d2a0_test.cpp'
$userProfileSetProfileNameSource = Join-Path $rebuildRoot 'src\compiled\00\40\CUserProfileManager_SetProfileName_0040d400.cpp'
$userProfileSetProfileNameBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CUserProfileManager_SetProfileName_0040d400_test.cpp'
$stage1BoundarySource = Join-Path $rebuildRoot 'integration\stage1_engine_boundary.cpp'
$progressSetupSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GFInitialiseSetupProgressDisplay_00413120.cpp'
$progressSetupBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GFInitialiseSetupProgressDisplay_00413120_test.cpp'
$progressDisplayConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\49\CProgressDisplay_Constructor_00499ce0.cpp'
$progressDisplayConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\49\CProgressDisplay_Constructor_00499ce0_test.cpp'
$progressDisplayTextBankSource = Join-Path $rebuildRoot 'src\compiled\00\49\CProgressDisplay_GetPTextBank_00497b30.cpp'
$progressDisplayTextBankBehaviorSource = Join-Path $rebuildRoot 'tests\00\49\CProgressDisplay_GetPTextBank_00497b30_test.cpp'
$progressDisplayTextModeSource = Join-Path $rebuildRoot 'src\compiled\00\49\CProgressDisplay_SetToDisplayText_00499a70.cpp'
$progressDisplayTextModeBehaviorSource = Join-Path $rebuildRoot 'tests\00\49\CProgressDisplay_SetToDisplayText_00499a70_test.cpp'
$progressDisplayIsActiveSource = Join-Path $rebuildRoot 'src\compiled\00\49\CProgressDisplay_IsActive_0049b460.cpp'
$progressDisplayIsActiveBehaviorSource = Join-Path $rebuildRoot 'tests\00\49\CProgressDisplay_IsActive_0049b460_test.cpp'
$setProgressDisplaySource = Join-Path $rebuildRoot 'src\compiled\00\9e\Global_SetProgressDisplay_009e9fd0.cpp'
$setProgressDisplayBehaviorSource = Join-Path $rebuildRoot 'tests\00\9e\Global_SetProgressDisplay_009e9fd0_test.cpp'
$getProgressDisplaySource = Join-Path $rebuildRoot 'src\compiled\00\9e\Global_GetProgressDisplay_009ea060.cpp'
$getProgressDisplayBehaviorSource = Join-Path $rebuildRoot 'tests\00\9e\Global_GetProgressDisplay_009ea060_test.cpp'
$setCurrentPathSource = Join-Path $rebuildRoot 'src\compiled\00\99\CAFile_SetCurrentPath_009974f0.cpp'
$setCurrentPathBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CAFile_SetCurrentPath_009974f0_test.cpp'
$getProjectPathSource = Join-Path $rebuildRoot 'src\compiled\00\99\CAFile_GetProjectPath_00997510.cpp'
$getProjectPathBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CAFile_GetProjectPath_00997510_test.cpp'
$wideStringConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_Constructor_0099aed0.cpp'
$wideStringConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_Constructor_0099aed0_test.cpp'
$wideStringDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_Destructor_0099b510.cpp'
$wideStringDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_Destructor_0099b510_test.cpp'
$wideStringConversionSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_ConversionOperator_0099af10.cpp'
$wideStringConversionBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_ConversionOperator_0099af10_test.cpp'
$wideStringNotEqualSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_NotEqualWide_0099b150.cpp'
$wideStringNotEqualBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_NotEqualWide_0099b150_test.cpp'
$systemRegistryConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CSystemRegistry_Constructor_0099cf10.cpp'
$systemRegistryConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CSystemRegistry_Constructor_0099cf10_test.cpp'
$systemRegistryDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CSystemRegistry_Destructor_0099cf70.cpp'
$systemRegistryDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CSystemRegistry_Destructor_0099cf70_test.cpp'
$systemRegistryWriteBoolSource = Join-Path $rebuildRoot 'src\compiled\00\99\CSystemRegistry_WriteBOOL_0099d100.cpp'
$systemRegistryWriteBoolBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CSystemRegistry_WriteBOOL_0099d100_test.cpp'
$wideLiteralConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_LiteralConstructor_0099b6b0.cpp'
$wideLiteralConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_LiteralConstructor_0099b6b0_test.cpp'
$wideStringCopyAssignmentSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_CopyAssignment_0099b7d0.cpp'
$wideStringCopyAssignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_CopyAssignment_0099b7d0_test.cpp'
$charStringConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_Constructor_0099ebf0.cpp'
$charStringConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_Constructor_0099ebf0_test.cpp'
$charStringDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_Destructor_0099eae0.cpp'
$charStringDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_Destructor_0099eae0_test.cpp'
$charStringCopyAssignmentSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_CopyAssignment_0099efb0.cpp'
$charStringCopyAssignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_CopyAssignment_0099efb0_test.cpp'
$retailBankNavigatorSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CBasicRetailBankManager_SetNavigatorEnabled_009a76d0.cpp'
$retailBankNavigatorBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CBasicRetailBankManager_SetNavigatorEnabled_009a76d0_test.cpp'
$retailBankAliasSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CBasicRetailBankManager_AddBankAlias_009a8150.cpp'
$retailBankAliasBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CBasicRetailBankManager_AddBankAlias_009a8150_test.cpp'
$getSystemManagerSource = Join-Path $rebuildRoot 'src\compiled\00\9a\Global_GFGetSystemManager_009a4ec0.cpp'
$getSystemManagerBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\Global_GFGetSystemManager_009a4ec0_test.cpp'
$movieLeftAlignSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CXMVPlayer_SetLeftAlignText_009a0ef0.cpp'
$movieLeftAlignBehaviorSource = Join-Path $rebuildRoot 'tests\00\9a\CXMVPlayer_SetLeftAlignText_009a0ef0_test.cpp'
$imePrimitiveSource = Join-Path $rebuildRoot 'src\compiled\00\40\CEngineInternalPrimitiveBase_AddChildPrimitive_00404a80.cpp'
$imePrimitiveBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CEngineInternalPrimitiveBase_AddChildPrimitive_00404a80_test.cpp'
$profileStartSource = Join-Path $rebuildRoot 'src\compiled\00\9d\NProfileTimer_StartProfile_009d8240.cpp'
$profileStartBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\NProfileTimer_StartProfile_009d8240_test.cpp'
$profileEndSource = Join-Path $rebuildRoot 'src\compiled\00\9d\NProfileTimer_EndProfile_009d8250.cpp'
$profileEndBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\NProfileTimer_EndProfile_009d8250_test.cpp'
$asyncFailureHandlingSource = Join-Path $rebuildRoot 'src\compiled\00\9d\CBankFileAsync_SetEnableFailureHandling_009d5240.cpp'
$asyncFailureHandlingBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\CBankFileAsync_SetEnableFailureHandling_009d5240_test.cpp'
$startupLatchSource = Join-Path $rebuildRoot 'src\compiled\00\9d\Global_ClearStartupLatch_009d81e0.cpp'
$startupLatchBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\Global_ClearStartupLatch_009d81e0_test.cpp'
$fileInstallerGetSource = Join-Path $rebuildRoot 'src\compiled\00\40\CFileInstallerSingleton_Get_00404440.cpp'
$fileInstallerGetBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CFileInstallerSingleton_Get_00404440_test.cpp'
$defaultLanguageNameSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetDefaultLanguageName_00415530.cpp'
$defaultLanguageNameBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetDefaultLanguageName_00415530_test.cpp'
$fontBankNameSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetFontBankName_00415260.cpp'
$fontBankNameBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetFontBankName_00415260_test.cpp'
$streamingFontBankNameSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetStreamingFontBankName_00415440.cpp'
$streamingFontBankNameBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetStreamingFontBankName_00415440_test.cpp'
$miscDirectoryASource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetMiscDirectoryA_0041a0a0.cpp'
$miscDirectoryABehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetMiscDirectoryA_0041a0a0_test.cpp'
$miscDirectoryBSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetMiscDirectoryB_0041a0c0.cpp'
$miscDirectoryBBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetMiscDirectoryB_0041a0c0_test.cpp'
$graphicsDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetGraphicsDirectory_0041a100.cpp'
$graphicsDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetGraphicsDirectory_0041a100_test.cpp'
$languageDirectoryASource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetLanguageDirectoryA_0041a120.cpp'
$languageDirectoryABehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetLanguageDirectoryA_0041a120_test.cpp'
$shadersDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetShadersDirectory_0041a1e0.cpp'
$shadersDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetShadersDirectory_0041a1e0_test.cpp'
$cacheDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetCacheDirectory_0041a180.cpp'
$cacheDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetCacheDirectory_0041a180_test.cpp'
$languageDirectoryBSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetLanguageDirectoryB_0041a330.cpp'
$languageDirectoryBBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetLanguageDirectoryB_0041a330_test.cpp'
$checkpointDirectorySource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetAutoSaveCheckpointDirectory_0041a4c0.cpp'
$checkpointDirectoryBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetAutoSaveCheckpointDirectory_0041a4c0_test.cpp'
$defTablePathSource = Join-Path $rebuildRoot 'src\compiled\00\9d\CDefStringTable_SetTablePath_009d4a40.cpp'
$defTablePathBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\CDefStringTable_SetTablePath_009d4a40_test.cpp'
$bankHandleSource = Join-Path $rebuildRoot 'src\compiled\00\9c\CBankFile_GetBankHandle_009c7a40.cpp'
$bankHandleBehaviorSource = Join-Path $rebuildRoot 'tests\00\9c\CBankFile_GetBankHandle_009c7a40_test.cpp'
$vectorMapLookupSource = Join-Path $rebuildRoot 'src\compiled\00\9c\FableVectorMap_LookupValue_009cc410.cpp'
$vectorMapLookupBehaviorSource = Join-Path $rebuildRoot 'tests\00\9c\FableVectorMap_LookupValue_009cc410_test.cpp'
$wideStringFromCharSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_CreateFromCharString_0099b6a0.cpp'
$wideStringFromCharBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_CreateFromCharString_0099b6a0_test.cpp'
$wideStringCopySource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_CopyConstructor_0099b720.cpp'
$wideStringCopyBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_CopyConstructor_0099b720_test.cpp'
$wideStringAddRightSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_AddWideStringAndLiteral_0099bf30.cpp'
$wideStringAddRightBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_AddWideStringAndLiteral_0099bf30_test.cpp'
$wideStringAddLeftSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_AddLiteralAndWideString_0099bff0.cpp'
$wideStringAddLeftBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_AddLiteralAndWideString_0099bff0_test.cpp'
$pathExistsSource = Join-Path $rebuildRoot 'src\compiled\00\99\CAFile_PathExists_00999230.cpp'
$pathExistsBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CAFile_PathExists_00999230_test.cpp'
$diskFileConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CDiskFileWin32_Constructor_0099AD80.cpp'
$diskFileConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CDiskFileWin32_Constructor_0099AD80_test.cpp'
$diskFileCloseSource = Join-Path $rebuildRoot 'src\compiled\00\99\CDiskFileWin32_Close_0099A920.cpp'
$diskFileCloseBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CDiskFileWin32_Close_0099A920_test.cpp'
$charStringLoadFileSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_LoadFromFile_0099f2e0.cpp'
$charStringLoadFileBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_LoadFromFile_0099f2e0_test.cpp'
$stringParserConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\40\CStringParser_Constructor_00404720.cpp'
$stringParserConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CStringParser_Constructor_00404720_test.cpp'
$persistContextTextConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\9b\CPersistContext_TextConstructor_009badd0.cpp'
$persistContextTextConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\9b\CPersistContext_TextConstructor_009badd0_test.cpp'
$charToWideStringSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_ToWideString_0099e480.cpp'
$charToWideStringBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_ToWideString_0099e480_test.cpp'
$primaryLeftAlignmentSource = Join-Path $rebuildRoot 'src\compiled\00\9b\TextLayout_SetPrimaryLeftAlignment_009bc890.cpp'
$primaryLeftAlignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\9b\TextLayout_SetPrimaryLeftAlignment_009bc890_test.cpp'
$secondaryLeftAlignmentSource = Join-Path $rebuildRoot 'src\compiled\00\9b\TextLayout_SetSecondaryLeftAlignment_009bc8a0.cpp'
$secondaryLeftAlignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\9b\TextLayout_SetSecondaryLeftAlignment_009bc8a0_test.cpp'
$cbaseRestoreASource = Join-Path $rebuildRoot 'src\compiled\00\99\CBase_RestoreVTable_0099a2e0.cpp'
$cbaseRestoreABehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CBase_RestoreVTable_0099a2e0_test.cpp'
$cbaseRestoreBSource = Join-Path $rebuildRoot 'src\compiled\00\99\CBase_RestoreVTable_0099a300.cpp'
$cbaseRestoreBBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CBase_RestoreVTable_0099a300_test.cpp'
$cmovieIsPlayingSource = Join-Path $rebuildRoot 'src\compiled\00\54\CMovie_IsPlaying_00548520.cpp'
$cmovieIsPlayingBehaviorSource = Join-Path $rebuildRoot 'tests\00\54\CMovie_IsPlaying_00548520_test.cpp'
$cmovieSetMovieSource = Join-Path $rebuildRoot 'src\compiled\00\54\CMovie_SetMovie_00548510.cpp'
$cmovieSetMovieBehaviorSource = Join-Path $rebuildRoot 'tests\00\54\CMovie_SetMovie_00548510_test.cpp'
$videoSysGetTextureSource = Join-Path $rebuildRoot 'src\compiled\00\a3\CVideoSys_GetTexture_00a3b320.cpp'
$videoSysGetTextureBehaviorSource = Join-Path $rebuildRoot 'tests\00\a3\CVideoSys_GetTexture_00a3b320_test.cpp'
$videoSysAttemptToPlaySource = Join-Path $rebuildRoot 'src\compiled\00\a3\CVideoSys_AttemptToPlay_00a3b1a0.cpp'
$videoSysAttemptToPlayBehaviorSource = Join-Path $rebuildRoot 'tests\00\a3\CVideoSys_AttemptToPlay_00a3b1a0_test.cpp'
$videoSysWaitForStateSource = Join-Path $rebuildRoot 'src\compiled\00\a3\CVideoSys_WaitForState_00a3b0f0.cpp'
$videoSysWaitForStateBehaviorSource = Join-Path $rebuildRoot 'tests\00\a3\CVideoSys_WaitForState_00a3b0f0_test.cpp'
$videoSysPauseSource = Join-Path $rebuildRoot 'src\compiled\00\a3\CVideoSys_Pause_00a3b1c0.cpp'
$videoSysPauseBehaviorSource = Join-Path $rebuildRoot 'tests\00\a3\CVideoSys_Pause_00a3b1c0_test.cpp'
$videoSysStopSource = Join-Path $rebuildRoot 'src\compiled\00\a3\CVideoSys_Stop_00a3b1f0.cpp'
$videoSysStopBehaviorSource = Join-Path $rebuildRoot 'tests\00\a3\CVideoSys_Stop_00a3b1f0_test.cpp'
$charStringDefaultSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_DefaultConstructor_0099e4b0.cpp'
$charStringDefaultBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_DefaultConstructor_0099e4b0_test.cpp'
$systemManagerInitSource = Join-Path $rebuildRoot 'src\compiled\00\40\CSystemManagerInit_Constructor_00403b10.cpp'
$systemManagerInitBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CSystemManagerInit_Constructor_00403b10_test.cpp'
$gfmainPhase1Source = Join-Path $rebuildRoot 'integration\gfmain_phase1.cpp'
$gfmainPhase2Source = Join-Path $rebuildRoot 'integration\gfmain_phase2.cpp'
$gfmainPhase3Source = Join-Path $rebuildRoot 'integration\gfmain_phase3.cpp'
$gfmainPhase4Source = Join-Path $rebuildRoot 'integration\gfmain_phase4.cpp'
$gfmainPhase5Source = Join-Path $rebuildRoot 'integration\gfmain_phase5.cpp'
$gfmainPhase6Source = Join-Path $rebuildRoot 'integration\gfmain_phase6.cpp'
$gfmainPhase7Source = Join-Path $rebuildRoot 'integration\gfmain_phase7.cpp'
$gfmainPhase8Source = Join-Path $rebuildRoot 'integration\gfmain_phase8.cpp'
$gfmainPhase9Source = Join-Path $rebuildRoot 'integration\gfmain_phase9.cpp'
$gfmainPhase10Source = Join-Path $rebuildRoot 'integration\gfmain_phase10.cpp'
$gfmainCompleteSource = Join-Path $rebuildRoot 'integration\gfmain_complete.cpp'
$gfInitialiseProgressPhaseSource = Join-Path $rebuildRoot 'integration\gfinitialise_progress_phase.cpp'
$gfInitialiseEngineBoundarySource = Join-Path $rebuildRoot 'integration\gfinitialise_engine_boundary.cpp'
$phase6RuntimeBoundarySource =
    Join-Path $rebuildRoot 'integration\phase6_runtime_boundary.cpp'
$systemManagerRuntimeBoundarySource =
    Join-Path $rebuildRoot 'integration\system_manager_runtime_boundary.cpp'
$saveMetadataRuntimeBoundarySource =
    Join-Path $rebuildRoot 'integration\save_metadata_runtime_boundary.cpp'
$phase8RuntimeBoundarySource =
    Join-Path $rebuildRoot 'integration\phase8_runtime_boundary.cpp'
$phase7WindowTitleRuntimeBoundarySource =
    Join-Path $rebuildRoot 'integration\phase7_window_title_runtime_boundary.cpp'
$progressDisplayStringBoundarySource = Join-Path $rebuildRoot 'integration\progress_display_string_boundary.cpp'
$stage2BoundarySource = Join-Path $rebuildRoot 'integration\stage2_engine_boundary.cpp'
$visualBootSource = Join-Path $rebuildRoot 'integration\visual_boot_checkpoint.cpp'
$visualBootD3D9Source = Join-Path $rebuildRoot 'integration\visual_boot_d3d9.cpp'
$retailVideoBridgeSource = Join-Path $rebuildRoot 'integration\retail_video_bridge.cpp'
$videoFrameConversionSource = Join-Path $rebuildRoot 'integration\video_frame_conversion.cpp'
$videoFramePublicationSource = Join-Path $rebuildRoot 'integration\video_frame_publication.cpp'
$videoFramePublicationBehaviorSource = Join-Path $rebuildRoot 'tests\integration\VideoFramePublication_test.cpp'
$frontendStartupSequenceSource = Join-Path $rebuildRoot 'integration\frontend_startup_sequence.cpp'
$frontendStartupSequenceBehaviorSource = Join-Path $rebuildRoot 'tests\integration\FrontendStartupSequence_test.cpp'
$visualBootFallbackArtwork = Join-Path $rebuildRoot 'assets\boot\fabledecomp_boot_concept.png'
$visualBootArtwork = $visualBootFallbackArtwork
$textureBuilder = Join-Path $workspaceRoot 'tools\texture_build.py'
$staticFontRenderer = Join-Path $workspaceRoot 'tools\render_fable_static_font.py'
$streamingFontRenderer = Join-Path $workspaceRoot 'tools\render_fable_streaming_font.py'
$cursorBuilder = Join-Path $workspaceRoot 'tools\png_to_windows_cursor.py'
$frontendAnimationRenderer = Join-Path $workspaceRoot `
    'tools\render_fable_frontend_animation.py'
$frontendMenuRenderer = Join-Path $workspaceRoot `
    'tools\render_fable_frontend_menu.py'
$frontendSubscreenRenderer = Join-Path $workspaceRoot `
    'tools\render_fable_frontend_subscreens.py'
$frontendSoundExtractor = Join-Path $workspaceRoot 'tools\parse_lug.py'
$visualBootBehaviorSource = Join-Path $rebuildRoot 'tests\integration\VisualBootCheckpoint_test.cpp'
$render2DBatchPlanSource = Join-Path $rebuildRoot 'integration\render2d_batch_plan.cpp'
$render2DBatchPlanBehaviorSource = Join-Path $rebuildRoot 'tests\integration\Render2DBatchPlan_test.cpp'
$render2DDrawListAdapterSource = Join-Path $rebuildRoot 'integration\render2d_draw_list_adapter.cpp'
$render2DDrawListAdapterBehaviorSource = Join-Path $rebuildRoot 'tests\integration\Render2DDrawListAdapter_test.cpp'
$attachTextureToStageSource = Join-Path $rebuildRoot 'src\compiled\00\9a\CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp'
$realiseRenderStateSource = Join-Path $rebuildRoot 'src\compiled\00\a0\CRenderStateManager_RealiseRenderState_00a058c0.cpp'
$soldStateBlockSource = Join-Path $rebuildRoot 'src\compiled\00\9d\CStateBlockFunctionSold_Apply_009df060.cpp'
$updatePixelShaderSource = Join-Path $rebuildRoot 'src\compiled\00\98\CShaderRenderManager_UpdatePixelShader_00988a20.cpp'
$resetWorldTransformSource = Join-Path $rebuildRoot 'src\compiled\00\98\CShaderRenderManager_ResetWorldTransform_00988290.cpp'
$displaySetViewportSource = Join-Path $rebuildRoot 'src\compiled\00\9b\CDisplayManager_SetViewport_009bf490.cpp'
$displaySetIntegerViewportSource = Join-Path $rebuildRoot 'src\compiled\00\9b\CDisplayManager_SetViewportInteger_009bef80.cpp'
$postViewportShaderSource = Join-Path $rebuildRoot 'src\compiled\00\98\CShaderRenderManager_OnPostViewportChanged_009880d0.cpp'
$viewportE2Source = Join-Path $rebuildRoot 'src\compiled\00\a0\Global_FableViewportE2_00a0aac0.cpp'
$setRenderWindowSource = Join-Path $rebuildRoot 'src\compiled\00\a0\CRenderManagerCore_SetAWindow_00a0aa80.cpp'
$clearRender2DVertexQueueSource = Join-Path $rebuildRoot 'src\compiled\00\9e\Render2DDrawList_CopyBlock_009e1440.cpp'
$restoreRenderStateCaptureSource = Join-Path $rebuildRoot 'src\compiled\00\a0\CRenderStateManager_RestoreCaptureBlock_00a05840.cpp'
$textureCalcByteLengthSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CTexture_CalcByteLength_009f9ee0.cpp'
$pixelFormatGetColourDepthSource = Join-Path $rebuildRoot 'src\compiled\00\9e\CPixelFormat_GetColourDepth_009e3820.cpp'
$pixelFormatInitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\9e\CPixelFormat_InitialiseD3DFormat_009e3830.cpp'
$pixelFormatTableSource = Join-Path $rebuildRoot 'src\compiled\01\29\CPixelFormat_Table_0129ba40.cpp'
$textureInitialisePreallocatedSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CTexture_InitialiseFromPreallocatedTexture_009fa230.cpp'
$textureAssignmentSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CTexture_Assignment_009fa1c0.cpp'
$textureUninitialiseSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CTexture_Uninitialise_009f9f70.cpp'
$surfaceCopyConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CSurface_CopyConstructor_009f2d60.cpp'
$surfaceCopyConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\9f\CSurface_CopyConstructor_009f2d60_test.cpp'
$surfaceInitialiseMipmapSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CSurface_InitialiseFromTextureMipmap_009f2f10.cpp'
$surfaceInitialiseMipmapBehaviorSource = Join-Path $rebuildRoot 'tests\00\9f\CSurface_InitialiseFromTextureMipmap_009f2f10_test.cpp'
$surfaceCopyFromSurfaceSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CSurface_CopyFromSurface_009f37e0.cpp'
$surfaceCopyFromSurfaceBehaviorSource = Join-Path $rebuildRoot 'tests\00\9f\CSurface_CopyFromSurface_009f37e0_test.cpp'
$textureCopyFromTextureSource = Join-Path $rebuildRoot 'src\compiled\00\9f\CTexture_CopyFromTexture_009fa4e0.cpp'
$textureCopyFromTextureBehaviorSource = Join-Path $rebuildRoot 'tests\00\9f\CTexture_CopyFromTexture_009fa4e0_test.cpp'
$gfmainPhase1BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase1_test.cpp'
$gfmainPhase2BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase2_test.cpp'
$gfmainPhase3BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase3_test.cpp'
$gfmainPhase4BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase4_test.cpp'
$gfmainPhase5BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase5_test.cpp'
$gfmainPhase6BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase6_test.cpp'
$gfmainPhase7BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase7_test.cpp'
$gfmainPhase8BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase8_test.cpp'
$gfmainPhase9BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase9_test.cpp'
$gfmainPhase10BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase10_test.cpp'
$gfmainCompleteBehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Complete_test.cpp'
$gfInitialiseProgressPhaseBehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFInitialise_ProgressPhase_test.cpp'
$bootObjectChecker = Join-Path $workspaceRoot 'tools\check_boot_object.py'
$bootstrapObject = Join-Path $outDir 'bootstrap_main.obj'
$retailObject = Join-Path $outDir 'retail_00403c60.obj'
$winMainObject = Join-Path $outDir 'retail_winmain.obj'
$winMainBehaviorObject = Join-Path $outDir 'winmain_behavior.obj'
$gfInitialiseObject = Join-Path $outDir 'gfinitialise.obj'
$gfUninitialiseObject = Join-Path $outDir 'gf-uninitialise.obj'
$imeInitialiseObject = Join-Path $outDir 'cime-initialise.obj'
$systemManagerInitialiseObject =
    Join-Path $outDir 'system-manager-initialise.obj'
$stage1BoundaryObject = Join-Path $outDir 'stage1_engine_boundary.obj'
$progressSetupObject = Join-Path $outDir 'gfinitialise_setup_progress.obj'
$progressSetupBehaviorObject = Join-Path $outDir 'gfinitialise_setup_progress_behavior.obj'
$progressDisplayConstructorObject = Join-Path $outDir 'progress-display-constructor.obj'
$progressDisplayTextModeObject = Join-Path $outDir 'progress-display-text-mode.obj'
$progressDisplayIsActiveObject = Join-Path $outDir 'progress-display-is-active.obj'
$setProgressDisplayObject = Join-Path $outDir 'set-progress-display.obj'
$getProgressDisplayObject = Join-Path $outDir 'get-progress-display.obj'
$progressDisplayStringBoundaryObject = Join-Path $outDir 'progress-display-string-boundary.obj'
$setCurrentPathObject = Join-Path $outDir 'set_current_path.obj'
$setCurrentPathBehaviorObject = Join-Path $outDir 'set_current_path_behavior.obj'
$getProjectPathObject = Join-Path $outDir 'get_project_path.obj'
$getProjectPathBehaviorObject = Join-Path $outDir 'get_project_path_behavior.obj'
$wideStringConstructorObject = Join-Path $outDir 'wide_string_constructor.obj'
$wideStringConstructorBehaviorObject = Join-Path $outDir 'wide_string_constructor_behavior.obj'
$wideStringDestructorObject = Join-Path $outDir 'wide_string_destructor.obj'
$wideStringConversionObject = Join-Path $outDir 'wide-string-conversion.obj'
$wideLiteralConstructorObject = Join-Path $outDir 'wide-literal-constructor.obj'
$wideStringCopyAssignmentObject = Join-Path $outDir 'wide-string-copy-assignment.obj'
$wideStringDestructorBehaviorObject = Join-Path $outDir 'wide_string_destructor_behavior.obj'
$charStringConstructorObject = Join-Path $outDir 'char_string_constructor.obj'
$charStringConstructorBehaviorObject = Join-Path $outDir 'char_string_constructor_behavior.obj'
$charStringDestructorObject = Join-Path $outDir 'char_string_destructor.obj'
$charStringDestructorBehaviorObject = Join-Path $outDir 'char_string_destructor_behavior.obj'
$charStringCopyAssignmentObject = Join-Path $outDir 'char-string-copy-assignment.obj'
$retailBankNavigatorObject = Join-Path $outDir 'retail-bank-navigator.obj'
$retailBankAliasObject = Join-Path $outDir 'retail-bank-alias.obj'
$getSystemManagerObject = Join-Path $outDir 'get-system-manager.obj'
$movieLeftAlignObject = Join-Path $outDir 'movie-left-align.obj'
$imePrimitiveObject = Join-Path $outDir 'ime-primitive.obj'
$crcCalcObject = Join-Path $outDir 'crc-calc.obj'
$profileStartObject = Join-Path $outDir 'profile_start.obj'
$profileStartBehaviorObject = Join-Path $outDir 'profile_start_behavior.obj'
$defaultLanguageNameObject = Join-Path $outDir 'default-language-name.obj'
$fontBankNameObject = Join-Path $outDir 'font-bank-name.obj'
$streamingFontBankNameObject = Join-Path $outDir 'streaming-font-bank-name.obj'
$miscDirectoryAObject = Join-Path $outDir 'misc-directory-a.obj'
$miscDirectoryBObject = Join-Path $outDir 'misc-directory-b.obj'
$graphicsDirectoryObject = Join-Path $outDir 'graphics-directory.obj'
$languageDirectoryAObject = Join-Path $outDir 'language-directory-a.obj'
$shadersDirectoryObject = Join-Path $outDir 'shaders-directory.obj'
$cacheDirectoryObject = Join-Path $outDir 'cache-directory.obj'
$languageDirectoryBObject = Join-Path $outDir 'language-directory-b.obj'
$checkpointDirectoryObject = Join-Path $outDir 'checkpoint-directory.obj'
$defTablePathObject = Join-Path $outDir 'def-table-path.obj'
$defStringTableLoadObject =
    Join-Path $outDir 'definition-string-table-load.obj'
$myDocumentsWritePermissionsObject =
    Join-Path $outDir 'my-documents-write-permissions.obj'
$bankHandleObject = Join-Path $outDir 'bank-handle.obj'
$vectorMapLookupObject = Join-Path $outDir 'vector-map-lookup.obj'
$wideStringFromCharObject = Join-Path $outDir 'wide-string-from-char.obj'
$wideStringCopyObject = Join-Path $outDir 'wide-string-copy.obj'
$charToWideStringObject = Join-Path $outDir 'char-to-wide-string.obj'
$wideStringAddRightObject = Join-Path $outDir 'wide-string-add-right.obj'
$wideStringAddLeftObject = Join-Path $outDir 'wide-string-add-left.obj'
$primaryLeftAlignmentObject = Join-Path $outDir 'primary-left-alignment.obj'
$secondaryLeftAlignmentObject = Join-Path $outDir 'secondary-left-alignment.obj'
$charStringDefaultObject = Join-Path $outDir 'char-string-default-constructor.obj'
$systemManagerInitObject = Join-Path $outDir 'system-manager-init-constructor.obj'
$gfmainPhase1Object = Join-Path $outDir 'gfmain_phase1.obj'
$gfmainPhase2Object = Join-Path $outDir 'gfmain_phase2.obj'
$gfmainPhase3Object = Join-Path $outDir 'gfmain_phase3.obj'
$gfmainPhase4Object = Join-Path $outDir 'gfmain_phase4.obj'
$gfmainPhase5Object = Join-Path $outDir 'gfmain_phase5.obj'
$gfmainPhase6Object = Join-Path $outDir 'gfmain_phase6.obj'
$gfmainPhase7Object = Join-Path $outDir 'gfmain_phase7.obj'
$gfmainPhase8Object = Join-Path $outDir 'gfmain_phase8.obj'
$gfmainPhase9Object = Join-Path $outDir 'gfmain_phase9.obj'
$gfmainPhase10Object = Join-Path $outDir 'gfmain_phase10.obj'
$gfmainCompleteObject = Join-Path $outDir 'gfmain_complete.obj'
$gfInitialiseProgressPhaseObject = Join-Path $outDir 'gfinitialise_progress_phase.obj'
$gfInitialiseEngineBoundaryObject = Join-Path $outDir 'gfinitialise_engine_boundary.obj'
$phase6RuntimeBoundaryObject =
    Join-Path $outDir 'phase6_runtime_boundary.obj'
$systemManagerRuntimeBoundaryObject =
    Join-Path $outDir 'system_manager_runtime_boundary.obj'
$saveMetadataRuntimeBoundaryObject =
    Join-Path $outDir 'save_metadata_runtime_boundary.obj'
$generateMetFilesObject = Join-Path $outDir 'generate-met-files.obj'
$phase8RuntimeBoundaryObject =
    Join-Path $outDir 'phase8_runtime_boundary.obj'
$doEulaObject = Join-Path $outDir 'do-eula-things.obj'
$configDetectionObject = Join-Path $outDir 'gf-config-detection.obj'
$freeConfigDetectionObject =
    Join-Path $outDir 'free-config-detection.obj'
$phase7WindowTitleRuntimeBoundaryObject =
    Join-Path $outDir 'phase7_window_title_runtime_boundary.obj'
$getWindowTitleObject = Join-Path $outDir 'get-window-title.obj'
$stage2BoundaryObject = Join-Path $outDir 'stage2_engine_boundary.obj'
$stage3BoundaryObject = Join-Path $outDir 'stage3_engine_boundary.obj'
$stage3Phase3BoundaryObject =
    Join-Path $outDir 'stage3_phase3_engine_boundary.obj'
$stage3Phase4BoundaryObject =
    Join-Path $outDir 'stage3_phase4_engine_boundary.obj'
$stage3Phase5BoundaryObject =
    Join-Path $outDir 'stage3_phase5_engine_boundary.obj'
$stage3Phase6BoundaryObject =
    Join-Path $outDir 'stage3_phase6_engine_boundary.obj'
$stage3Phase7BoundaryObject =
    Join-Path $outDir 'stage3_phase7_engine_boundary.obj'
$stage3Phase8BoundaryObject =
    Join-Path $outDir 'stage3_phase8_engine_boundary.obj'
$stage3Phase9BoundaryObject =
    Join-Path $outDir 'stage3_phase9_engine_boundary.obj'
$stage3Phase10BoundaryObject =
    Join-Path $outDir 'stage3_phase10_engine_boundary.obj'
$stage3CompleteBoundaryObject =
    Join-Path $outDir 'stage3_complete_engine_boundary.obj'
$visualBoundaryObject = Join-Path $outDir 'visual_engine_boundary.obj'
$visualBootObject = Join-Path $outDir 'visual_boot_checkpoint.obj'
$visualBootD3D9Object = Join-Path $outDir 'visual_boot_d3d9.obj'
$retailVideoBridgeObject = Join-Path $outDir 'retail_video_bridge.obj'
$videoFrameConversionObject = Join-Path $outDir 'video_frame_conversion.obj'
$videoFramePublicationObject = Join-Path $outDir 'video_frame_publication.obj'
$videoFramePublicationBehaviorObject = Join-Path $outDir 'video_frame_publication_behavior.obj'
$frontendStartupSequenceObject = Join-Path $outDir 'frontend_startup_sequence.obj'
$frontendStartupSequenceBehaviorObject = Join-Path $outDir 'frontend_startup_sequence_behavior.obj'
$visualBootBehaviorObject = Join-Path $outDir 'visual_boot_checkpoint_behavior.obj'
$render2DBatchPlanObject = Join-Path $outDir 'render2d_batch_plan.obj'
$render2DBatchPlanBehaviorObject = Join-Path $outDir 'render2d_batch_plan_behavior.obj'
$render2DDrawListAdapterObject = Join-Path $outDir 'render2d_draw_list_adapter.obj'
$render2DDrawListAdapterBehaviorObject = Join-Path $outDir 'render2d_draw_list_adapter_behavior.obj'
$attachTextureToStageObject = Join-Path $outDir 'attach_texture_to_stage.obj'
$realiseRenderStateObject = Join-Path $outDir 'realise_render_state.obj'
$soldStateBlockObject = Join-Path $outDir 'sold_state_block.obj'
$updatePixelShaderObject = Join-Path $outDir 'update_pixel_shader.obj'
$resetWorldTransformObject = Join-Path $outDir 'reset_world_transform.obj'
$displaySetViewportObject = Join-Path $outDir 'display_set_viewport.obj'
$displaySetIntegerViewportObject = Join-Path $outDir 'display_set_integer_viewport.obj'
$postViewportShaderObject = Join-Path $outDir 'post_viewport_shader.obj'
$viewportE2Object = Join-Path $outDir 'viewport_e2.obj'
$setRenderWindowObject = Join-Path $outDir 'set_render_window.obj'
$clearRender2DVertexQueueObject = Join-Path $outDir 'clear_render2d_vertex_queue.obj'
$restoreRenderStateCaptureObject = Join-Path $outDir 'restore_render_state_capture.obj'
$textureCalcByteLengthObject = Join-Path $outDir 'texture_calc_byte_length.obj'
$pixelFormatGetColourDepthObject = Join-Path $outDir 'pixel_format_get_colour_depth.obj'
$pixelFormatInitialiseObject = Join-Path $outDir 'pixel_format_initialise.obj'
$pixelFormatTableObject = Join-Path $outDir 'pixel_format_table.obj'
$textureInitialisePreallocatedObject = Join-Path $outDir 'texture_initialise_preallocated.obj'
$textureAssignmentObject = Join-Path $outDir 'texture_assignment.obj'
$textureUninitialiseObject = Join-Path $outDir 'texture_uninitialise.obj'
$visualBootRetailArtwork = Join-Path $outDir 'frontend_backdrop_01.png'
$visualBootRetailTitle = Join-Path $outDir 'frontend_title_01_sprite.png'
$visualBootRetailTitleRight = Join-Path $outDir 'frontend_title_02_sprite.png'
$visualBootRetailPrompt = Join-Path $outDir 'frontend_press_start_text.png'
$visualBootRetailLegal = Join-Path $outDir 'frontend_legal_text.png'
$visualBootRetailPointer = Join-Path $outDir 'frontend_mouse_pointer.png'
$visualBootRetailForestSheet = Join-Path $outDir 'frontend_forest_sheet.png'
$visualBootRetailSunbeamSheet = Join-Path $outDir 'frontend_sunbeam_sheet.png'
$visualBootRetailMenu = Join-Path $outDir 'frontend_main_menu.png'
$visualBootRetailMenuComponents =
    Join-Path $outDir 'frontend_main_menu_components.png'
$visualBootBuffJesusMenu =
    Join-Path $outDir 'frontend_main_menu_buff_jesus.png'
$visualBootBuffJesusMenuComponents =
    Join-Path $outDir 'frontend_main_menu_buff_jesus_components.png'
$visualBootRetailOptions = Join-Path $outDir 'frontend_options_menu.png'
$visualBootRetailOptionsComponents =
    Join-Path $outDir 'frontend_options_menu_components.png'
$visualBootRetailTitleSegment =
    Join-Path $outDir 'frontend_title_rule_segment.png'
$visualBootRetailButtonLeft =
    Join-Path $outDir 'frontend_options_button_left.png'
$visualBootRetailButtonMiddle =
    Join-Path $outDir 'frontend_options_button_middle.png'
$visualBootRetailButtonRight =
    Join-Path $outDir 'frontend_options_button_right.png'
$visualBootRetailHelpers = Join-Path $outDir 'frontend_helpers.png'
$visualBootRetailAbout = Join-Path $outDir 'frontend_about_menu.png'
$visualBootRetailCoastalSheet = Join-Path $outDir 'frontend_coastal_sheet.png'
$visualBootRetailCoastalSunbeamSheet =
    Join-Path $outDir 'frontend_coastal_sunbeam_sheet.png'
$visualBootRetailSpookySheet = Join-Path $outDir 'frontend_spooky_sheet.png'
$visualBootRetailSpookySunbeamSheet =
    Join-Path $outDir 'frontend_spooky_sunbeam_sheet.png'
$visualBootBitmap = Join-Path $outDir 'visual_boot_artwork.bmp'
$visualBootTitleBitmap = Join-Path $outDir 'visual_boot_title.bmp'
$visualBootForestBitmap = Join-Path $outDir 'visual_boot_forest.bmp'
$visualBootSunbeamBitmap = Join-Path $outDir 'visual_boot_sunbeam.bmp'
$visualBootMenuBitmap = Join-Path $outDir 'visual_boot_main_menu.bmp'
$visualBootBuffJesusMenuBitmap =
    Join-Path $outDir 'visual_boot_main_menu_buff_jesus.bmp'
$visualBootOptionsBitmap = Join-Path $outDir 'visual_boot_options_menu.bmp'
$visualBootTitleSegmentBitmap =
    Join-Path $outDir 'visual_boot_title_rule_segment.bmp'
$visualBootButtonLeftBitmap =
    Join-Path $outDir 'visual_boot_options_button_left.bmp'
$visualBootButtonMiddleBitmap =
    Join-Path $outDir 'visual_boot_options_button_middle.bmp'
$visualBootButtonRightBitmap =
    Join-Path $outDir 'visual_boot_options_button_right.bmp'
$visualBootHelpersBitmap = Join-Path $outDir 'visual_boot_helpers.bmp'
$visualBootAboutBitmap = Join-Path $outDir 'visual_boot_about_menu.bmp'
$visualBootCoastalBitmap = Join-Path $outDir 'visual_boot_coastal.bmp'
$visualBootCoastalSunbeamBitmap =
    Join-Path $outDir 'visual_boot_coastal_sunbeam.bmp'
$visualBootSpookyBitmap = Join-Path $outDir 'visual_boot_spooky.bmp'
$visualBootSpookySunbeamBitmap =
    Join-Path $outDir 'visual_boot_spooky_sunbeam.bmp'
$visualBootSoundUpDown = Join-Path $outDir 'visual_boot_gui_updown.wav'
$visualBootSoundError = Join-Path $outDir 'visual_boot_gui_error.wav'
$visualBootSoundBack = Join-Path $outDir 'visual_boot_gui_back.wav'
$visualBootSoundForward = Join-Path $outDir 'visual_boot_gui_forward.wav'
$visualBootCursor = Join-Path $outDir 'visual_boot_pointer.cur'
$visualBootResourceSource = Join-Path $outDir 'visual_boot_checkpoint.rc'
$visualBootResource = Join-Path $outDir 'visual_boot_checkpoint.res'
$gfmainPhase1BehaviorObject = Join-Path $outDir 'gfmain_phase1_behavior.obj'
$gfmainPhase2BehaviorObject = Join-Path $outDir 'gfmain_phase2_behavior.obj'
$gfmainPhase3BehaviorObject = Join-Path $outDir 'gfmain_phase3_behavior.obj'
$gfmainPhase4BehaviorObject = Join-Path $outDir 'gfmain_phase4_behavior.obj'
$gfmainPhase5BehaviorObject = Join-Path $outDir 'gfmain_phase5_behavior.obj'
$gfmainPhase6BehaviorObject = Join-Path $outDir 'gfmain_phase6_behavior.obj'
$gfmainPhase7BehaviorObject = Join-Path $outDir 'gfmain_phase7_behavior.obj'
$gfmainPhase8BehaviorObject = Join-Path $outDir 'gfmain_phase8_behavior.obj'
$gfmainPhase9BehaviorObject = Join-Path $outDir 'gfmain_phase9_behavior.obj'
$gfmainPhase10BehaviorObject = Join-Path $outDir 'gfmain_phase10_behavior.obj'
$gfmainCompleteBehaviorObject = Join-Path $outDir 'gfmain_complete_behavior.obj'
$gfInitialiseProgressPhaseBehaviorObject = Join-Path $outDir 'gfinitialise_progress_phase_behavior.obj'
$profileEndObject = Join-Path $outDir 'profile-end.obj'
$asyncFailureHandlingObject = Join-Path $outDir 'async-failure-handling.obj'
$startupLatchObject = Join-Path $outDir 'startup-latch-clear.obj'
$fileInstallerGetObject = Join-Path $outDir 'cfi-singleton-get.obj'
$executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage0.exe'
$winMainBehaviorExecutable = Join-Path $outDir 'FableTLC-WinMain-Behavior.exe'
$stage1Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage1.exe'
$progressSetupBehaviorExecutable = Join-Path $outDir 'FableTLC-ProgressDisplay-Behavior.exe'
$setCurrentPathBehaviorExecutable = Join-Path $outDir 'FableTLC-SetCurrentPath-Behavior.exe'
$getProjectPathBehaviorExecutable = Join-Path $outDir 'FableTLC-GetProjectPath-Behavior.exe'
$wideStringConstructorBehaviorExecutable = Join-Path $outDir 'FableTLC-WideStringConstructor-Behavior.exe'
$wideStringDestructorBehaviorExecutable = Join-Path $outDir 'FableTLC-WideStringDestructor-Behavior.exe'
$charStringConstructorBehaviorExecutable = Join-Path $outDir 'FableTLC-CharStringConstructor-Behavior.exe'
$charStringDestructorBehaviorExecutable = Join-Path $outDir 'FableTLC-CharStringDestructor-Behavior.exe'
$profileStartBehaviorExecutable = Join-Path $outDir 'FableTLC-ProfileStart-Behavior.exe'
$gfmainPhase1BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase1-Behavior.exe'
$gfmainPhase2BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase2-Behavior.exe'
$gfmainPhase3BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase3-Behavior.exe'
$gfmainPhase4BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase4-Behavior.exe'
$gfmainPhase5BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase5-Behavior.exe'
$gfmainPhase6BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase6-Behavior.exe'
$gfmainPhase7BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase7-Behavior.exe'
$gfmainPhase8BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase8-Behavior.exe'
$gfmainPhase9BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase9-Behavior.exe'
$gfmainPhase10BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase10-Behavior.exe'
$gfmainCompleteBehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainComplete-Behavior.exe'
$gfInitialiseProgressPhaseBehaviorExecutable = Join-Path $outDir 'FableTLC-GFInitialiseProgressPhase-Behavior.exe'
$stage2Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage2.exe'
$stage3Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage3.exe'
$visualCheckpointExecutable = Join-Path $outDir 'FableTLC-Reconstruction-VisualCheckpoint.exe'
$visualBootBehaviorExecutable = Join-Path $outDir 'FableTLC-VisualBoot-Behavior.exe'
$videoFramePublicationBehaviorExecutable = Join-Path $outDir 'FableTLC-VideoFramePublication-Behavior.exe'
$frontendStartupSequenceBehaviorExecutable = Join-Path $outDir 'FableTLC-FrontendStartupSequence-Behavior.exe'
$render2DBatchPlanBehaviorExecutable = Join-Path $outDir 'FableTLC-Render2DBatchPlan-Behavior.exe'
$render2DDrawListAdapterBehaviorExecutable = Join-Path $outDir 'FableTLC-Render2DDrawListAdapter-Behavior.exe'
$passPattern = 'FABLETLC_BOOTSTRAP_STAGE0 PASS'
$winMainPassPattern = 'FABLETLC_WINMAIN_BEHAVIOR PASS'
$gfInitialisePassPattern = 'FABLETLC_GFINITIALISE_BEHAVIOR PASS'
$gfUninitialisePassPattern = 'FABLETLC_GF_UNINITIALISE_BEHAVIOR PASS'
$gfHandleSystemInitErrorPassPattern = 'FABLETLC_GF_HANDLE_SYSTEM_INIT_ERROR_BEHAVIOR PASS'
$gamePlayPassPattern = 'FABLETLC_CGAME_PLAY_BEHAVIOR PASS'
$initialiseConsoleVariablesPassPattern = 'FABLETLC_INITIALISE_CONSOLE_VARIABLES PASS'
$getWindowTitlePassPattern = 'FABLETLC_GET_WINDOW_TITLE_BEHAVIOR PASS'
$imeInitialisePassPattern = 'FABLETLC_CIME_INITIALISE_BEHAVIOR PASS'
$generateMetFilesPassPattern = 'FABLETLC_GENERATE_MET_FILES_BEHAVIOR PASS'
$freeConfigDetectionPassPattern = 'FABLETLC_FREE_CONFIG_DETECTION_BEHAVIOR PASS'
$systemManagerInitialisePassPattern = 'FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR PASS'
$exitThunkPassPattern = 'FABLETLC_EXIT_THUNK_BEHAVIOR PASS'
$doEulaPassPattern = 'FABLETLC_EULA_BEHAVIOR PASS'
$configDetectionPassPattern = 'FABLETLC_CONFIG_DETECTION_BEHAVIOR PASS'
$persistSignedCharPassPattern = 'FABLETLC_PERSIST_SIGNED_CHAR_BEHAVIOR PASS'
$stringParserCommentListDestructorPassPattern = 'FABLETLC_STRING_PARSER_COMMENT_LIST_DTOR PASS'
$imeSettingsPassPattern = 'FABLETLC_IME_SETTINGS_BEHAVIOR PASS'
$bankHeaderDirectoryPassPattern = 'FABLETLC_BANK_HEADER_DIRECTORY_BEHAVIOR PASS'
$myDocumentsDirectoryPassPattern = 'FABLETLC_MY_DOCUMENTS_DIRECTORY_BEHAVIOR PASS'
$saveDirectoryPassPattern = 'FABLETLC_SAVE_DIRECTORY_BEHAVIOR PASS'
$physicsFacesCountedResetPassPattern = 'FABLETLC_PHYSICS_FACES_COUNTED_RESET PASS'
$wideStringAssignLiteralPassPattern = 'FABLETLC_WIDE_STRING_ASSIGN_LITERAL PASS'
$wideStringAddWidePassPattern = 'FABLETLC_WIDE_STRING_ADD_WIDE PASS'
$defStringTableLoadPassPattern = 'FABLETLC_DEF_STRING_TABLE_LOAD_BEHAVIOR PASS'
$gameTextDataBankConstructorPassPattern = 'FABLETLC_GAME_TEXT_DATA_BANK_CONSTRUCTOR PASS'
$gameTextGetGroupEntryPassPattern = 'FABLETLC_GAME_TEXT_GET_GROUP_ENTRY PASS'
$myDocumentsWritePermissionsPassPattern = 'FABLETLC_MY_DOCUMENTS_WRITE_PERMISSIONS PASS'
$retailBankConstructorPassPattern = 'FABLETLC_RETAIL_BANK_CONSTRUCTOR PASS'
$threadedFileConstructorPassPattern = 'FABLETLC_THREADED_FILE_CONSTRUCTOR PASS'
$threadedFileCountedResetPassPattern = 'FABLETLC_THREADED_FILE_COUNTED_RESET PASS'
$threadedFileOpenPassPattern = 'FABLETLC_THREADED_FILE_OPEN PASS'
$containedBankMapIndexPassPattern = 'FABLETLC_CONTAINED_BANK_MAP_INDEX PASS'
$openRetailBankPassPattern = 'FABLETLC_OPEN_RETAIL_BANK PASS'
$openIniFilePassPattern = 'FABLETLC_OPEN_INI_FILE PASS'
$userProfileSingletonGetPassPattern = 'FABLETLC_USER_PROFILE_SINGLETON_GET_BEHAVIOR PASS'
$userProfileSetProfileNamePassPattern = 'FABLETLC_USER_PROFILE_SET_PROFILE_NAME_BEHAVIOR PASS'
$progressSetupPassPattern = 'FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS'
$progressDisplayConstructorPassPattern = 'FABLETLC_PROGRESS_DISPLAY_CONSTRUCTOR_BEHAVIOR PASS'
$progressDisplayTextBankPassPattern = 'FABLETLC_PROGRESS_TEXT_BANK_BEHAVIOR PASS'
$progressDisplayTextModePassPattern = 'FABLETLC_PROGRESS_DISPLAY_TEXT_MODE_BEHAVIOR PASS'
$progressDisplayIsActivePassPattern = 'FABLETLC_PROGRESS_DISPLAY_IS_ACTIVE_BEHAVIOR PASS'
$setProgressDisplayPassPattern = 'FABLETLC_SET_PROGRESS_DISPLAY_BEHAVIOR PASS'
$getProgressDisplayPassPattern = 'FABLETLC_GET_PROGRESS_DISPLAY_BEHAVIOR PASS'
$setCurrentPathPassPattern = 'FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS'
$getProjectPathPassPattern = 'FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS'
$wideStringConstructorPassPattern = 'FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS'
$wideStringDestructorPassPattern = 'FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS'
$wideStringConversionPassPattern = 'FABLETLC_WIDE_STRING_CONVERSION_BEHAVIOR PASS'
$wideStringNotEqualPassPattern = 'FABLETLC_WIDE_STRING_NOT_EQUAL_BEHAVIOR PASS'
$systemRegistryConstructorPassPattern = 'FABLETLC_SYSTEM_REGISTRY_CONSTRUCTOR_BEHAVIOR PASS'
$systemRegistryDestructorPassPattern = 'FABLETLC_SYSTEM_REGISTRY_DESTRUCTOR_BEHAVIOR PASS'
$systemRegistryWriteBoolPassPattern = 'FABLETLC_SYSTEM_REGISTRY_WRITE_BOOL_BEHAVIOR PASS'
$wideLiteralConstructorPassPattern = 'FABLETLC_WIDE_LITERAL_CONSTRUCTOR_BEHAVIOR PASS'
$wideStringCopyAssignmentPassPattern = 'FABLETLC_WIDE_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS'
$charStringConstructorPassPattern = 'FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS'
$charStringDestructorPassPattern = 'FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS'
$profileStartPassPattern = 'FABLETLC_PROFILE_START_BEHAVIOR PASS'
$charStringDefaultPassPattern = 'FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS'
$systemManagerInitPassPattern = 'FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS'
$gfmainPhase1PassPattern = 'FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS'
$gfmainPhase2PassPattern = 'FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS'
$gfmainPhase3PassPattern = 'FABLETLC_GFMAIN_PHASE3_BEHAVIOR PASS'
$gfmainPhase4PassPattern = 'FABLETLC_GFMAIN_PHASE4_BEHAVIOR PASS'
$gfmainPhase5PassPattern = 'FABLETLC_GFMAIN_PHASE5_BEHAVIOR PASS'
$gfmainPhase6PassPattern = 'FABLETLC_GFMAIN_PHASE6_BEHAVIOR PASS'
$gfmainPhase7PassPattern = 'FABLETLC_GFMAIN_PHASE7_BEHAVIOR PASS'
$gfmainPhase8PassPattern = 'FABLETLC_GFMAIN_PHASE8_BEHAVIOR PASS'
$gfmainPhase9PassPattern = 'FABLETLC_GFMAIN_PHASE9_BEHAVIOR PASS'
$gfmainPhase10PassPattern = 'FABLETLC_GFMAIN_PHASE10_BEHAVIOR PASS'
$gfmainCompletePassPattern = 'FABLETLC_GFMAIN_COMPLETE PASS'
$gfInitialiseProgressPhasePassPattern = 'FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS'
$visualBootPassPattern = 'FABLETLC_VISUAL_BOOT_BEHAVIOR PASS'
$videoFramePublicationPassPattern = 'FABLETLC_VIDEO_FRAME_PUBLICATION PASS'
$frontendStartupSequencePassPattern = 'FABLETLC_FRONTEND_STARTUP_SEQUENCE PASS'
$render2DBatchPlanPassPattern = 'FABLETLC_RENDER2D_BATCH_PLAN PASS'
$render2DDrawListAdapterPassPattern = 'FABLETLC_RENDER2D_DRAW_LIST_ADAPTER PASS'
$profileEndPassPattern = 'FABLETLC_PROFILE_END_BEHAVIOR PASS'
$asyncFailureHandlingPassPattern = 'FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS'
$startupLatchPassPattern = 'FABLETLC_STARTUP_LATCH_BEHAVIOR PASS'
$fileInstallerGetPassPattern = 'FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS'
$defaultLanguageNamePassPattern = 'FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS'
$fontBankNamePassPattern = 'FABLETLC_FONT_BANK_NAME_BEHAVIOR PASS'
$streamingFontBankNamePassPattern = 'FABLETLC_STREAMING_FONT_BANK_NAME_BEHAVIOR PASS'
$wideStringFromCharPassPattern = 'FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS'
$wideStringCopyPassPattern = 'FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS'
$wideStringAddRightPassPattern = 'FABLETLC_WIDE_STRING_ADD_RIGHT PASS'
$wideStringAddLeftPassPattern = 'FABLETLC_WIDE_STRING_ADD_LEFT PASS'
$pathExistsPassPattern = 'FABLETLC_PATH_EXISTS_00999230 PASS'
$diskFileConstructorPassPattern = 'FABLETLC_DISK_FILE_CTOR_0099AD80 PASS'
$diskFileClosePassPattern = 'FABLETLC_DISK_FILE_CLOSE_0099A920 PASS'
$charStringLoadFilePassPattern = 'FABLETLC_CHAR_STRING_LOAD_FILE_0099F2E0 PASS'
$stringParserConstructorPassPattern = 'FABLETLC_STRING_PARSER_CTOR_00404720 PASS'
$persistContextTextConstructorPassPattern = 'FABLETLC_PERSIST_CONTEXT_TEXT_CTOR_009BADD0 PASS'
$surfaceCopyConstructorPassPattern = 'FABLETLC_SURFACE_COPY_CONSTRUCTOR_BEHAVIOR PASS'
$surfaceInitialiseMipmapPassPattern = 'FABLETLC_SURFACE_INITIALISE_TEXTURE_MIPMAP_BEHAVIOR PASS'
$surfaceCopyFromSurfacePassPattern = 'FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR PASS'
$textureCopyFromTexturePassPattern = 'FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR PASS'
$charToWideStringPassPattern = 'FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS'
$primaryLeftAlignmentPassPattern = 'FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS'
$secondaryLeftAlignmentPassPattern = 'FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS'
$charStringCopyAssignmentPassPattern = 'FABLETLC_CHAR_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS'
$retailBankNavigatorPassPattern = 'FABLETLC_RETAIL_BANK_NAVIGATOR_BEHAVIOR PASS'
$retailBankAliasPassPattern = 'FABLETLC_RETAIL_BANK_ALIAS_BEHAVIOR PASS'
$getSystemManagerPassPattern = 'FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR PASS'
$movieLeftAlignPassPattern = 'FABLETLC_MOVIE_LEFT_ALIGN_BEHAVIOR PASS'
$imePrimitivePassPattern = 'CEngineInternalPrimitiveBase_00404a80_TEST PASS'
$crcCalcPassPattern = 'FABLETLC_CRC_CALC_BEHAVIOR PASS'
$miscDirectoryAPassPattern = 'FABLETLC_MISC_DIRECTORY_A_BEHAVIOR PASS'
$miscDirectoryBPassPattern = 'FABLETLC_MISC_DIRECTORY_B_BEHAVIOR PASS'
$graphicsDirectoryPassPattern = 'FABLETLC_GRAPHICS_DIRECTORY_BEHAVIOR PASS'
$languageDirectoryAPassPattern = 'FABLETLC_LANGUAGE_DIRECTORY_A_BEHAVIOR PASS'
$shadersDirectoryPassPattern = 'FABLETLC_SHADERS_DIRECTORY_BEHAVIOR PASS'
$cacheDirectoryPassPattern = 'FABLETLC_CACHE_DIRECTORY_BEHAVIOR PASS'
$languageDirectoryBPassPattern = 'FABLETLC_LANGUAGE_DIRECTORY_B_BEHAVIOR PASS'
$checkpointDirectoryPassPattern = 'FABLETLC_CHECKPOINT_DIRECTORY_BEHAVIOR PASS'
$defTablePathPassPattern = 'FABLETLC_DEF_TABLE_PATH_BEHAVIOR PASS'
$bankHandlePassPattern = 'FABLETLC_BANK_HANDLE_BEHAVIOR PASS'
$vectorMapLookupPassPattern = 'FABLETLC_VECTOR_MAP_LOOKUP_BEHAVIOR PASS'
$cbaseRestoreAPassPattern = 'FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS'
$cbaseRestoreBPassPattern = 'FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS'
$cmovieIsPlayingPassPattern = 'FABLETLC_CMOVIE_IS_PLAYING PASS'
$cmovieSetMoviePassPattern = 'FABLETLC_CMOVIE_SET_MOVIE PASS'
$videoSysGetTexturePassPattern = 'FABLETLC_CVIDEOSYS_GET_TEXTURE PASS'
$videoSysAttemptToPlayPassPattern = 'FABLETLC_CVIDEOSYS_ATTEMPT_TO_PLAY PASS'
$videoSysWaitForStatePassPattern = 'FABLETLC_CVIDEOSYS_WAIT_FOR_STATE PASS'
$videoSysPausePassPattern = 'FABLETLC_CVIDEOSYS_PAUSE PASS'
$videoSysStopPassPattern = 'FABLETLC_CVIDEOSYS_STOP PASS'

$required = @(
    (Join-Path $vcRoot 'bin\cl.exe'),
    (Join-Path $vcRoot 'bin\link.exe'),
    $bootstrapSource,
    $retailSource,
    $winMainSource,
    $winMainBehaviorSource,
    $gfInitialiseSource,
    $gfInitialiseBehaviorSource,
    $gfUninitialiseSource,
    $gfUninitialiseBehaviorSource,
    $gfHandleSystemInitErrorSource,
    $gfHandleSystemInitErrorBehaviorSource,
    $gamePlaySource,
    $gamePlayBehaviorSource,
    $initialiseConsoleVariablesSource,
    $initialiseConsoleVariablesBehaviorSource,
    $getWindowTitleSource,
    $getWindowTitleBehaviorSource,
    $imeInitialiseSource,
    $imeInitialiseBehaviorSource,
    $generateMetFilesSource,
    $generateMetFilesBehaviorSource,
    $freeConfigDetectionSource,
    $freeConfigDetectionBehaviorSource,
    $systemManagerInitialiseSource,
    $systemManagerInitialiseBehaviorSource,
    $exitThunkSource,
    $exitThunkBehaviorSource,
    $doEulaSource,
    $doEulaBehaviorSource,
    $configDetectionSource,
    $configDetectionBehaviorSource,
    $persistSignedCharSource,
    $persistSignedCharBehaviorSource,
    $userProfileSingletonGetSource,
    $userProfileSingletonGetBehaviorSource,
    $userProfileSetProfileNameSource,
    $userProfileSetProfileNameBehaviorSource,
    $stage1BoundarySource,
    $progressSetupSource,
    $progressSetupBehaviorSource,
    $progressDisplayConstructorSource,
    $progressDisplayConstructorBehaviorSource,
    $progressDisplayTextBankSource,
    $progressDisplayTextBankBehaviorSource,
    $progressDisplayTextModeSource,
    $progressDisplayTextModeBehaviorSource,
    $progressDisplayIsActiveSource,
    $progressDisplayIsActiveBehaviorSource,
    $setProgressDisplaySource,
    $setProgressDisplayBehaviorSource,
    $getProgressDisplaySource,
    $getProgressDisplayBehaviorSource,
    $setCurrentPathSource,
    $setCurrentPathBehaviorSource,
    $getProjectPathSource,
    $getProjectPathBehaviorSource,
    $wideStringConstructorSource,
    $wideStringConstructorBehaviorSource,
    $wideStringDestructorSource,
    $wideStringDestructorBehaviorSource,
    $wideStringConversionSource,
    $wideStringConversionBehaviorSource,
    $wideStringNotEqualSource,
    $wideStringNotEqualBehaviorSource,
    $systemRegistryConstructorSource,
    $systemRegistryConstructorBehaviorSource,
    $systemRegistryDestructorSource,
    $systemRegistryDestructorBehaviorSource,
    $systemRegistryWriteBoolSource,
    $systemRegistryWriteBoolBehaviorSource,
    $wideLiteralConstructorSource,
    $wideLiteralConstructorBehaviorSource,
    $wideStringCopyAssignmentSource,
    $wideStringCopyAssignmentBehaviorSource,
    $charStringConstructorSource,
    $charStringConstructorBehaviorSource,
    $charStringDestructorSource,
    $charStringDestructorBehaviorSource,
    $charStringCopyAssignmentSource,
    $charStringCopyAssignmentBehaviorSource,
    $retailBankNavigatorSource,
    $retailBankNavigatorBehaviorSource,
    $retailBankAliasSource,
    $retailBankAliasBehaviorSource,
    $getSystemManagerSource,
    $getSystemManagerBehaviorSource,
    $movieLeftAlignSource,
    $movieLeftAlignBehaviorSource,
    $imePrimitiveSource,
    $imePrimitiveBehaviorSource,
    $crcCalcSource,
    $crcCalcBehaviorSource,
    $profileStartSource,
    $profileStartBehaviorSource,
    $profileEndSource,
    $profileEndBehaviorSource,
    $asyncFailureHandlingSource,
    $asyncFailureHandlingBehaviorSource,
    $startupLatchSource,
    $startupLatchBehaviorSource,
    $fileInstallerGetSource,
    $fileInstallerGetBehaviorSource,
    $defaultLanguageNameSource,
    $defaultLanguageNameBehaviorSource,
    $fontBankNameSource,
    $fontBankNameBehaviorSource,
    $streamingFontBankNameSource,
    $streamingFontBankNameBehaviorSource,
    $miscDirectoryASource,
    $miscDirectoryABehaviorSource,
    $miscDirectoryBSource,
    $miscDirectoryBBehaviorSource,
    $graphicsDirectorySource,
    $graphicsDirectoryBehaviorSource,
    $languageDirectoryASource,
    $languageDirectoryABehaviorSource,
    $shadersDirectorySource,
    $shadersDirectoryBehaviorSource,
    $cacheDirectorySource,
    $cacheDirectoryBehaviorSource,
    $languageDirectoryBSource,
    $languageDirectoryBBehaviorSource,
    $checkpointDirectorySource,
    $checkpointDirectoryBehaviorSource,
    $defTablePathSource,
    $defTablePathBehaviorSource,
    $bankHandleSource,
    $bankHandleBehaviorSource,
    $vectorMapLookupSource,
    $vectorMapLookupBehaviorSource,
    $wideStringFromCharSource,
    $wideStringFromCharBehaviorSource,
    $wideStringCopySource,
    $wideStringCopyBehaviorSource,
    $wideStringAddRightSource,
    $wideStringAddRightBehaviorSource,
    $wideStringAddLeftSource,
    $wideStringAddLeftBehaviorSource,
    $pathExistsSource,
    $pathExistsBehaviorSource,
    $diskFileConstructorSource,
    $diskFileConstructorBehaviorSource,
    $diskFileCloseSource,
    $diskFileCloseBehaviorSource,
    $charStringLoadFileSource,
    $charStringLoadFileBehaviorSource,
    $stringParserConstructorSource,
    $stringParserConstructorBehaviorSource,
    $persistContextTextConstructorSource,
    $persistContextTextConstructorBehaviorSource,
    $charToWideStringSource,
    $charToWideStringBehaviorSource,
    $primaryLeftAlignmentSource,
    $primaryLeftAlignmentBehaviorSource,
    $secondaryLeftAlignmentSource,
    $secondaryLeftAlignmentBehaviorSource,
    $cbaseRestoreASource,
    $cbaseRestoreABehaviorSource,
    $cbaseRestoreBSource,
    $cbaseRestoreBBehaviorSource,
    $cmovieIsPlayingSource,
    $cmovieIsPlayingBehaviorSource,
    $cmovieSetMovieSource,
    $cmovieSetMovieBehaviorSource,
    $videoSysGetTextureSource,
    $videoSysGetTextureBehaviorSource,
    $videoSysAttemptToPlaySource,
    $videoSysAttemptToPlayBehaviorSource,
    $videoSysWaitForStateSource,
    $videoSysWaitForStateBehaviorSource,
    $videoSysPauseSource,
    $videoSysPauseBehaviorSource,
    $videoSysStopSource,
    $videoSysStopBehaviorSource,
    $charStringDefaultSource,
    $charStringDefaultBehaviorSource,
    $systemManagerInitSource,
    $systemManagerInitBehaviorSource,
    $gfmainPhase1Source,
    $gfmainPhase2Source,
    $gfmainPhase3Source,
    $gfmainPhase4Source,
    $gfmainPhase5Source,
    $gfmainPhase6Source,
    $gfmainPhase7Source,
    $gfmainPhase8Source,
    $gfmainPhase9Source,
    $gfmainPhase10Source,
    $gfmainCompleteSource,
    $gfInitialiseProgressPhaseSource,
    $gfInitialiseEngineBoundarySource,
    $phase6RuntimeBoundarySource,
    $systemManagerRuntimeBoundarySource,
    $saveMetadataRuntimeBoundarySource,
    $phase8RuntimeBoundarySource,
    $phase7WindowTitleRuntimeBoundarySource,
    $progressDisplayStringBoundarySource,
    $stage2BoundarySource,
    $visualBootSource,
    $visualBootD3D9Source,
    $videoFrameConversionSource,
    $videoFramePublicationSource,
    $videoFramePublicationBehaviorSource,
    $frontendStartupSequenceSource,
    $frontendStartupSequenceBehaviorSource,
    $visualBootFallbackArtwork,
    $frontendSoundExtractor,
    $visualBootBehaviorSource,
    $render2DBatchPlanSource,
    $render2DBatchPlanBehaviorSource,
    $render2DDrawListAdapterSource,
    $render2DDrawListAdapterBehaviorSource,
    $attachTextureToStageSource,
    $realiseRenderStateSource,
    $displaySetViewportSource,
    $displaySetIntegerViewportSource,
    $postViewportShaderSource,
    $viewportE2Source,
    $setRenderWindowSource,
    $clearRender2DVertexQueueSource,
    $restoreRenderStateCaptureSource,
    $textureCalcByteLengthSource,
    $pixelFormatGetColourDepthSource,
    $pixelFormatInitialiseSource,
    $pixelFormatTableSource,
    $textureInitialisePreallocatedSource,
    $textureAssignmentSource,
    $textureUninitialiseSource,
    $surfaceCopyConstructorSource,
    $surfaceCopyConstructorBehaviorSource,
    $surfaceInitialiseMipmapSource,
    $surfaceInitialiseMipmapBehaviorSource,
    $surfaceCopyFromSurfaceSource,
    $surfaceCopyFromSurfaceBehaviorSource,
    $textureCopyFromTextureSource,
    $textureCopyFromTextureBehaviorSource,
    $gfmainPhase1BehaviorSource,
    $gfmainPhase2BehaviorSource,
    $gfmainPhase3BehaviorSource,
    $gfmainPhase4BehaviorSource,
    $gfmainPhase5BehaviorSource,
    $gfmainPhase6BehaviorSource,
    $gfmainPhase7BehaviorSource,
    $gfmainPhase8BehaviorSource,
    $gfmainPhase9BehaviorSource,
    $gfmainPhase10BehaviorSource,
    $gfmainCompleteBehaviorSource,
    $gfInitialiseProgressPhaseBehaviorSource,
    $textureBuilder,
    $staticFontRenderer,
    $streamingFontRenderer,
    $cursorBuilder,
    $frontendAnimationRenderer,
    $frontendMenuRenderer,
    $bootObjectChecker
)
$missing = @($required | Where-Object { -not (Test-Path -LiteralPath $_) })
if ($missing.Count -gt 0) {
    throw "Bootstrap prerequisites are missing: $($missing -join ', ')"
}

New-Item -ItemType Directory -Path $outDir -Force | Out-Null

$retailFrontendCandidates = @()
if ($RetailFrontendBank) {
    $retailFrontendCandidates += $RetailFrontendBank
} else {
    # Prefer an untouched retail bank.  work\ui_proto\art is an intentional
    # texture-authoring fixture whose FRONTEND_BUTTON_L_SPRITE is gold-tinted;
    # using it for the checkpoint leaks that experiment into every title rule.
    $retailFrontendCandidates +=
        'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\graphics\pc\frontend.big'
    $programFilesX86 = ${env:ProgramFiles(x86)}
    if ($programFilesX86) {
        $retailFrontendCandidates += Join-Path $programFilesX86 `
            'Steam\steamapps\common\Fable The Lost Chapters\data\graphics\pc\frontend.big'
    }
    $retailFrontendCandidates += Join-Path $workspaceRoot `
        'work\ui_proto\art\frontend.big'
}

$selectedRetailFrontendBank = $retailFrontendCandidates |
    Where-Object {
        $_ -and (Test-Path -LiteralPath $_ -PathType Leaf)
    } |
    Select-Object -First 1
$visualBootUsesRetailAsset = $false
$visualBootUsesRetailPrompt = $false
$visualBootUsesRetailLegal = $false
$visualBootUsesRetailPointer = $false
$visualBootUsesRetailAnimation = $false
$visualBootUsesRetailMenu = $false
$visualBootUsesBuffJesusMenu = $false
$visualBootUsesRetailSubscreens = $false
$visualBootUsesRetailUiSounds = $false
if ($selectedRetailFrontendBank) {
    $retailDataRootFromFrontend = Split-Path -Parent (
        Split-Path -Parent (
            Split-Path -Parent $selectedRetailFrontendBank
        )
    )
    $selectedRetailFrontendSoundBank =
        Join-Path $retailDataRootFromFrontend 'Sound\Frontend.lug'
    if (Test-Path -LiteralPath $selectedRetailFrontendSoundBank -PathType Leaf) {
        $soundSamples = @(
            @{ Id = 3; Output = $visualBootSoundUpDown },
            @{ Id = 7; Output = $visualBootSoundError },
            @{ Id = 4; Output = $visualBootSoundBack },
            @{ Id = 5; Output = $visualBootSoundForward }
        )
        $soundExtractionFailed = $false
        foreach ($soundSample in $soundSamples) {
            & python $frontendSoundExtractor `
                $selectedRetailFrontendSoundBank `
                --extract $soundSample.Id $soundSample.Output
            if (
                $LASTEXITCODE -ne 0 -or
                -not (Test-Path -LiteralPath $soundSample.Output -PathType Leaf)
            ) {
                $soundExtractionFailed = $true
                break
            }
        }
        if (-not $soundExtractionFailed) {
            $visualBootUsesRetailUiSounds = $true
            Write-Output (
                'VISUAL_UI_SOUND RETAIL ' +
                'updown=CS_GUI_2/sample-3 ' +
                'error=CS_GUI_5/sample-7 ' +
                'back=CS_GUI_6/sample-4 ' +
                'forward=CS_GUI_7/sample-5'
            )
        } else {
            Write-Warning (
                'Retail frontend sound extraction failed; visual input ' +
                'continues without UI audio.'
            )
        }
    }
    & python $textureBuilder decode `
        $selectedRetailFrontendBank `
        FRONTEND_BACKDROP_01 `
        $visualBootRetailArtwork `
        --crop-real
    $backdropDecodeExitCode = $LASTEXITCODE
    & python $textureBuilder decode `
        $selectedRetailFrontendBank `
        FRONTEND_TITLE_01_SPRITE `
        $visualBootRetailTitle `
        --crop-real
    $titleLeftDecodeExitCode = $LASTEXITCODE
    & python $textureBuilder decode `
        $selectedRetailFrontendBank `
        FRONTEND_TITLE_02_SPRITE `
        $visualBootRetailTitleRight `
        --crop-real
    $titleRightDecodeExitCode = $LASTEXITCODE
    & python $textureBuilder decode `
        $selectedRetailFrontendBank `
        MOUSE_POINTER_SPRITE_FE `
        $visualBootRetailPointer `
        --crop-real
    $pointerDecodeExitCode = $LASTEXITCODE
    if (
        $backdropDecodeExitCode -ne 0 -or
        $titleLeftDecodeExitCode -ne 0 -or
        $titleRightDecodeExitCode -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootRetailArtwork) -or
        -not (Test-Path -LiteralPath $visualBootRetailTitle) -or
        -not (Test-Path -LiteralPath $visualBootRetailTitleRight)
    ) {
        if ($RetailFrontendBank) {
            throw (
                "Failed to decode the retail frontend backdrop/title pair " +
                "from $selectedRetailFrontendBank."
            )
        }
        Write-Warning (
            "Retail frontend asset decode failed; using authored artwork fallback."
        )
    } else {
        $visualBootArtwork = $visualBootRetailArtwork
        $visualBootUsesRetailAsset = $true
        if (
            $pointerDecodeExitCode -eq 0 -and
            (Test-Path -LiteralPath $visualBootRetailPointer)
        ) {
            & python $cursorBuilder `
                $visualBootRetailPointer `
                $visualBootCursor `
                --hotspot 0 0
            if (
                $LASTEXITCODE -eq 0 -and
                (Test-Path -LiteralPath $visualBootCursor)
            ) {
                $visualBootUsesRetailPointer = $true
            }
        }
        if (-not $visualBootUsesRetailPointer) {
            Write-Warning (
                "Retail mouse-pointer decode failed; keeping the system arrow."
            )
        }
        & python $frontendAnimationRenderer `
            $selectedRetailFrontendBank `
            $visualBootRetailForestSheet `
            $visualBootRetailSunbeamSheet
        if (
            $LASTEXITCODE -eq 0 -and
            (Test-Path -LiteralPath $visualBootRetailForestSheet) -and
            (Test-Path -LiteralPath $visualBootRetailSunbeamSheet)
        ) {
            $visualBootUsesRetailAnimation = $true
        } else {
            Write-Warning (
                "Retail forest/sunbeam decode failed; keeping the static " +
                "frontend backdrop."
            )
        }
        & python $frontendAnimationRenderer `
            $selectedRetailFrontendBank `
            $visualBootRetailCoastalSheet `
            $visualBootRetailCoastalSunbeamSheet `
            --theme coastal
        $coastalAnimationReady =
            $LASTEXITCODE -eq 0 -and
            (Test-Path -LiteralPath $visualBootRetailCoastalSheet) -and
            (Test-Path -LiteralPath $visualBootRetailCoastalSunbeamSheet)
        & python $frontendAnimationRenderer `
            $selectedRetailFrontendBank `
            $visualBootRetailSpookySheet `
            $visualBootRetailSpookySunbeamSheet `
            --theme spooky
        $spookyAnimationReady =
            $LASTEXITCODE -eq 0 -and
            (Test-Path -LiteralPath $visualBootRetailSpookySheet) -and
            (Test-Path -LiteralPath $visualBootRetailSpookySunbeamSheet)
        $retailDataRoot = Split-Path -Parent (
            Split-Path -Parent (
                Split-Path -Parent $selectedRetailFrontendBank
            )
        )
        $retailGameRoot = Split-Path -Parent $retailDataRoot
        $frontendLayoutArguments = @()
        $frontendSchema = Join-Path $workspaceRoot `
            'ghidra_out\def_schema.json'
        if (
            (Test-Path -LiteralPath (
                Join-Path $retailGameRoot `
                    'data\CompiledDefs\frontend.bin')) -and
            (Test-Path -LiteralPath $frontendSchema)
        ) {
            $frontendLayoutArguments += @(
                '--game-root',
                $retailGameRoot,
                '--schema',
                $frontendSchema
            )
        }
        $fontCandidates = @()
        $fontCandidates += Join-Path $retailDataRoot `
            'lang\English\fonts.big'
        $fontCandidates +=
            'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\fonts.big'
        if ($programFilesX86) {
            $fontCandidates += Join-Path $programFilesX86 `
                'Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\fonts.big'
        }
        $selectedRetailFontBank = $fontCandidates |
            Where-Object {
                $_ -and (Test-Path -LiteralPath $_ -PathType Leaf)
            } |
            Select-Object -First 1
        if ($selectedRetailFontBank) {
            & python $staticFontRenderer `
                $selectedRetailFontBank `
                ENG_ARIAL_24 `
                'Press Left Mouse Button To Continue' `
                $visualBootRetailPrompt `
                --canvas 640 480 `
                --position 320 240 `
                --align center `
                --scale 0.6666666667 `
                --outline-pixels 1
            if (
                $LASTEXITCODE -eq 0 -and
                (Test-Path -LiteralPath $visualBootRetailPrompt)
            ) {
                $visualBootUsesRetailPrompt = $true
            } else {
                Write-Warning (
                    "Retail press-start font render failed; keeping the " +
                    "decoded title without the prompt."
                )
            }

            $legalText =
                'Fable: The Lost Chapters ' +
                [char]0x00A9 +
                ' 2005 Lionhead Studios Ltd.  (P) 2005 Microsoft ' +
                'Corporation.  All rights reserved.  Developed by ' +
                'Lionhead Studios Ltd.'
            & python $streamingFontRenderer `
                $selectedRetailFontBank `
                ENG_ARIAL_24 `
                $legalText `
                $visualBootRetailLegal `
                --canvas 640 480 `
                --position 320 340 `
                --align center `
                --wrap-width 420 `
                --line-height 30 `
                --scale 0.6666666667 `
                --prefer-static-ascii `
                --outline-pixels 1 `
                --require-all-glyphs
            if (
                $LASTEXITCODE -eq 0 -and
                (Test-Path -LiteralPath $visualBootRetailLegal)
            ) {
                $visualBootUsesRetailLegal = $true
            } else {
                Write-Warning (
                    "Retail legal-text streaming-font render failed; " +
                    "keeping the decoded title and available prompt."
                )
            }
            if ($coastalAnimationReady) {
                & python $frontendMenuRenderer `
                    $selectedRetailFrontendBank `
                    $selectedRetailFontBank `
                    $visualBootRetailMenu `
                    --component-output $visualBootRetailMenuComponents `
                    @frontendLayoutArguments
                if (
                    $LASTEXITCODE -eq 0 -and
                    (Test-Path -LiteralPath $visualBootRetailMenu) -and
                    (Test-Path -LiteralPath $visualBootRetailMenuComponents)
                ) {
                    $visualBootUsesRetailMenu = $true
                    & python $frontendMenuRenderer `
                        $selectedRetailFrontendBank `
                        $selectedRetailFontBank `
                        $visualBootBuffJesusMenu `
                        --text-variant buff-jesus `
                        --component-output $visualBootBuffJesusMenuComponents `
                        @frontendLayoutArguments
                    if (
                        $LASTEXITCODE -eq 0 -and
                        (Test-Path -LiteralPath $visualBootBuffJesusMenu) -and
                        (Test-Path -LiteralPath $visualBootBuffJesusMenuComponents)
                    ) {
                        $visualBootUsesBuffJesusMenu = $true
                    } else {
                        Write-Warning (
                            "BuffJesus menu composition failed; the " +
                            "retail-exact menu remains available."
                        )
                    }
                } else {
                    Write-Warning (
                        "Retail main-menu composition failed; the " +
                        "press-start screen remains available."
                    )
                }
                if ($visualBootUsesRetailMenu) {
                    & python $frontendSubscreenRenderer `
                        $selectedRetailFrontendBank `
                        $selectedRetailFontBank `
                        $visualBootRetailOptions `
                        $visualBootRetailHelpers `
                        --components-output `
                        $visualBootRetailOptionsComponents `
                        --title-segment-output `
                        $visualBootRetailTitleSegment `
                        --button-left-output `
                        $visualBootRetailButtonLeft `
                        --button-middle-output `
                        $visualBootRetailButtonMiddle `
                        --button-right-output `
                        $visualBootRetailButtonRight `
                        --about-output `
                        $visualBootRetailAbout `
                        @frontendLayoutArguments
                    if (
                        $LASTEXITCODE -eq 0 -and
                        (Test-Path -LiteralPath $visualBootRetailOptions) -and
                        (Test-Path -LiteralPath `
                            $visualBootRetailOptionsComponents) -and
                        (Test-Path -LiteralPath `
                            $visualBootRetailTitleSegment) -and
                        (Test-Path -LiteralPath `
                            $visualBootRetailButtonLeft) -and
                        (Test-Path -LiteralPath `
                            $visualBootRetailButtonMiddle) -and
                        (Test-Path -LiteralPath `
                            $visualBootRetailButtonRight) -and
                        (Test-Path -LiteralPath $visualBootRetailAbout) -and
                        (Test-Path -LiteralPath $visualBootRetailHelpers)
                    ) {
                        $visualBootUsesRetailSubscreens = $true
                    } else {
                        Write-Warning (
                            "Retail Options/Quit composition failed; the " +
                            "first main menu remains available."
                        )
                    }
                }
            }
        }
        Write-Output (
            "VISUAL_ASSET RETAIL names=FRONTEND_BACKDROP_01," +
            "FRONTEND_TITLE_01_SPRITE,FRONTEND_TITLE_02_SPRITE," +
            "MOUSE_POINTER_SPRITE_FE " +
            "bank=$selectedRetailFrontendBank"
        )
        if ($visualBootUsesRetailAnimation) {
            Write-Output (
                "VISUAL_ANIMATION RETAIL forest=206-229 frames=4 " +
                "sunbeams=230-247 frames=3"
            )
        }
        if ($visualBootUsesRetailMenu) {
            Write-Output (
                "VISUAL_MENU RETAIL root=UI_FRONTEND_MAIN_MENU " +
                "background=COASTAL action=229"
            )
            if ($visualBootUsesBuffJesusMenu) {
                Write-Output (
                    "VISUAL_MENU VARIANT name=buff-jesus " +
                    "activation=--buff-jesus retail_default=preserved"
                )
            }
        }
        if ($visualBootUsesRetailSubscreens) {
            Write-Output (
                "VISUAL_SUBSCREENS RETAIL options_action=297 " +
                "detail_actions=9,12,13,283 " +
                "quit_action=314 no_action=86 yes_action=296"
            )
        }
    }
} elseif ($RetailFrontendBank) {
    throw "Retail frontend bank was not found: $RetailFrontendBank"
} else {
    Write-Output 'VISUAL_ASSET FALLBACK name=fabledecomp_boot_concept'
}

$windowsSdkUmLib = Get-ChildItem -LiteralPath $windowsSdkLibRoot -Directory |
    Where-Object {
        Test-Path -LiteralPath (Join-Path $_.FullName 'um\x86\User32.Lib')
    } |
    Sort-Object { [version]$_.Name } -Descending |
    Select-Object -First 1 -ExpandProperty FullName
if (-not $windowsSdkUmLib) {
    throw 'A Windows SDK x86 import-library directory is required for the visual checkpoint.'
}
$windowsSdkUmLib = Join-Path $windowsSdkUmLib 'um\x86'

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = @(
        (Join-Path $vcRoot 'include'),
        (Join-Path $rebuildRoot 'include')
    ) -join ';'
    $env:LIB = @(
        (Join-Path $vcRoot 'lib'),
        $windowsSdkUmLib
    ) -join ';'

    $compileOptions = @('/nologo', '/c', '/W3', '/MT', '/GS')
    if ($Configuration -eq 'Release') {
        $compileOptions += @('/O2', '/Oy')
    } else {
        $compileOptions += @('/Od', '/Zi')
    }
    $visualBootCompileOptions = @($compileOptions)
    if ($visualBootUsesRetailAsset) {
        $visualBootCompileOptions += '/DFABLETLC_RETAIL_FRONTEND_ARTWORK'
    }
    if ($visualBootUsesRetailAnimation) {
        $visualBootCompileOptions += '/DFABLETLC_RETAIL_FRONTEND_ANIMATION'
    }
    if ($visualBootUsesRetailMenu) {
        $visualBootCompileOptions += '/DFABLETLC_RETAIL_FRONTEND_MENU'
    }
    if ($visualBootUsesRetailSubscreens) {
        $visualBootCompileOptions += '/DFABLETLC_RETAIL_FRONTEND_SUBSCREENS'
    }
    if ($visualBootUsesRetailUiSounds) {
        $visualBootCompileOptions += '/DFABLETLC_RETAIL_FRONTEND_SOUNDS'
    }
    function Invoke-VerifiedLeaf {
        param(
            [string]$Address,
            [string]$Description,
            [string]$Source,
            [string]$BehaviorSource,
            [string]$OutputStem,
            [string]$PassPattern,
            [string]$AllowMove
        )

        $object = Join-Path $outDir "$OutputStem.obj"
        $behaviorObject = Join-Path $outDir "$OutputStem-behavior.obj"
        $behaviorExecutable = Join-Path $outDir "$OutputStem-behavior.exe"

        & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
            "/Fo$object" $Source
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $object)) {
            throw "Failed to compile $Description."
        }

        $checkArguments = @(
            $bootObjectChecker,
            '--root',
            $workspaceRoot,
            '--object',
            $object,
            '--address',
            $Address
        )
        if ($AllowMove) {
            $checkArguments += @('--allow-move', $AllowMove)
        }
        & python @checkArguments
        if ($LASTEXITCODE -ne 0) {
            throw "$Description failed its retail parity/residue gate."
        }

        & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
            "/Fo$behaviorObject" $BehaviorSource
        if (
            $LASTEXITCODE -ne 0 -or
            -not (Test-Path -LiteralPath $behaviorObject)
        ) {
            throw "Failed to compile the $Description behavior fixture."
        }

        & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
            "/out:$behaviorExecutable" $object $behaviorObject
        if (
            $LASTEXITCODE -ne 0 -or
            -not (Test-Path -LiteralPath $behaviorExecutable)
        ) {
            throw "Failed to link the $Description behavior fixture."
        }

        $behaviorOutput = & $behaviorExecutable 2>&1
        $behaviorExitCode = $LASTEXITCODE
        $behaviorOutput | Write-Output
        if (
            $behaviorExitCode -ne 0 -or
            (($behaviorOutput -join "`n") -notmatch [regex]::Escape($PassPattern))
        ) {
            throw "$Description behavior fixture failed with exit code $behaviorExitCode."
        }
    }

    Invoke-VerifiedLeaf `
        -Address '0099e4b0' `
        -Description 'CCharString default constructor' `
        -Source $charStringDefaultSource `
        -BehaviorSource $charStringDefaultBehaviorSource `
        -OutputStem 'char-string-default-constructor' `
        -PassPattern $charStringDefaultPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099af10' `
        -Description 'CWideString const-pointer conversion' `
        -Source $wideStringConversionSource `
        -BehaviorSource $wideStringConversionBehaviorSource `
        -OutputStem 'wide-string-conversion' `
        -PassPattern $wideStringConversionPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b150' `
        -Description 'CWideString wide-literal inequality comparison' `
        -Source $wideStringNotEqualSource `
        -BehaviorSource $wideStringNotEqualBehaviorSource `
        -OutputStem 'wide-string-not-equal' `
        -PassPattern $wideStringNotEqualPassPattern

    Invoke-VerifiedLeaf `
        -Address '0040d400' `
        -Description 'CUserProfileManager profile-name load/save dispatcher' `
        -Source $userProfileSetProfileNameSource `
        -BehaviorSource $userProfileSetProfileNameBehaviorSource `
        -OutputStem 'user-profile-set-profile-name' `
        -PassPattern $userProfileSetProfileNamePassPattern

    Invoke-VerifiedLeaf `
        -Address '00401b80' `
        -Description 'GFUninitialise shutdown coordinator' `
        -Source $gfUninitialiseSource `
        -BehaviorSource $gfUninitialiseBehaviorSource `
        -OutputStem 'gf-uninitialise' `
        -PassPattern $gfUninitialisePassPattern

    Invoke-VerifiedLeaf `
        -Address '00401c00' `
        -Description 'GFHandleSystemInitError message dispatcher' `
        -Source $gfHandleSystemInitErrorSource `
        -BehaviorSource $gfHandleSystemInitErrorBehaviorSource `
        -OutputStem 'gf-handle-system-init-error' `
        -PassPattern $gfHandleSystemInitErrorPassPattern

    Invoke-VerifiedLeaf `
        -Address '00412f90' `
        -Description 'CGame play and component ownership coordinator' `
        -Source $gamePlaySource `
        -BehaviorSource $gamePlayBehaviorSource `
        -OutputStem 'cgame-play' `
        -PassPattern $gamePlayPassPattern

    Invoke-VerifiedLeaf `
        -Address '00413c50' `
        -Description 'retail console-variable registrar' `
        -Source $initialiseConsoleVariablesSource `
        -BehaviorSource $initialiseConsoleVariablesBehaviorSource `
        -OutputStem 'initialise-console-variables' `
        -PassPattern $initialiseConsoleVariablesPassPattern

    Invoke-VerifiedLeaf `
        -Address '004023f0' `
        -Description 'localized window-title ownership coordinator' `
        -Source $getWindowTitleSource `
        -BehaviorSource $getWindowTitleBehaviorSource `
        -OutputStem 'get-window-title' `
        -PassPattern $getWindowTitlePassPattern

    Invoke-VerifiedLeaf `
        -Address '00405650' `
        -Description 'NUISystem CIME initialization coordinator' `
        -Source $imeInitialiseSource `
        -BehaviorSource $imeInitialiseBehaviorSource `
        -OutputStem 'cime-initialise' `
        -PassPattern $imeInitialisePassPattern

    Invoke-VerifiedLeaf `
        -Address '00418c3b' `
        -Description 'CMainGameComponent LUG-to-MET generator' `
        -Source $generateMetFilesSource `
        -BehaviorSource $generateMetFilesBehaviorSource `
        -OutputStem 'generate-met-files' `
        -PassPattern $generateMetFilesPassPattern

    Invoke-VerifiedLeaf `
        -Address '00401b20' `
        -Description 'configuration-detection release coordinator' `
        -Source $freeConfigDetectionSource `
        -BehaviorSource $freeConfigDetectionBehaviorSource `
        -OutputStem 'free-config-detection' `
        -PassPattern $freeConfigDetectionPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a6610' `
        -Description 'CSystemManager initialization coordinator' `
        -Source $systemManagerInitialiseSource `
        -BehaviorSource $systemManagerInitialiseBehaviorSource `
        -OutputStem 'system-manager-initialise' `
        -PassPattern $systemManagerInitialisePassPattern

    Invoke-VerifiedLeaf `
        -Address '00401284' `
        -Description 'CRT process-exit import thunk' `
        -Source $exitThunkSource `
        -BehaviorSource $exitThunkBehaviorSource `
        -OutputStem 'crt-exit-thunk' `
        -PassPattern $exitThunkPassPattern

    Invoke-VerifiedLeaf `
        -Address '00401fe0' `
        -Description 'EULA DLL coordinator' `
        -Source $doEulaSource `
        -BehaviorSource $doEulaBehaviorSource `
        -OutputStem 'do-eula-things' `
        -PassPattern $doEulaPassPattern

    Invoke-VerifiedLeaf `
        -Address '004017b0' `
        -Description 'hardware configuration-detection coordinator' `
        -Source $configDetectionSource `
        -BehaviorSource $configDetectionBehaviorSource `
        -OutputStem 'gf-config-detection' `
        -PassPattern $configDetectionPassPattern

    Invoke-VerifiedLeaf `
        -Address '004045c0' `
        -Description 'CPersistContext signed-char transfer dispatcher' `
        -Source $persistSignedCharSource `
        -BehaviorSource $persistSignedCharBehaviorSource `
        -OutputStem 'persist-signed-char' `
        -PassPattern $persistSignedCharPassPattern

    $persistSignedCharObject =
        Join-Path $outDir 'persist-signed-char.obj'
    foreach ($persistCaseAddress in @(
        '004045db',
        '004045ef',
        '00404608',
        '00404622',
        '00404647'
    )) {
        & python $bootObjectChecker `
            --root $workspaceRoot `
            --object $persistSignedCharObject `
            --address $persistCaseAddress
        if ($LASTEXITCODE -ne 0) {
            throw (
                'CPersistContext signed-char transfer case ' +
                "$persistCaseAddress failed retail parity."
            )
        }
    }

    Invoke-VerifiedLeaf `
        -Address '004046b0' `
        -Description 'CStringParser comment-list destructor' `
        -Source $stringParserCommentListDestructorSource `
        -BehaviorSource $stringParserCommentListDestructorBehaviorSource `
        -OutputStem 'string-parser-comment-list-destructor' `
        -PassPattern $stringParserCommentListDestructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '00405350' `
        -Description 'NUISystem CIME settings loader' `
        -Source $imeSettingsSource `
        -BehaviorSource $imeSettingsBehaviorSource `
        -OutputStem 'ime-settings' `
        -PassPattern $imeSettingsPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a76c0' `
        -Description 'bank-file-manager header directory setter' `
        -Source $bankHeaderDirectorySource `
        -BehaviorSource $bankHeaderDirectoryBehaviorSource `
        -OutputStem 'bank-header-directory' `
        -PassPattern $bankHeaderDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a430' `
        -Description 'My Documents directory resolver' `
        -Source $myDocumentsDirectorySource `
        -BehaviorSource $myDocumentsDirectoryBehaviorSource `
        -OutputStem 'my-documents-directory' `
        -PassPattern $myDocumentsDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a540' `
        -Description 'save directory resolver' `
        -Source $saveDirectorySource `
        -BehaviorSource $saveDirectoryBehaviorSource `
        -OutputStem 'save-directory' `
        -PassPattern $saveDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '00403e40' `
        -Description 'physics-faces counted-pointer reset' `
        -Source $physicsFacesCountedResetSource `
        -BehaviorSource $physicsFacesCountedResetBehaviorSource `
        -OutputStem 'physics-faces-counted-reset' `
        -PassPattern $physicsFacesCountedResetPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b800' `
        -Description 'CWideString literal assignment' `
        -Source $wideStringAssignLiteralSource `
        -BehaviorSource $wideStringAssignLiteralBehaviorSource `
        -OutputStem 'wide-string-assign-literal' `
        -PassPattern $wideStringAssignLiteralPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099be70' `
        -Description 'CWideString wide-string concatenation' `
        -Source $wideStringAddWideSource `
        -BehaviorSource $wideStringAddWideBehaviorSource `
        -OutputStem 'wide-string-add-wide' `
        -PassPattern $wideStringAddWidePassPattern

    Invoke-VerifiedLeaf `
        -Address '009d4c30' `
        -Description 'definition string table loader' `
        -Source $defStringTableLoadSource `
        -BehaviorSource $defStringTableLoadBehaviorSource `
        -OutputStem 'definition-string-table-load' `
        -PassPattern $defStringTableLoadPassPattern

    Invoke-VerifiedLeaf `
        -Address '009c85a0' `
        -Description 'game-text data-bank constructor' `
        -Source $gameTextDataBankConstructorSource `
        -BehaviorSource $gameTextDataBankConstructorBehaviorSource `
        -OutputStem 'game-text-data-bank-constructor' `
        -PassPattern $gameTextDataBankConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009c9070' `
        -Description 'game-text group-entry acquisition' `
        -Source $gameTextGetGroupEntrySource `
        -BehaviorSource $gameTextGetGroupEntryBehaviorSource `
        -OutputStem 'game-text-get-group-entry' `
        -PassPattern $gameTextGetGroupEntryPassPattern

    Invoke-VerifiedLeaf `
        -Address '00402130' `
        -Description 'My Documents write-permission probe' `
        -Source $myDocumentsWritePermissionsSource `
        -BehaviorSource $myDocumentsWritePermissionsBehaviorSource `
        -OutputStem 'my-documents-write-permissions' `
        -PassPattern $myDocumentsWritePermissionsPassPattern

    Invoke-VerifiedLeaf `
        -Address '009ac4d0' `
        -Description 'retail-bank ownership constructor' `
        -Source $retailBankConstructorSource `
        -BehaviorSource $retailBankConstructorBehaviorSource `
        -OutputStem 'retail-bank-constructor' `
        -PassPattern $retailBankConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0098dfd0' `
        -Description 'threaded-file constructor' `
        -Source $threadedFileConstructorSource `
        -BehaviorSource $threadedFileConstructorBehaviorSource `
        -OutputStem 'threaded-file-constructor' `
        -PassPattern $threadedFileConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a9c80' `
        -Description 'threaded-file counted reset' `
        -Source $threadedFileCountedResetSource `
        -BehaviorSource $threadedFileCountedResetBehaviorSource `
        -OutputStem 'threaded-file-counted-reset' `
        -PassPattern $threadedFileCountedResetPassPattern

    Invoke-VerifiedLeaf `
        -Address '0098e1e0' `
        -Description 'threaded-file open' `
        -Source $threadedFileOpenSource `
        -BehaviorSource $threadedFileOpenBehaviorSource `
        -OutputStem 'threaded-file-open' `
        -PassPattern $threadedFileOpenPassPattern

    Invoke-VerifiedLeaf `
        -Address '009ac530' `
        -Description 'contained-bank map indexing' `
        -Source $containedBankMapIndexSource `
        -BehaviorSource $containedBankMapIndexBehaviorSource `
        -OutputStem 'contained-bank-map-index' `
        -PassPattern $containedBankMapIndexPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a8840' `
        -Description 'retail BIGB bank opening and header parse' `
        -Source $openRetailBankSource `
        -BehaviorSource $openRetailBankBehaviorSource `
        -OutputStem 'open-retail-bank' `
        -PassPattern $openRetailBankPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a8170' `
        -Description 'development bank INI parser' `
        -Source $openIniFileSource `
        -BehaviorSource $openIniFileBehaviorSource `
        -OutputStem 'open-ini-file' `
        -PassPattern $openIniFilePassPattern

    Invoke-VerifiedLeaf `
        -Address '0040d2a0' `
        -Description 'CUserProfileManager counted singleton getter' `
        -Source $userProfileSingletonGetSource `
        -BehaviorSource $userProfileSingletonGetBehaviorSource `
        -OutputStem 'user-profile-singleton-get' `
        -PassPattern $userProfileSingletonGetPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099cf10' `
        -Description 'CSystemRegistry constructor' `
        -Source $systemRegistryConstructorSource `
        -BehaviorSource $systemRegistryConstructorBehaviorSource `
        -OutputStem 'system-registry-constructor' `
        -PassPattern $systemRegistryConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099cf70' `
        -Description 'CSystemRegistry destructor' `
        -Source $systemRegistryDestructorSource `
        -BehaviorSource $systemRegistryDestructorBehaviorSource `
        -OutputStem 'system-registry-destructor' `
        -PassPattern $systemRegistryDestructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099d100' `
        -Description 'CSystemRegistry BOOL writer' `
        -Source $systemRegistryWriteBoolSource `
        -BehaviorSource $systemRegistryWriteBoolBehaviorSource `
        -OutputStem 'system-registry-write-bool' `
        -PassPattern $systemRegistryWriteBoolPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b6b0' `
        -Description 'CWideString wide-literal constructor' `
        -Source $wideLiteralConstructorSource `
        -BehaviorSource $wideLiteralConstructorBehaviorSource `
        -OutputStem 'wide-literal-constructor' `
        -PassPattern $wideLiteralConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b7d0' `
        -Description 'CWideString copy assignment' `
        -Source $wideStringCopyAssignmentSource `
        -BehaviorSource $wideStringCopyAssignmentBehaviorSource `
        -OutputStem 'wide-string-copy-assignment' `
        -PassPattern $wideStringCopyAssignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099efb0' `
        -Description 'CCharString counted-storage copy assignment' `
        -Source $charStringCopyAssignmentSource `
        -BehaviorSource $charStringCopyAssignmentBehaviorSource `
        -OutputStem 'char-string-copy-assignment' `
        -PassPattern $charStringCopyAssignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a76d0' `
        -Description 'basic retail-bank navigator setter' `
        -Source $retailBankNavigatorSource `
        -BehaviorSource $retailBankNavigatorBehaviorSource `
        -OutputStem 'retail-bank-navigator' `
        -PassPattern $retailBankNavigatorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a8150' `
        -Description 'basic retail-bank alias wrapper' `
        -Source $retailBankAliasSource `
        -BehaviorSource $retailBankAliasBehaviorSource `
        -OutputStem 'retail-bank-alias' `
        -PassPattern $retailBankAliasPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a4ec0' `
        -Description 'global system-manager accessor' `
        -Source $getSystemManagerSource `
        -BehaviorSource $getSystemManagerBehaviorSource `
        -OutputStem 'get-system-manager' `
        -PassPattern $getSystemManagerPassPattern

    Invoke-VerifiedLeaf `
        -Address '009a0ef0' `
        -Description 'CXMVPlayer left-aligned text setter' `
        -Source $movieLeftAlignSource `
        -BehaviorSource $movieLeftAlignBehaviorSource `
        -OutputStem 'movie-left-align' `
        -PassPattern $movieLeftAlignPassPattern

    Invoke-VerifiedLeaf `
        -Address '00404a80' `
        -Description 'global IME primitive accessor' `
        -Source $imePrimitiveSource `
        -BehaviorSource $imePrimitiveBehaviorSource `
        -OutputStem 'ime-primitive' `
        -PassPattern $imePrimitivePassPattern

    Invoke-VerifiedLeaf `
        -Address '004014a0' `
        -Description 'CRC table accumulator' `
        -Source $crcCalcSource `
        -BehaviorSource $crcCalcBehaviorSource `
        -OutputStem 'crc-calc' `
        -PassPattern $crcCalcPassPattern

    Invoke-VerifiedLeaf `
        -Address '00499ce0' `
        -Description 'CProgressDisplay constructor' `
        -Source $progressDisplayConstructorSource `
        -BehaviorSource $progressDisplayConstructorBehaviorSource `
        -OutputStem 'progress-display-constructor' `
        -PassPattern $progressDisplayConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0049b460' `
        -Description 'CProgressDisplay active-state query' `
        -Source $progressDisplayIsActiveSource `
        -BehaviorSource $progressDisplayIsActiveBehaviorSource `
        -OutputStem 'progress-display-is-active' `
        -PassPattern $progressDisplayIsActivePassPattern

    Invoke-VerifiedLeaf `
        -Address '00497b30' `
        -Description 'CProgressDisplay text-bank selector' `
        -Source $progressDisplayTextBankSource `
        -BehaviorSource $progressDisplayTextBankBehaviorSource `
        -OutputStem 'progress-display-text-bank' `
        -PassPattern $progressDisplayTextBankPassPattern

    Invoke-VerifiedLeaf `
        -Address '00499a70' `
        -Description 'CProgressDisplay text-mode setter' `
        -Source $progressDisplayTextModeSource `
        -BehaviorSource $progressDisplayTextModeBehaviorSource `
        -OutputStem 'progress-display-text-mode' `
        -PassPattern $progressDisplayTextModePassPattern

    Invoke-VerifiedLeaf `
        -Address '009e9fd0' `
        -Description 'retained progress-display owner assignment' `
        -Source $setProgressDisplaySource `
        -BehaviorSource $setProgressDisplayBehaviorSource `
        -OutputStem 'set-progress-display' `
        -PassPattern $setProgressDisplayPassPattern

    Invoke-VerifiedLeaf `
        -Address '009ea060' `
        -Description 'retained progress-display owner query' `
        -Source $getProgressDisplaySource `
        -BehaviorSource $getProgressDisplayBehaviorSource `
        -OutputStem 'get-progress-display' `
        -PassPattern $getProgressDisplayPassPattern

    Invoke-VerifiedLeaf `
        -Address '00403b10' `
        -Description 'CSystemManagerInit constructor' `
        -Source $systemManagerInitSource `
        -BehaviorSource $systemManagerInitBehaviorSource `
        -OutputStem 'system-manager-init-constructor' `
        -PassPattern $systemManagerInitPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d8250' `
        -Description 'NProfileTimer end-profile no-op' `
        -Source $profileEndSource `
        -BehaviorSource $profileEndBehaviorSource `
        -OutputStem 'profile-end' `
        -PassPattern $profileEndPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d5240' `
        -Description 'CBankFileAsync failure-handling policy setter' `
        -Source $asyncFailureHandlingSource `
        -BehaviorSource $asyncFailureHandlingBehaviorSource `
        -OutputStem 'async-failure-handling' `
        -PassPattern $asyncFailureHandlingPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d81e0' `
        -Description 'TLC startup-latch clear leaf' `
        -Source $startupLatchSource `
        -BehaviorSource $startupLatchBehaviorSource `
        -OutputStem 'startup-latch-clear' `
        -PassPattern $startupLatchPassPattern

    # Avoid "installer" in the executable name: Windows' legacy installer
    # detection otherwise requests elevation before this manifest-free VC7.1
    # console fixture can run.
    Invoke-VerifiedLeaf `
        -Address '00404440' `
        -Description 'CFileInstaller counted singleton retrieval' `
        -Source $fileInstallerGetSource `
        -BehaviorSource $fileInstallerGetBehaviorSource `
        -OutputStem 'cfi-singleton-get' `
        -PassPattern $fileInstallerGetPassPattern

    Invoke-VerifiedLeaf `
        -Address '00415530' `
        -Description 'default-language narrow-string factory' `
        -Source $defaultLanguageNameSource `
        -BehaviorSource $defaultLanguageNameBehaviorSource `
        -OutputStem 'default-language-name' `
        -PassPattern $defaultLanguageNamePassPattern

    Invoke-VerifiedLeaf `
        -Address '00415260' `
        -Description 'language-specific font-bank name factory' `
        -Source $fontBankNameSource `
        -BehaviorSource $fontBankNameBehaviorSource `
        -OutputStem 'font-bank-name' `
        -PassPattern $fontBankNamePassPattern

    Invoke-VerifiedLeaf `
        -Address '00415440' `
        -Description 'language-specific streaming-font bank factory' `
        -Source $streamingFontBankNameSource `
        -BehaviorSource $streamingFontBankNameBehaviorSource `
        -OutputStem 'streaming-font-bank-name' `
        -PassPattern $streamingFontBankNamePassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a0a0' `
        -Description 'first miscellaneous-resource directory factory' `
        -Source $miscDirectoryASource `
        -BehaviorSource $miscDirectoryABehaviorSource `
        -OutputStem 'misc-directory-a' `
        -PassPattern $miscDirectoryAPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a0c0' `
        -Description 'second miscellaneous-resource directory factory' `
        -Source $miscDirectoryBSource `
        -BehaviorSource $miscDirectoryBBehaviorSource `
        -OutputStem 'misc-directory-b' `
        -PassPattern $miscDirectoryBPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a100' `
        -Description 'graphics-resource directory factory' `
        -Source $graphicsDirectorySource `
        -BehaviorSource $graphicsDirectoryBehaviorSource `
        -OutputStem 'graphics-directory' `
        -PassPattern $graphicsDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a120' `
        -Description 'first language-resource directory factory' `
        -Source $languageDirectoryASource `
        -BehaviorSource $languageDirectoryABehaviorSource `
        -OutputStem 'language-directory-a' `
        -PassPattern $languageDirectoryAPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a1e0' `
        -Description 'shader-resource directory factory' `
        -Source $shadersDirectorySource `
        -BehaviorSource $shadersDirectoryBehaviorSource `
        -OutputStem 'shaders-directory' `
        -PassPattern $shadersDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a180' `
        -Description 'cache-resource directory factory' `
        -Source $cacheDirectorySource `
        -BehaviorSource $cacheDirectoryBehaviorSource `
        -OutputStem 'cache-directory' `
        -PassPattern $cacheDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a330' `
        -Description 'second language-resource directory factory' `
        -Source $languageDirectoryBSource `
        -BehaviorSource $languageDirectoryBBehaviorSource `
        -OutputStem 'language-directory-b' `
        -PassPattern $languageDirectoryBPassPattern

    Invoke-VerifiedLeaf `
        -Address '0041a4c0' `
        -Description 'auto-save checkpoint directory factory' `
        -Source $checkpointDirectorySource `
        -BehaviorSource $checkpointDirectoryBehaviorSource `
        -OutputStem 'checkpoint-directory' `
        -PassPattern $checkpointDirectoryPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d4a40' `
        -Description 'definition string-table pathname setter' `
        -Source $defTablePathSource `
        -BehaviorSource $defTablePathBehaviorSource `
        -OutputStem 'def-table-path' `
        -PassPattern $defTablePathPassPattern

    Invoke-VerifiedLeaf `
        -Address '009c7a40' `
        -Description 'bank-handle wide-string accessor' `
        -Source $bankHandleSource `
        -BehaviorSource $bankHandleBehaviorSource `
        -OutputStem 'bank-handle' `
        -PassPattern $bankHandlePassPattern

    Invoke-VerifiedLeaf `
        -Address '009cc410' `
        -Description 'vector-map exact-key lookup' `
        -Source $vectorMapLookupSource `
        -BehaviorSource $vectorMapLookupBehaviorSource `
        -OutputStem 'vector-map-lookup' `
        -PassPattern $vectorMapLookupPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b6a0' `
        -Description 'CWideString narrow-string conversion factory' `
        -Source $wideStringFromCharSource `
        -BehaviorSource $wideStringFromCharBehaviorSource `
        -OutputStem 'wide-string-from-char' `
        -PassPattern $wideStringFromCharPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b720' `
        -Description 'CWideString counted-storage copy constructor' `
        -Source $wideStringCopySource `
        -BehaviorSource $wideStringCopyBehaviorSource `
        -OutputStem 'wide-string-copy' `
        -PassPattern $wideStringCopyPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099e480' `
        -Description 'CCharString to CWideString conversion' `
        -Source $charToWideStringSource `
        -BehaviorSource $charToWideStringBehaviorSource `
        -OutputStem 'char-to-wide-string' `
        -PassPattern $charToWideStringPassPattern

    Invoke-VerifiedLeaf `
        -Address '009bc890' `
        -Description 'primary left-alignment flag setter' `
        -Source $primaryLeftAlignmentSource `
        -BehaviorSource $primaryLeftAlignmentBehaviorSource `
        -OutputStem 'primary-left-alignment' `
        -PassPattern $primaryLeftAlignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '009bc8a0' `
        -Description 'secondary left-alignment flag setter' `
        -Source $secondaryLeftAlignmentSource `
        -BehaviorSource $secondaryLeftAlignmentBehaviorSource `
        -OutputStem 'secondary-left-alignment' `
        -PassPattern $secondaryLeftAlignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099a2e0' `
        -Description 'first folded CBase vtable restore' `
        -Source $cbaseRestoreASource `
        -BehaviorSource $cbaseRestoreABehaviorSource `
        -OutputStem 'cbase-restore-a' `
        -PassPattern $cbaseRestoreAPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099a300' `
        -Description 'second folded CBase vtable restore' `
        -Source $cbaseRestoreBSource `
        -BehaviorSource $cbaseRestoreBBehaviorSource `
        -OutputStem 'cbase-restore-b' `
        -PassPattern $cbaseRestoreBPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099bf30' `
        -Description 'wide-string plus UTF-16 literal' `
        -Source $wideStringAddRightSource `
        -BehaviorSource $wideStringAddRightBehaviorSource `
        -OutputStem 'wide-string-add-right' `
        -PassPattern $wideStringAddRightPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099bff0' `
        -Description 'UTF-16 literal plus wide-string' `
        -Source $wideStringAddLeftSource `
        -BehaviorSource $wideStringAddLeftBehaviorSource `
        -OutputStem 'wide-string-add-left' `
        -PassPattern $wideStringAddLeftPassPattern

    Invoke-VerifiedLeaf `
        -Address '00999230' `
        -Description 'Phase 3 expanded-path existence probe' `
        -Source $pathExistsSource `
        -BehaviorSource $pathExistsBehaviorSource `
        -OutputStem 'path-exists' `
        -PassPattern $pathExistsPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099ad80' `
        -Description 'Phase 3 disk-file construction' `
        -Source $diskFileConstructorSource `
        -BehaviorSource $diskFileConstructorBehaviorSource `
        -OutputStem 'disk-file-constructor' `
        -PassPattern $diskFileConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099a920' `
        -Description 'Phase 3 disk-file close' `
        -Source $diskFileCloseSource `
        -BehaviorSource $diskFileCloseBehaviorSource `
        -OutputStem 'disk-file-close' `
        -PassPattern $diskFileClosePassPattern

    Invoke-VerifiedLeaf `
        -Address '0099f2e0' `
        -Description 'Phase 3 settings-file read' `
        -Source $charStringLoadFileSource `
        -BehaviorSource $charStringLoadFileBehaviorSource `
        -OutputStem 'char-string-load-file' `
        -PassPattern $charStringLoadFilePassPattern

    Invoke-VerifiedLeaf `
        -Address '00404720' `
        -Description 'Phase 3 settings string-parser construction' `
        -Source $stringParserConstructorSource `
        -BehaviorSource $stringParserConstructorBehaviorSource `
        -OutputStem 'string-parser-constructor' `
        -PassPattern $stringParserConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009badd0' `
        -Description 'Phase 3 text persistence-context construction' `
        -Source $persistContextTextConstructorSource `
        -BehaviorSource $persistContextTextConstructorBehaviorSource `
        -OutputStem 'persist-context-text-constructor' `
        -PassPattern $persistContextTextConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009f2d60' `
        -Description 'movie UI-surface copy construction' `
        -Source $surfaceCopyConstructorSource `
        -BehaviorSource $surfaceCopyConstructorBehaviorSource `
        -OutputStem 'surface-copy-constructor' `
        -PassPattern $surfaceCopyConstructorPassPattern

    Invoke-VerifiedLeaf `
        -Address '009f2f10' `
        -Description 'movie mip-surface initialization' `
        -Source $surfaceInitialiseMipmapSource `
        -BehaviorSource $surfaceInitialiseMipmapBehaviorSource `
        -OutputStem 'surface-initialise-mipmap' `
        -PassPattern $surfaceInitialiseMipmapPassPattern

    Invoke-VerifiedLeaf `
        -Address '009f37e0' `
        -Description 'movie decoder-to-UI surface copy' `
        -Source $surfaceCopyFromSurfaceSource `
        -BehaviorSource $surfaceCopyFromSurfaceBehaviorSource `
        -OutputStem 'surface-copy-from-surface' `
        -PassPattern $surfaceCopyFromSurfacePassPattern

    Invoke-VerifiedLeaf `
        -Address '009fa4e0' `
        -Description 'movie texture mip-chain copy coordinator' `
        -Source $textureCopyFromTextureSource `
        -BehaviorSource $textureCopyFromTextureBehaviorSource `
        -OutputStem 'texture-copy-from-texture' `
        -PassPattern $textureCopyFromTexturePassPattern

    Invoke-VerifiedLeaf `
        -Address '00548510' `
        -Description 'CMovie filename assignment' `
        -Source $cmovieSetMovieSource `
        -BehaviorSource $cmovieSetMovieBehaviorSource `
        -OutputStem 'cmovie-set-movie' `
        -PassPattern $cmovieSetMoviePassPattern

    Invoke-VerifiedLeaf `
        -Address '00548520' `
        -Description 'CMovie playing-state query' `
        -Source $cmovieIsPlayingSource `
        -BehaviorSource $cmovieIsPlayingBehaviorSource `
        -OutputStem 'cmovie-is-playing' `
        -PassPattern $cmovieIsPlayingPassPattern

    Invoke-VerifiedLeaf `
        -Address '00a3b0f0' `
        -Description 'CVideoSys filter-state wait loop' `
        -Source $videoSysWaitForStateSource `
        -BehaviorSource $videoSysWaitForStateBehaviorSource `
        -OutputStem 'cvideosys-wait-for-state' `
        -PassPattern $videoSysWaitForStatePassPattern

    Invoke-VerifiedLeaf `
        -Address '00a3b1c0' `
        -Description 'CVideoSys pause transition' `
        -Source $videoSysPauseSource `
        -BehaviorSource $videoSysPauseBehaviorSource `
        -OutputStem 'cvideosys-pause' `
        -PassPattern $videoSysPausePassPattern

    Invoke-VerifiedLeaf `
        -Address '00a3b1f0' `
        -Description 'CVideoSys stop transition' `
        -Source $videoSysStopSource `
        -BehaviorSource $videoSysStopBehaviorSource `
        -OutputStem 'cvideosys-stop' `
        -PassPattern $videoSysStopPassPattern

    Invoke-VerifiedLeaf `
        -Address '00a3b1a0' `
        -Description 'CVideoSys media-control run transition' `
        -Source $videoSysAttemptToPlaySource `
        -BehaviorSource $videoSysAttemptToPlayBehaviorSource `
        -OutputStem 'cvideosys-attempt-to-play' `
        -PassPattern $videoSysAttemptToPlayPassPattern

    Invoke-VerifiedLeaf `
        -Address '00a3b320' `
        -Description 'CVideoSys decoder-texture query' `
        -Source $videoSysGetTextureSource `
        -BehaviorSource $videoSysGetTextureBehaviorSource `
        -OutputStem 'cvideosys-get-texture' `
        -PassPattern $videoSysGetTexturePassPattern

    Invoke-VerifiedLeaf `
        -Address '004022b0' `
        -Description 'full GFInitialise coordinator' `
        -Source $gfInitialiseSource `
        -BehaviorSource $gfInitialiseBehaviorSource `
        -OutputStem 'gfinitialise' `
        -PassPattern $gfInitialisePassPattern

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$retailObject" $retailSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $retailObject)) {
        throw 'Failed to compile the verified retail function.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$bootstrapObject" $bootstrapSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $bootstrapObject)) {
        throw 'Failed to compile the reconstruction bootstrap.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$winMainObject" $winMainSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainObject)) {
        throw 'Failed to compile the recovered WinMain wrapper.'
    }

    & python $bootObjectChecker --root $workspaceRoot --object $winMainObject --address 00403480
    if ($LASTEXITCODE -ne 0) {
        throw 'The recovered WinMain wrapper differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$winMainBehaviorObject" $winMainBehaviorSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainBehaviorObject)) {
        throw 'Failed to compile the WinMain behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$stage1BoundaryObject" $stage1BoundarySource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $stage1BoundaryObject)) {
        throw 'Failed to compile the Stage 1 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$progressSetupObject" $progressSetupSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $progressSetupObject)) {
        throw 'Failed to compile the GFInitialise progress-display setup leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot --object $progressSetupObject --address 00413120
    if ($LASTEXITCODE -ne 0) {
        throw 'The progress-display setup leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$progressSetupBehaviorObject" $progressSetupBehaviorSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $progressSetupBehaviorObject)) {
        throw 'Failed to compile the progress-display setup behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$setCurrentPathObject" $setCurrentPathSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $setCurrentPathObject)) {
        throw 'Failed to compile the CAFile current-path leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $setCurrentPathObject --address 009974f0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CAFile current-path leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$setCurrentPathBehaviorObject" $setCurrentPathBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $setCurrentPathBehaviorObject)
    ) {
        throw 'Failed to compile the CAFile current-path behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$getProjectPathObject" $getProjectPathSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $getProjectPathObject)) {
        throw 'Failed to compile the CAFile project-path leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $getProjectPathObject --address 00997510
    if ($LASTEXITCODE -ne 0) {
        throw 'The CAFile project-path leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$getProjectPathBehaviorObject" $getProjectPathBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $getProjectPathBehaviorObject)
    ) {
        throw 'Failed to compile the CAFile project-path behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringConstructorObject" $wideStringConstructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorObject)
    ) {
        throw 'Failed to compile the CWideString default constructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $wideStringConstructorObject --address 0099aed0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CWideString default constructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringConstructorBehaviorObject" $wideStringConstructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorBehaviorObject)
    ) {
        throw 'Failed to compile the CWideString constructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringDestructorObject" $wideStringDestructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorObject)
    ) {
        throw 'Failed to compile the CWideString destructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $wideStringDestructorObject --address 0099b510
    if ($LASTEXITCODE -ne 0) {
        throw 'The CWideString destructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringDestructorBehaviorObject" $wideStringDestructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorBehaviorObject)
    ) {
        throw 'Failed to compile the CWideString destructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringConstructorObject" $charStringConstructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorObject)
    ) {
        throw 'Failed to compile the CCharString constructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $charStringConstructorObject --address 0099ebf0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CCharString constructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringConstructorBehaviorObject" $charStringConstructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorBehaviorObject)
    ) {
        throw 'Failed to compile the CCharString constructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringDestructorObject" $charStringDestructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorObject)
    ) {
        throw 'Failed to compile the CCharString destructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $charStringDestructorObject --address 0099eae0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CCharString destructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringDestructorBehaviorObject" $charStringDestructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorBehaviorObject)
    ) {
        throw 'Failed to compile the CCharString destructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$profileStartObject" $profileStartSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $profileStartObject)) {
        throw 'Failed to compile the retail profile-start no-op.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $profileStartObject --address 009d8240
    if ($LASTEXITCODE -ne 0) {
        throw 'The profile-start no-op differs from retail.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$profileStartBehaviorObject" $profileStartBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $profileStartBehaviorObject)
    ) {
        throw 'Failed to compile the profile-start behavior fixture.'
    }

    $linkOptions = @(
        '/nologo',
        '/subsystem:console',
        "/out:$executable"
    )
    if ($Configuration -eq 'Debug') {
        $linkOptions += '/debug'
    }

    & (Join-Path $vcRoot 'bin\link.exe') @linkOptions $bootstrapObject $retailObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $executable)) {
        throw 'Failed to link the reconstruction bootstrap executable.'
    }

    $output = & $executable 2>&1
    $exitCode = $LASTEXITCODE
    $output | Write-Output
    if ($exitCode -ne 0 -or (($output -join "`n") -notmatch [regex]::Escape($passPattern))) {
        throw "Bootstrap execution failed with exit code $exitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$winMainBehaviorExecutable" $winMainObject $winMainBehaviorObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainBehaviorExecutable)) {
        throw 'Failed to link the WinMain behavior fixture.'
    }

    $winMainOutput = & $winMainBehaviorExecutable 2>&1
    $winMainExitCode = $LASTEXITCODE
    $winMainOutput | Write-Output
    if (
        $winMainExitCode -ne 0 -or
        (($winMainOutput -join "`n") -notmatch [regex]::Escape($winMainPassPattern))
    ) {
        throw "WinMain behavior fixture failed with exit code $winMainExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage1Executable" $winMainObject $stage1BoundaryObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $stage1Executable)) {
        throw 'Failed to link the Stage 1 startup executable.'
    }

    $stage1Process = Start-Process -FilePath $stage1Executable -Wait -PassThru
    if ($stage1Process.ExitCode -ne 0) {
        throw "Stage 1 startup executable failed with exit code $($stage1Process.ExitCode)."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$progressSetupBehaviorExecutable" `
        $progressSetupObject $progressSetupBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $progressSetupBehaviorExecutable)
    ) {
        throw 'Failed to link the progress-display setup behavior fixture.'
    }

    $progressSetupOutput = & $progressSetupBehaviorExecutable 2>&1
    $progressSetupExitCode = $LASTEXITCODE
    $progressSetupOutput | Write-Output
    if (
        $progressSetupExitCode -ne 0 -or
        (($progressSetupOutput -join "`n") -notmatch [regex]::Escape($progressSetupPassPattern))
    ) {
        throw "Progress-display setup fixture failed with exit code $progressSetupExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$setCurrentPathBehaviorExecutable" `
        $setCurrentPathObject $setCurrentPathBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $setCurrentPathBehaviorExecutable)
    ) {
        throw 'Failed to link the CAFile current-path behavior fixture.'
    }

    $setCurrentPathOutput = & $setCurrentPathBehaviorExecutable 2>&1
    $setCurrentPathExitCode = $LASTEXITCODE
    $setCurrentPathOutput | Write-Output
    if (
        $setCurrentPathExitCode -ne 0 -or
        (($setCurrentPathOutput -join "`n") -notmatch [regex]::Escape($setCurrentPathPassPattern))
    ) {
        throw "CAFile current-path fixture failed with exit code $setCurrentPathExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$getProjectPathBehaviorExecutable" `
        $getProjectPathObject $getProjectPathBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $getProjectPathBehaviorExecutable)
    ) {
        throw 'Failed to link the CAFile project-path behavior fixture.'
    }

    $getProjectPathOutput = & $getProjectPathBehaviorExecutable 2>&1
    $getProjectPathExitCode = $LASTEXITCODE
    $getProjectPathOutput | Write-Output
    if (
        $getProjectPathExitCode -ne 0 -or
        (($getProjectPathOutput -join "`n") -notmatch [regex]::Escape($getProjectPathPassPattern))
    ) {
        throw "CAFile project-path fixture failed with exit code $getProjectPathExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$wideStringConstructorBehaviorExecutable" `
        $wideStringConstructorObject $wideStringConstructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CWideString constructor behavior fixture.'
    }

    $wideStringConstructorOutput = & $wideStringConstructorBehaviorExecutable 2>&1
    $wideStringConstructorExitCode = $LASTEXITCODE
    $wideStringConstructorOutput | Write-Output
    if (
        $wideStringConstructorExitCode -ne 0 -or
        (
            ($wideStringConstructorOutput -join "`n") -notmatch
            [regex]::Escape($wideStringConstructorPassPattern)
        )
    ) {
        throw "CWideString constructor fixture failed with exit code $wideStringConstructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$wideStringDestructorBehaviorExecutable" `
        $wideStringConstructorObject `
        $wideStringDestructorObject `
        $wideStringDestructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CWideString destructor behavior fixture.'
    }

    $wideStringDestructorOutput = & $wideStringDestructorBehaviorExecutable 2>&1
    $wideStringDestructorExitCode = $LASTEXITCODE
    $wideStringDestructorOutput | Write-Output
    if (
        $wideStringDestructorExitCode -ne 0 -or
        (
            ($wideStringDestructorOutput -join "`n") -notmatch
            [regex]::Escape($wideStringDestructorPassPattern)
        )
    ) {
        throw "CWideString destructor fixture failed with exit code $wideStringDestructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$charStringConstructorBehaviorExecutable" `
        $charStringConstructorObject `
        $charStringConstructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CCharString constructor behavior fixture.'
    }

    $charStringConstructorOutput = & $charStringConstructorBehaviorExecutable 2>&1
    $charStringConstructorExitCode = $LASTEXITCODE
    $charStringConstructorOutput | Write-Output
    if (
        $charStringConstructorExitCode -ne 0 -or
        (
            ($charStringConstructorOutput -join "`n") -notmatch
            [regex]::Escape($charStringConstructorPassPattern)
        )
    ) {
        throw "CCharString constructor fixture failed with exit code $charStringConstructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$charStringDestructorBehaviorExecutable" `
        $charStringDestructorObject `
        $charStringDestructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CCharString destructor behavior fixture.'
    }

    $charStringDestructorOutput = & $charStringDestructorBehaviorExecutable 2>&1
    $charStringDestructorExitCode = $LASTEXITCODE
    $charStringDestructorOutput | Write-Output
    if (
        $charStringDestructorExitCode -ne 0 -or
        (
            ($charStringDestructorOutput -join "`n") -notmatch
            [regex]::Escape($charStringDestructorPassPattern)
        )
    ) {
        throw "CCharString destructor fixture failed with exit code $charStringDestructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$profileStartBehaviorExecutable" `
        $profileStartObject `
        $profileStartBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $profileStartBehaviorExecutable)
    ) {
        throw 'Failed to link the profile-start behavior fixture.'
    }

    $profileStartOutput = & $profileStartBehaviorExecutable 2>&1
    $profileStartExitCode = $LASTEXITCODE
    $profileStartOutput | Write-Output
    if (
        $profileStartExitCode -ne 0 -or
        (($profileStartOutput -join "`n") -notmatch [regex]::Escape($profileStartPassPattern))
    ) {
        throw "Profile-start fixture failed with exit code $profileStartExitCode."
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase1Object" $gfmainPhase1Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1Object)
    ) {
        throw 'Failed to compile the GFMain Phase 1 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase2Object" $gfmainPhase2Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2Object)
    ) {
        throw 'Failed to compile the GFMain Phase 2 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase3Object" $gfmainPhase3Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase3Object)
    ) {
        throw 'Failed to compile the GFMain Phase 3 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase4Object" $gfmainPhase4Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase4Object)
    ) {
        throw 'Failed to compile the GFMain Phase 4 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase5Object" $gfmainPhase5Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase5Object)
    ) {
        throw 'Failed to compile the GFMain Phase 5 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase6Object" $gfmainPhase6Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase6Object)
    ) {
        throw 'Failed to compile the GFMain Phase 6 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase7Object" $gfmainPhase7Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase7Object)
    ) {
        throw 'Failed to compile the GFMain Phase 7 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase8Object" $gfmainPhase8Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase8Object)
    ) {
        throw 'Failed to compile the GFMain Phase 8 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase9Object" $gfmainPhase9Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase9Object)
    ) {
        throw 'Failed to compile the GFMain Phase 9 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase10Object" $gfmainPhase10Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase10Object)
    ) {
        throw 'Failed to compile the GFMain Phase 10 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainCompleteObject" $gfmainCompleteSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainCompleteObject)
    ) {
        throw 'Failed to compile the complete GFMain parent coordinator.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfInitialiseProgressPhaseObject" $gfInitialiseProgressPhaseSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfInitialiseProgressPhaseObject)
    ) {
        throw 'Failed to compile the GFInitialise progress integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfInitialiseEngineBoundaryObject" $gfInitialiseEngineBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfInitialiseEngineBoundaryObject)
    ) {
        throw 'Failed to compile the GFInitialise engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$phase6RuntimeBoundaryObject" `
        $phase6RuntimeBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $phase6RuntimeBoundaryObject)
    ) {
        throw 'Failed to compile the exact Phase 6 definition-table boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$systemManagerRuntimeBoundaryObject" `
        $systemManagerRuntimeBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $systemManagerRuntimeBoundaryObject)
    ) {
        throw 'Failed to compile the exact system-manager runtime boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$saveMetadataRuntimeBoundaryObject" `
        $saveMetadataRuntimeBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $saveMetadataRuntimeBoundaryObject)
    ) {
        throw 'Failed to compile the exact save-metadata runtime boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$phase8RuntimeBoundaryObject" `
        $phase8RuntimeBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $phase8RuntimeBoundaryObject)
    ) {
        throw 'Failed to compile the exact Phase 8 runtime boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$phase7WindowTitleRuntimeBoundaryObject" `
        $phase7WindowTitleRuntimeBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $phase7WindowTitleRuntimeBoundaryObject)
    ) {
        throw 'Failed to compile the exact Phase 7 window-title boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$progressDisplayStringBoundaryObject" `
        $progressDisplayStringBoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $progressDisplayStringBoundaryObject)
    ) {
        throw 'Failed to compile the progress-display string boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$stage2BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage2BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 2 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        "/Fo$stage3BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        "/Fo$stage3Phase3BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase3BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 3 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        "/Fo$stage3Phase4BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase4BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 4 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        "/Fo$stage3Phase5BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase5BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 5 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        "/Fo$stage3Phase6BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase6BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 6 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        "/Fo$stage3Phase7BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase7BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 7 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE8 `
        "/Fo$stage3Phase8BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase8BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 8 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE8 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE9 `
        "/Fo$stage3Phase9BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase9BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 9 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE8 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE9 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE10 `
        "/Fo$stage3Phase10BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Phase10BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 Phase 10 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE8 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE9 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE10 `
        /DFABLETLC_EXECUTE_EXACT_GFINITIALISE `
        /DFABLETLC_EXECUTE_EXACT_GFUNINITIALISE `
        /DFABLETLC_EXECUTE_EXACT_CIME `
        /DFABLETLC_EXECUTE_EXACT_DEFINITION_TABLE `
        /DFABLETLC_EXECUTE_EXACT_WRITE_PERMISSIONS `
        /DFABLETLC_EXECUTE_EXACT_SYSTEM_MANAGER `
        /DFABLETLC_EXECUTE_EXACT_SAVE_METADATA `
        /DFABLETLC_EXECUTE_EXACT_PHASE8 `
        /DFABLETLC_EXECUTE_EXACT_WINDOW_TITLE `
        "/Fo$stage3CompleteBoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3CompleteBoundaryObject)
    ) {
        throw 'Failed to compile the complete Stage 3 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE3 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE4 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE5 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE6 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE7 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE8 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE9 `
        /DFABLETLC_ENABLE_GFMAIN_PHASE10 `
        /DFABLETLC_EXECUTE_EXACT_GFUNINITIALISE `
        /DFABLETLC_EXECUTE_EXACT_CIME `
        /DFABLETLC_EXECUTE_EXACT_DEFINITION_TABLE `
        /DFABLETLC_EXECUTE_EXACT_WRITE_PERMISSIONS `
        /DFABLETLC_EXECUTE_EXACT_SYSTEM_MANAGER `
        /DFABLETLC_EXECUTE_EXACT_SAVE_METADATA `
        /DFABLETLC_EXECUTE_EXACT_PHASE8 `
        /DFABLETLC_EXECUTE_EXACT_WINDOW_TITLE `
        /DFABLETLC_ENABLE_VISUAL_BOOT `
        "/Fo$visualBoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBoundaryObject)
    ) {
        throw 'Failed to compile the authored visual engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$visualBootObject" $visualBootSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootObject)
    ) {
        throw 'Failed to compile the visual boot checkpoint.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$visualBootD3D9Object" $visualBootD3D9Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootD3D9Object)
    ) {
        throw 'Failed to compile the D3D9 visual presenter.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$resetWorldTransformObject" $resetWorldTransformSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $resetWorldTransformObject)
    ) {
        throw 'Failed to compile the recovered world-transform reset.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$retailVideoBridgeObject" $retailVideoBridgeSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $retailVideoBridgeObject)
    ) {
        throw 'Failed to compile the retail video bridge.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$videoFrameConversionObject" $videoFrameConversionSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $videoFrameConversionObject)
    ) {
        throw 'Failed to compile the recovered video-frame conversion.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$videoFramePublicationObject" $videoFramePublicationSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $videoFramePublicationObject)
    ) {
        throw 'Failed to compile the decoded-frame publication contract.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$videoFramePublicationBehaviorObject" `
        $videoFramePublicationBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $videoFramePublicationBehaviorObject)
    ) {
        throw 'Failed to compile the decoded-frame publication fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$frontendStartupSequenceObject" `
        $frontendStartupSequenceSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $frontendStartupSequenceObject)
    ) {
        throw 'Failed to compile the frontend startup sequence.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$frontendStartupSequenceBehaviorObject" `
        $frontendStartupSequenceBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $frontendStartupSequenceBehaviorObject)
    ) {
        throw 'Failed to compile the frontend startup sequence fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @visualBootCompileOptions `
        "/Fo$visualBootBehaviorObject" $visualBootBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootBehaviorObject)
    ) {
        throw 'Failed to compile the visual boot behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$render2DBatchPlanObject" $render2DBatchPlanSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $render2DBatchPlanObject)
    ) {
        throw 'Failed to compile the Render2D batch planner.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$render2DBatchPlanBehaviorObject" $render2DBatchPlanBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $render2DBatchPlanBehaviorObject)
    ) {
        throw 'Failed to compile the Render2D batch-plan fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$render2DDrawListAdapterObject" $render2DDrawListAdapterSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $render2DDrawListAdapterObject)
    ) {
        throw 'Failed to compile the Render2D draw-list adapter.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$render2DDrawListAdapterBehaviorObject" `
        $render2DDrawListAdapterBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $render2DDrawListAdapterBehaviorObject)
    ) {
        throw 'Failed to compile the Render2D draw-list adapter fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$attachTextureToStageObject" $attachTextureToStageSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $attachTextureToStageObject)
    ) {
        throw 'Failed to compile the recovered texture-stage attachment.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$realiseRenderStateObject" $realiseRenderStateSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $realiseRenderStateObject)
    ) {
        throw 'Failed to compile recovered render-state realisation.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$soldStateBlockObject" $soldStateBlockSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $soldStateBlockObject)
    ) {
        throw 'Failed to compile recovered Sold render-state block.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$updatePixelShaderObject" $updatePixelShaderSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $updatePixelShaderObject)
    ) {
        throw 'Failed to compile recovered pixel-shader update.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$displaySetViewportObject" $displaySetViewportSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $displaySetViewportObject)
    ) {
        throw 'Failed to compile recovered display viewport wrapper.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$displaySetIntegerViewportObject" $displaySetIntegerViewportSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $displaySetIntegerViewportObject)
    ) {
        throw 'Failed to compile recovered integer display viewport.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$postViewportShaderObject" $postViewportShaderSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $postViewportShaderObject)
    ) {
        throw 'Failed to compile recovered post-viewport shader notification.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$viewportE2Object" $viewportE2Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $viewportE2Object)
    ) {
        throw 'Failed to compile recovered viewport E2 target.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$setRenderWindowObject" $setRenderWindowSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $setRenderWindowObject)
    ) {
        throw 'Failed to compile the recovered Render2D window wrapper.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$clearRender2DVertexQueueObject" `
        $clearRender2DVertexQueueSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath `
            $clearRender2DVertexQueueObject)
    ) {
        throw 'Failed to compile recovered Render2D vertex cleanup.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$textureAssignmentObject" $textureAssignmentSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $textureAssignmentObject)
    ) {
        throw 'Failed to compile recovered texture assignment.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$textureUninitialiseObject" $textureUninitialiseSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $textureUninitialiseObject)
    ) {
        throw 'Failed to compile recovered texture uninitialisation.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$restoreRenderStateCaptureObject" $restoreRenderStateCaptureSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $restoreRenderStateCaptureObject)
    ) {
        throw 'Failed to compile recovered render-state capture restoration.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$textureCalcByteLengthObject" $textureCalcByteLengthSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $textureCalcByteLengthObject)
    ) {
        throw 'Failed to compile recovered texture byte-length calculation.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$pixelFormatGetColourDepthObject" $pixelFormatGetColourDepthSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $pixelFormatGetColourDepthObject)
    ) {
        throw 'Failed to compile recovered pixel-format colour depth.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$pixelFormatInitialiseObject" $pixelFormatInitialiseSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $pixelFormatInitialiseObject)
    ) {
        throw 'Failed to compile recovered pixel-format initialisation.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$pixelFormatTableObject" $pixelFormatTableSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $pixelFormatTableObject)
    ) {
        throw 'Failed to compile the recovered retail pixel-format table.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$textureInitialisePreallocatedObject" $textureInitialisePreallocatedSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $textureInitialisePreallocatedObject)
    ) {
        throw 'Failed to compile recovered preallocated texture initialisation.'
    }

    Add-Type -AssemblyName PresentationCore
    $pngStream = [System.IO.File]::OpenRead($visualBootArtwork)
    try {
        $decoder = New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
            $pngStream,
            [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
            [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
        if (
            $visualBootUsesRetailAsset -and
            (
                $decoder.Frames[0].PixelWidth -ne 640 -or
                $decoder.Frames[0].PixelHeight -ne 480
            )
        ) {
            throw (
                "Retail frontend artwork must be 640x480 after cropping; got " +
                "$($decoder.Frames[0].PixelWidth)x$($decoder.Frames[0].PixelHeight)."
            )
        }
        $encoder = New-Object System.Windows.Media.Imaging.BmpBitmapEncoder
        $encoder.Frames.Add($decoder.Frames[0])
        $bitmapStream = [System.IO.File]::Create($visualBootBitmap)
        try {
            $encoder.Save($bitmapStream)
        } finally {
            $bitmapStream.Dispose()
        }
    } finally {
        $pngStream.Dispose()
    }

    if ($visualBootUsesRetailAsset) {
        $titlePngStream = [System.IO.File]::OpenRead(
            $visualBootRetailTitle)
        $titleRightPngStream = [System.IO.File]::OpenRead(
            $visualBootRetailTitleRight)
        $promptPngStream = $null
        $legalPngStream = $null
        if ($visualBootUsesRetailPrompt) {
            $promptPngStream = [System.IO.File]::OpenRead(
                $visualBootRetailPrompt)
        }
        if ($visualBootUsesRetailLegal) {
            $legalPngStream = [System.IO.File]::OpenRead(
                $visualBootRetailLegal)
        }
        try {
            $titleDecoder =
                New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
                    $titlePngStream,
                    [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
                    [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
            $titleRightDecoder =
                New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
                    $titleRightPngStream,
                    [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
                    [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
            $promptDecoder = $null
            if ($promptPngStream) {
                $promptDecoder =
                    New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
                        $promptPngStream,
                        [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
                        [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
            }
            $legalDecoder = $null
            if ($legalPngStream) {
                $legalDecoder =
                    New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
                        $legalPngStream,
                        [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
                        [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
            }
            if (
                $titleDecoder.Frames[0].PixelWidth -ne 256 -or
                $titleDecoder.Frames[0].PixelHeight -ne 128 -or
                $titleRightDecoder.Frames[0].PixelWidth -ne 256 -or
                $titleRightDecoder.Frames[0].PixelHeight -ne 128
            ) {
                throw (
                    "Each retail frontend title half must be 256x128 after " +
                    "cropping; got " +
                    "$($titleDecoder.Frames[0].PixelWidth)x" +
                    "$($titleDecoder.Frames[0].PixelHeight) and " +
                    "$($titleRightDecoder.Frames[0].PixelWidth)x" +
                    "$($titleRightDecoder.Frames[0].PixelHeight)."
                )
            }
            $titleVisual =
                New-Object System.Windows.Media.DrawingVisual
            $titleContext = $titleVisual.RenderOpen()
            try {
                $hasRetailTextOverlay =
                    $visualBootUsesRetailPrompt -or
                    $visualBootUsesRetailLegal
                $titleX = if ($hasRetailTextOverlay) { 70 } else { 0 }
                $titleY = if ($hasRetailTextOverlay) { 30 } else { 0 }
                $titleContext.DrawImage(
                    $titleDecoder.Frames[0],
                    (New-Object System.Windows.Rect(
                        $titleX, $titleY, 256, 128)))
                $titleContext.DrawImage(
                    $titleRightDecoder.Frames[0],
                    (New-Object System.Windows.Rect(
                        ($titleX + 256), $titleY, 256, 128)))
                if ($promptDecoder) {
                    $titleContext.DrawImage(
                        $promptDecoder.Frames[0],
                        (New-Object System.Windows.Rect(0, 0, 640, 480)))
                }
                if ($legalDecoder) {
                    $titleContext.DrawImage(
                        $legalDecoder.Frames[0],
                        (New-Object System.Windows.Rect(0, 0, 640, 480)))
                }
            } finally {
                $titleContext.Close()
            }
            $titleCanvasWidth =
                if ($hasRetailTextOverlay) { 640 } else { 512 }
            $titleCanvasHeight =
                if ($hasRetailTextOverlay) { 480 } else { 128 }
            $combinedTitle =
                New-Object System.Windows.Media.Imaging.RenderTargetBitmap(
                    $titleCanvasWidth,
                    $titleCanvasHeight,
                    96,
                    96,
                    [System.Windows.Media.PixelFormats]::Pbgra32)
            $combinedTitle.Render($titleVisual)
            $titleEncoder =
                New-Object System.Windows.Media.Imaging.BmpBitmapEncoder
            $titleEncoder.Frames.Add(
                [System.Windows.Media.Imaging.BitmapFrame]::Create(
                    $combinedTitle))
            $titleBitmapStream =
                [System.IO.File]::Create($visualBootTitleBitmap)
            try {
                $titleEncoder.Save($titleBitmapStream)
            } finally {
                $titleBitmapStream.Dispose()
            }
        } finally {
            if ($legalPngStream) {
                $legalPngStream.Dispose()
            }
            if ($promptPngStream) {
                $promptPngStream.Dispose()
            }
            $titleRightPngStream.Dispose()
            $titlePngStream.Dispose()
        }
    }

    if ($visualBootUsesRetailAnimation) {
        $animationSheets = @(
            @{
                Png = $visualBootRetailForestSheet
                Bmp = $visualBootForestBitmap
                Width = 640
                Height = 1920
            },
            @{
                Png = $visualBootRetailSunbeamSheet
                Bmp = $visualBootSunbeamBitmap
                Width = 640
                Height = 1440
            }
        )
        if ($visualBootUsesRetailMenu) {
            $retailMenuEmbedPng = $visualBootRetailMenu
            if ($visualBootUsesRetailSubscreens) {
                $retailMenuEmbedPng = $visualBootRetailMenuComponents
            }
            $animationSheets += @(
                @{
                    Png = $retailMenuEmbedPng
                    Bmp = $visualBootMenuBitmap
                    Width = if ($visualBootUsesRetailSubscreens) { 1280 } else { 640 }
                    Height = 3360
                },
                @{
                    Png = $visualBootRetailCoastalSheet
                    Bmp = $visualBootCoastalBitmap
                    Width = 640
                    Height = 1920
                },
                @{
                    Png = $visualBootRetailCoastalSunbeamSheet
                    Bmp = $visualBootCoastalSunbeamBitmap
                    Width = 640
                    Height = 1440
                }
            )
            if ($visualBootUsesBuffJesusMenu) {
                $buffJesusMenuEmbedPng = $visualBootBuffJesusMenu
                if ($visualBootUsesRetailSubscreens) {
                    $buffJesusMenuEmbedPng =
                        $visualBootBuffJesusMenuComponents
                }
                $animationSheets += @{
                    Png = $buffJesusMenuEmbedPng
                    Bmp = $visualBootBuffJesusMenuBitmap
                    Width = if ($visualBootUsesRetailSubscreens) { 1280 } else { 640 }
                    Height = 3360
                }
            }
        }
        if ($spookyAnimationReady) {
            $animationSheets += @(
                @{
                    Png = $visualBootRetailSpookySheet
                    Bmp = $visualBootSpookyBitmap
                    Width = 640
                    Height = 1920
                },
                @{
                    Png = $visualBootRetailSpookySunbeamSheet
                    Bmp = $visualBootSpookySunbeamBitmap
                    Width = 640
                    Height = 1440
                }
            )
        }
        if ($visualBootUsesRetailSubscreens) {
            $animationSheets += @(
                @{
                    Png = $visualBootRetailOptionsComponents
                    Bmp = $visualBootOptionsBitmap
                    Width = 1664
                    Height = 3840
                },
                @{
                    Png = $visualBootRetailTitleSegment
                    Bmp = $visualBootTitleSegmentBitmap
                    Width = 8
                    Height = 64
                },
                @{
                    Png = $visualBootRetailButtonLeft
                    Bmp = $visualBootButtonLeftBitmap
                    Width = 64
                    Height = 64
                },
                @{
                    Png = $visualBootRetailButtonMiddle
                    Bmp = $visualBootButtonMiddleBitmap
                    Width = 8
                    Height = 64
                },
                @{
                    Png = $visualBootRetailButtonRight
                    Bmp = $visualBootButtonRightBitmap
                    Width = 64
                    Height = 64
                },
                @{
                    Png = $visualBootRetailHelpers
                    Bmp = $visualBootHelpersBitmap
                    Width = 640
                    Height = 2880
                },
                @{
                    Png = $visualBootRetailAbout
                    Bmp = $visualBootAboutBitmap
                    Width = 640
                    Height = 480
                }
            )
        }
        foreach ($sheet in $animationSheets) {
            $sheetStream = [System.IO.File]::OpenRead($sheet.Png)
            try {
                $sheetDecoder =
                    New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
                        $sheetStream,
                        [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
                        [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
                if (
                    $sheetDecoder.Frames[0].PixelWidth -ne $sheet.Width -or
                    $sheetDecoder.Frames[0].PixelHeight -ne $sheet.Height
                ) {
                    throw (
                        "Retail animation sheet dimensions differ for " +
                        "$($sheet.Png): " +
                        "$($sheetDecoder.Frames[0].PixelWidth)x" +
                        "$($sheetDecoder.Frames[0].PixelHeight)."
                    )
                }
                $sheetEncoder =
                    New-Object System.Windows.Media.Imaging.BmpBitmapEncoder
                $sheetEncoder.Frames.Add($sheetDecoder.Frames[0])
                $sheetBitmapStream =
                    [System.IO.File]::Create($sheet.Bmp)
                try {
                    $sheetEncoder.Save($sheetBitmapStream)
                } finally {
                    $sheetBitmapStream.Dispose()
                }
            } finally {
                $sheetStream.Dispose()
            }
        }
    }

    $resourceBitmapPath = $visualBootBitmap.Replace('\', '/')
    $resourceLines = @("101 BITMAP `"$resourceBitmapPath`"")
    if ($visualBootUsesRetailAsset) {
        $resourceTitleBitmapPath =
            $visualBootTitleBitmap.Replace('\', '/')
        $resourceLines +=
            "102 BITMAP `"$resourceTitleBitmapPath`""
    }
    if ($visualBootUsesRetailAnimation) {
        $resourceForestBitmapPath =
            $visualBootForestBitmap.Replace('\', '/')
        $resourceSunbeamBitmapPath =
            $visualBootSunbeamBitmap.Replace('\', '/')
        $resourceLines +=
            "103 BITMAP `"$resourceForestBitmapPath`""
        $resourceLines +=
            "104 BITMAP `"$resourceSunbeamBitmapPath`""
    }
    if ($visualBootUsesRetailPointer) {
        $resourceCursorPath = $visualBootCursor.Replace('\', '/')
        $resourceLines += "105 CURSOR `"$resourceCursorPath`""
    }
    if ($visualBootUsesRetailMenu) {
        $resourceMenuBitmapPath =
            $visualBootMenuBitmap.Replace('\', '/')
        $resourceCoastalBitmapPath =
            $visualBootCoastalBitmap.Replace('\', '/')
        $resourceCoastalSunbeamBitmapPath =
            $visualBootCoastalSunbeamBitmap.Replace('\', '/')
        $resourceLines +=
            "106 BITMAP `"$resourceMenuBitmapPath`""
        $resourceLines +=
            "107 BITMAP `"$resourceCoastalBitmapPath`""
        $resourceLines +=
            "108 BITMAP `"$resourceCoastalSunbeamBitmapPath`""
        if ($visualBootUsesBuffJesusMenu) {
            $resourceBuffJesusMenuBitmapPath =
                $visualBootBuffJesusMenuBitmap.Replace('\', '/')
            $resourceLines +=
                "111 BITMAP `"$resourceBuffJesusMenuBitmapPath`""
        }
    }
    if ($spookyAnimationReady) {
        $resourceSpookyBitmapPath =
            $visualBootSpookyBitmap.Replace('\', '/')
        $resourceSpookySunbeamBitmapPath =
            $visualBootSpookySunbeamBitmap.Replace('\', '/')
        $resourceLines +=
            "121 BITMAP `"$resourceSpookyBitmapPath`""
        $resourceLines +=
            "122 BITMAP `"$resourceSpookySunbeamBitmapPath`""
    }
    if ($visualBootUsesRetailSubscreens) {
        $resourceOptionsBitmapPath =
            $visualBootOptionsBitmap.Replace('\', '/')
        $resourceHelpersBitmapPath =
            $visualBootHelpersBitmap.Replace('\', '/')
        $resourceTitleSegmentBitmapPath =
            $visualBootTitleSegmentBitmap.Replace('\', '/')
        $resourceButtonLeftBitmapPath =
            $visualBootButtonLeftBitmap.Replace('\', '/')
        $resourceButtonMiddleBitmapPath =
            $visualBootButtonMiddleBitmap.Replace('\', '/')
        $resourceButtonRightBitmapPath =
            $visualBootButtonRightBitmap.Replace('\', '/')
        $resourceAboutBitmapPath =
            $visualBootAboutBitmap.Replace('\', '/')
        $resourceLines +=
            "109 BITMAP `"$resourceOptionsBitmapPath`""
        $resourceLines +=
            "110 BITMAP `"$resourceHelpersBitmapPath`""
        $resourceLines +=
            "120 BITMAP `"$resourceAboutBitmapPath`""
        $resourceLines +=
            "112 BITMAP `"$resourceTitleSegmentBitmapPath`""
        $resourceLines +=
            "113 BITMAP `"$resourceButtonLeftBitmapPath`""
        $resourceLines +=
            "114 BITMAP `"$resourceButtonMiddleBitmapPath`""
        $resourceLines +=
            "115 BITMAP `"$resourceButtonRightBitmapPath`""
    }
    if ($visualBootUsesRetailUiSounds) {
        $resourceSoundUpDownPath =
            $visualBootSoundUpDown.Replace('\', '/')
        $resourceSoundErrorPath =
            $visualBootSoundError.Replace('\', '/')
        $resourceSoundBackPath =
            $visualBootSoundBack.Replace('\', '/')
        $resourceSoundForwardPath =
            $visualBootSoundForward.Replace('\', '/')
        $resourceLines +=
            "116 WAVE `"$resourceSoundUpDownPath`""
        $resourceLines +=
            "117 WAVE `"$resourceSoundErrorPath`""
        $resourceLines +=
            "118 WAVE `"$resourceSoundBackPath`""
        $resourceLines +=
            "119 WAVE `"$resourceSoundForwardPath`""
    }
    Set-Content -LiteralPath $visualBootResourceSource `
        -Value $resourceLines `
        -Encoding Ascii
    & (Join-Path $vcRoot 'bin\rc.exe') `
        "/fo$visualBootResource" $visualBootResourceSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootResource)
    ) {
        throw 'Failed to compile the visual boot artwork resource.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase1BehaviorObject" $gfmainPhase1BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 1 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase2BehaviorObject" $gfmainPhase2BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 2 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase3BehaviorObject" $gfmainPhase3BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase3BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 3 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase4BehaviorObject" $gfmainPhase4BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase4BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 4 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase5BehaviorObject" $gfmainPhase5BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase5BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 5 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase6BehaviorObject" $gfmainPhase6BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase6BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 6 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase7BehaviorObject" $gfmainPhase7BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase7BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 7 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase8BehaviorObject" $gfmainPhase8BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase8BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 8 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase9BehaviorObject" $gfmainPhase9BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase9BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 9 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase10BehaviorObject" $gfmainPhase10BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase10BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 10 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainCompleteBehaviorObject" $gfmainCompleteBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainCompleteBehaviorObject)
    ) {
        throw 'Failed to compile the complete GFMain parent fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfInitialiseProgressPhaseBehaviorObject" `
        $gfInitialiseProgressPhaseBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfInitialiseProgressPhaseBehaviorObject)
    ) {
        throw 'Failed to compile the GFInitialise progress behavior fixture.'
    }

    $phase1RuntimeObjects = @(
        $gfmainPhase1Object,
        $stage2BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $profileStartObject,
        $systemManagerInitObject
    )

    $phase2RuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $stage3BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $profileStartObject,
        $profileEndObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    $phase3RuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $gfmainPhase3Object,
        $stage3Phase3BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $wideStringCopyObject,
        $wideStringFromCharObject,
        $wideStringAddRightObject,
        $wideStringAddLeftObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $charToWideStringObject,
        $defaultLanguageNameObject,
        $profileStartObject,
        $profileEndObject,
        $primaryLeftAlignmentObject,
        $secondaryLeftAlignmentObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    $phase4RuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $gfmainPhase3Object,
        $gfmainPhase4Object,
        $stage3Phase4BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $wideStringCopyObject,
        $wideStringFromCharObject,
        $wideStringAddRightObject,
        $wideStringAddLeftObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $charStringCopyAssignmentObject,
        $charToWideStringObject,
        $defaultLanguageNameObject,
        $profileStartObject,
        $profileEndObject,
        $primaryLeftAlignmentObject,
        $secondaryLeftAlignmentObject,
        $retailBankNavigatorObject,
        $retailBankAliasObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    $phase5RuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $gfmainPhase3Object,
        $gfmainPhase4Object,
        $gfmainPhase5Object,
        $stage3Phase5BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $wideStringCopyObject,
        $wideStringFromCharObject,
        $wideStringAddRightObject,
        $wideStringAddLeftObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $charStringCopyAssignmentObject,
        $charToWideStringObject,
        $defaultLanguageNameObject,
        $miscDirectoryAObject,
        $miscDirectoryBObject,
        $graphicsDirectoryObject,
        $languageDirectoryAObject,
        $shadersDirectoryObject,
        $languageDirectoryBObject,
        $profileStartObject,
        $profileEndObject,
        $primaryLeftAlignmentObject,
        $secondaryLeftAlignmentObject,
        $retailBankNavigatorObject,
        $retailBankAliasObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    $phase6RuntimeObjects = @(
        $phase5RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase5BoundaryObject }
    )
    $phase6RuntimeObjects += @(
        $gfmainPhase6Object,
        $stage3Phase6BoundaryObject
    )

    $phase7RuntimeObjects = @(
        $phase6RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase6BoundaryObject }
    )
    $phase7RuntimeObjects += @(
        $gfmainPhase7Object,
        $stage3Phase7BoundaryObject,
        $cacheDirectoryObject,
        $streamingFontBankNameObject
    )

    $phase8RuntimeObjects = @(
        $phase7RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase7BoundaryObject }
    )
    $phase8RuntimeObjects += @(
        $gfmainPhase8Object,
        $stage3Phase8BoundaryObject
    )

    $phase9RuntimeObjects = @(
        $phase8RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase8BoundaryObject }
    )
    $phase9RuntimeObjects += @(
        $gfmainPhase9Object,
        $stage3Phase9BoundaryObject,
        $getSystemManagerObject,
        $movieLeftAlignObject,
        $imePrimitiveObject
    )

    $phase10RuntimeObjects = @(
        $phase9RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase9BoundaryObject }
    )
    $phase10RuntimeObjects += @(
        $gfmainPhase10Object,
        $gfmainCompleteObject,
        $stage3Phase10BoundaryObject
    )

    $stage3CompleteRuntimeObjects = @(
        $phase10RuntimeObjects |
            Where-Object { $_ -ne $stage3Phase10BoundaryObject }
    )
    $stage3CompleteRuntimeObjects += @(
        $stage3CompleteBoundaryObject,
        $defTablePathObject,
        $defStringTableLoadObject,
        $myDocumentsWritePermissionsObject,
        $phase6RuntimeBoundaryObject,
        $gfInitialiseObject,
        $gfUninitialiseObject,
        $imeInitialiseObject,
        $systemManagerInitialiseObject,
        $systemManagerRuntimeBoundaryObject,
        $generateMetFilesObject,
        $saveMetadataRuntimeBoundaryObject,
        $doEulaObject,
        $configDetectionObject,
        $freeConfigDetectionObject,
        $phase8RuntimeBoundaryObject,
        $getWindowTitleObject,
        $phase7WindowTitleRuntimeBoundaryObject,
        $gfInitialiseEngineBoundaryObject,
        $progressSetupObject,
        $progressDisplayConstructorObject,
        $progressDisplayTextModeObject,
        $progressDisplayIsActiveObject,
        $setProgressDisplayObject,
        $getProgressDisplayObject
    )

    $visualRuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $gfmainPhase3Object,
        $gfmainPhase4Object,
        $gfmainPhase5Object,
        $gfmainPhase6Object,
        $gfmainPhase7Object,
        $gfmainPhase8Object,
        $gfmainPhase9Object,
        $gfmainPhase10Object,
        $gfmainCompleteObject,
        $defTablePathObject,
        $defStringTableLoadObject,
        $myDocumentsWritePermissionsObject,
        $phase6RuntimeBoundaryObject,
        $gfInitialiseObject,
        $gfUninitialiseObject,
        $imeInitialiseObject,
        $systemManagerInitialiseObject,
        $systemManagerRuntimeBoundaryObject,
        $generateMetFilesObject,
        $saveMetadataRuntimeBoundaryObject,
        $doEulaObject,
        $configDetectionObject,
        $freeConfigDetectionObject,
        $phase8RuntimeBoundaryObject,
        $getWindowTitleObject,
        $phase7WindowTitleRuntimeBoundaryObject,
        $gfInitialiseEngineBoundaryObject,
        $progressSetupObject,
        $progressDisplayConstructorObject,
        $progressDisplayTextModeObject,
        $progressDisplayIsActiveObject,
        $setProgressDisplayObject,
        $getProgressDisplayObject,
        $visualBoundaryObject,
        $visualBootObject,
        $visualBootD3D9Object,
        $retailVideoBridgeObject,
        $videoFrameConversionObject,
        $videoFramePublicationObject,
        $frontendStartupSequenceObject,
        $render2DBatchPlanObject,
        $render2DDrawListAdapterObject,
        $attachTextureToStageObject,
        $realiseRenderStateObject,
        $soldStateBlockObject,
        $updatePixelShaderObject,
        $resetWorldTransformObject,
        $displaySetViewportObject,
        $displaySetIntegerViewportObject,
        $postViewportShaderObject,
        $viewportE2Object,
        $setRenderWindowObject,
        $clearRender2DVertexQueueObject,
        $restoreRenderStateCaptureObject,
        $textureCalcByteLengthObject,
        $pixelFormatGetColourDepthObject,
        $pixelFormatInitialiseObject,
        $pixelFormatTableObject,
        $textureInitialisePreallocatedObject,
        $textureAssignmentObject,
        $textureUninitialiseObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $wideStringCopyObject,
        $wideStringFromCharObject,
        $wideStringAddRightObject,
        $wideStringAddLeftObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $charStringCopyAssignmentObject,
        $charToWideStringObject,
        $defaultLanguageNameObject,
        $miscDirectoryAObject,
        $miscDirectoryBObject,
        $graphicsDirectoryObject,
        $languageDirectoryAObject,
        $shadersDirectoryObject,
        $cacheDirectoryObject,
        $languageDirectoryBObject,
        $streamingFontBankNameObject,
        $profileStartObject,
        $profileEndObject,
        $primaryLeftAlignmentObject,
        $secondaryLeftAlignmentObject,
        $movieLeftAlignObject,
        $imePrimitiveObject,
        $retailBankNavigatorObject,
        $retailBankAliasObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase1BehaviorExecutable" `
        @phase1RuntimeObjects $gfmainPhase1BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 1 behavior fixture.'
    }

    $gfmainPhase1Output = & $gfmainPhase1BehaviorExecutable 2>&1
    $gfmainPhase1ExitCode = $LASTEXITCODE
    $gfmainPhase1Output | Write-Output
    if (
        $gfmainPhase1ExitCode -ne 0 -or
        (($gfmainPhase1Output -join "`n") -notmatch [regex]::Escape($gfmainPhase1PassPattern))
    ) {
        throw "GFMain Phase 1 fixture failed with exit code $gfmainPhase1ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase2BehaviorExecutable" `
        @phase2RuntimeObjects $gfmainPhase2BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 2 behavior fixture.'
    }

    $gfmainPhase2Output = & $gfmainPhase2BehaviorExecutable 2>&1
    $gfmainPhase2ExitCode = $LASTEXITCODE
    $gfmainPhase2Output | Write-Output
    if (
        $gfmainPhase2ExitCode -ne 0 -or
        (($gfmainPhase2Output -join "`n") -notmatch [regex]::Escape($gfmainPhase2PassPattern))
    ) {
        throw "GFMain Phase 2 fixture failed with exit code $gfmainPhase2ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase3BehaviorExecutable" `
        @phase3RuntimeObjects $gfmainPhase3BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase3BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 3 behavior fixture.'
    }

    $gfmainPhase3Output = & $gfmainPhase3BehaviorExecutable 2>&1
    $gfmainPhase3ExitCode = $LASTEXITCODE
    $gfmainPhase3Output | Write-Output
    if (
        $gfmainPhase3ExitCode -ne 0 -or
        (($gfmainPhase3Output -join "`n") -notmatch [regex]::Escape($gfmainPhase3PassPattern))
    ) {
        throw "GFMain Phase 3 fixture failed with exit code $gfmainPhase3ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase4BehaviorExecutable" `
        @phase4RuntimeObjects $gfmainPhase4BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase4BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 4 behavior fixture.'
    }

    $gfmainPhase4Output = & $gfmainPhase4BehaviorExecutable 2>&1
    $gfmainPhase4ExitCode = $LASTEXITCODE
    $gfmainPhase4Output | Write-Output
    if (
        $gfmainPhase4ExitCode -ne 0 -or
        (($gfmainPhase4Output -join "`n") -notmatch [regex]::Escape($gfmainPhase4PassPattern))
    ) {
        throw "GFMain Phase 4 fixture failed with exit code $gfmainPhase4ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase5BehaviorExecutable" `
        @phase5RuntimeObjects $gfmainPhase5BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase5BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 5 behavior fixture.'
    }

    $gfmainPhase5Output = & $gfmainPhase5BehaviorExecutable 2>&1
    $gfmainPhase5ExitCode = $LASTEXITCODE
    $gfmainPhase5Output | Write-Output
    if (
        $gfmainPhase5ExitCode -ne 0 -or
        (($gfmainPhase5Output -join "`n") -notmatch [regex]::Escape($gfmainPhase5PassPattern))
    ) {
        throw "GFMain Phase 5 fixture failed with exit code $gfmainPhase5ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase6BehaviorExecutable" `
        @phase6RuntimeObjects $gfmainPhase6BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase6BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 6 behavior fixture.'
    }

    $gfmainPhase6Output = & $gfmainPhase6BehaviorExecutable 2>&1
    $gfmainPhase6ExitCode = $LASTEXITCODE
    $gfmainPhase6Output | Write-Output
    if (
        $gfmainPhase6ExitCode -ne 0 -or
        (($gfmainPhase6Output -join "`n") -notmatch [regex]::Escape($gfmainPhase6PassPattern))
    ) {
        throw "GFMain Phase 6 fixture failed with exit code $gfmainPhase6ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase7BehaviorExecutable" `
        @phase7RuntimeObjects $gfmainPhase7BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase7BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 7 behavior fixture.'
    }

    $gfmainPhase7Output = & $gfmainPhase7BehaviorExecutable 2>&1
    $gfmainPhase7ExitCode = $LASTEXITCODE
    $gfmainPhase7Output | Write-Output
    if (
        $gfmainPhase7ExitCode -ne 0 -or
        (($gfmainPhase7Output -join "`n") -notmatch [regex]::Escape($gfmainPhase7PassPattern))
    ) {
        throw "GFMain Phase 7 fixture failed with exit code $gfmainPhase7ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase8BehaviorExecutable" `
        @phase8RuntimeObjects $gfmainPhase8BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase8BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 8 behavior fixture.'
    }

    $gfmainPhase8Output = & $gfmainPhase8BehaviorExecutable 2>&1
    $gfmainPhase8ExitCode = $LASTEXITCODE
    $gfmainPhase8Output | Write-Output
    if (
        $gfmainPhase8ExitCode -ne 0 -or
        (($gfmainPhase8Output -join "`n") -notmatch [regex]::Escape($gfmainPhase8PassPattern))
    ) {
        throw "GFMain Phase 8 fixture failed with exit code $gfmainPhase8ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase9BehaviorExecutable" `
        @phase9RuntimeObjects $gfmainPhase9BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase9BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 9 behavior fixture.'
    }

    $gfmainPhase9Output = & $gfmainPhase9BehaviorExecutable 2>&1
    $gfmainPhase9ExitCode = $LASTEXITCODE
    $gfmainPhase9Output | Write-Output
    if (
        $gfmainPhase9ExitCode -ne 0 -or
        (($gfmainPhase9Output -join "`n") -notmatch [regex]::Escape($gfmainPhase9PassPattern))
    ) {
        throw "GFMain Phase 9 fixture failed with exit code $gfmainPhase9ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainCompleteBehaviorExecutable" `
        $gfmainCompleteObject $gfmainCompleteBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainCompleteBehaviorExecutable)
    ) {
        throw 'Failed to link the complete GFMain parent fixture.'
    }

    $gfmainCompleteOutput = & $gfmainCompleteBehaviorExecutable 2>&1
    $gfmainCompleteExitCode = $LASTEXITCODE
    $gfmainCompleteOutput | Write-Output
    if (
        $gfmainCompleteExitCode -ne 0 -or
        (($gfmainCompleteOutput -join "`n") -notmatch [regex]::Escape($gfmainCompletePassPattern))
    ) {
        throw "Complete GFMain fixture failed with exit code $gfmainCompleteExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase10BehaviorExecutable" `
        @phase10RuntimeObjects $gfmainPhase10BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase10BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 10 behavior fixture.'
    }

    $gfmainPhase10Output = & $gfmainPhase10BehaviorExecutable 2>&1
    $gfmainPhase10ExitCode = $LASTEXITCODE
    $gfmainPhase10Output | Write-Output
    if (
        $gfmainPhase10ExitCode -ne 0 -or
        (($gfmainPhase10Output -join "`n") -notmatch [regex]::Escape($gfmainPhase10PassPattern))
    ) {
        throw "GFMain Phase 10 fixture failed with exit code $gfmainPhase10ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfInitialiseProgressPhaseBehaviorExecutable" `
        $gfInitialiseProgressPhaseObject `
        $gfInitialiseEngineBoundaryObject `
        $progressSetupObject `
        $progressDisplayConstructorObject `
        $progressDisplayIsActiveObject `
        $setProgressDisplayObject `
        $getProgressDisplayObject `
        $progressDisplayStringBoundaryObject `
        $wideStringConstructorObject `
        $wideStringDestructorObject `
        $charStringDefaultObject `
        $charStringDestructorObject `
        $gfInitialiseProgressPhaseBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfInitialiseProgressPhaseBehaviorExecutable)
    ) {
        throw 'Failed to link the GFInitialise progress behavior fixture.'
    }

    $gfInitialiseProgressPhaseOutput =
        & $gfInitialiseProgressPhaseBehaviorExecutable 2>&1
    $gfInitialiseProgressPhaseExitCode = $LASTEXITCODE
    $gfInitialiseProgressPhaseOutput | Write-Output
    if (
        $gfInitialiseProgressPhaseExitCode -ne 0 -or
        (($gfInitialiseProgressPhaseOutput -join "`n") -notmatch `
            [regex]::Escape($gfInitialiseProgressPhasePassPattern))
    ) {
        throw (
            'GFInitialise progress fixture failed with exit code ' +
            "$gfInitialiseProgressPhaseExitCode."
        )
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage2Executable" $winMainObject @phase1RuntimeObjects
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage2Executable)
    ) {
        throw 'Failed to link the Stage 2 startup executable.'
    }

    & $stage2Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Stage 2 startup failed with exit code $LASTEXITCODE."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage3Executable" `
        $winMainObject @stage3CompleteRuntimeObjects
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Executable)
    ) {
        throw 'Failed to link the Stage 3 startup executable.'
    }

    & $stage3Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Stage 3 startup failed with exit code $LASTEXITCODE."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$visualCheckpointExecutable" `
        $winMainObject @visualRuntimeObjects $visualBootResource `
        user32.lib gdi32.lib d3d9.lib advapi32.lib ole32.lib winmm.lib
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualCheckpointExecutable)
    ) {
        throw 'Failed to link the authored visual startup executable.'
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$visualBootBehaviorExecutable" `
        $visualBootObject $visualBootD3D9Object $retailVideoBridgeObject `
        $videoFrameConversionObject $videoFramePublicationObject `
        $frontendStartupSequenceObject `
        $render2DBatchPlanObject $render2DDrawListAdapterObject `
        $attachTextureToStageObject $realiseRenderStateObject `
        $soldStateBlockObject `
        $updatePixelShaderObject `
        $resetWorldTransformObject `
        $displaySetViewportObject $displaySetIntegerViewportObject `
        $postViewportShaderObject $viewportE2Object `
        $setRenderWindowObject `
        $clearRender2DVertexQueueObject `
        $restoreRenderStateCaptureObject `
        $textureCalcByteLengthObject `
        $pixelFormatGetColourDepthObject $pixelFormatInitialiseObject `
        $pixelFormatTableObject $textureInitialisePreallocatedObject `
        $textureAssignmentObject $textureUninitialiseObject `
        $visualBootBehaviorObject $visualBootResource `
        user32.lib gdi32.lib d3d9.lib advapi32.lib ole32.lib winmm.lib
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootBehaviorExecutable)
    ) {
        throw 'Failed to link the visual boot behavior fixture.'
    }

    $visualBootOutput = & $visualBootBehaviorExecutable 2>&1
    $visualBootExitCode = $LASTEXITCODE
    $visualBootOutput | Write-Output
    if (
        $visualBootExitCode -ne 0 -or
        (($visualBootOutput -join "`n") -notmatch [regex]::Escape($visualBootPassPattern))
    ) {
        throw "Visual boot fixture failed with exit code $visualBootExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$frontendStartupSequenceBehaviorExecutable" `
        $frontendStartupSequenceObject `
        $frontendStartupSequenceBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $frontendStartupSequenceBehaviorExecutable)
    ) {
        throw 'Failed to link the frontend startup sequence fixture.'
    }

    $frontendStartupSequenceOutput =
        & $frontendStartupSequenceBehaviorExecutable 2>&1
    $frontendStartupSequenceExitCode = $LASTEXITCODE
    $frontendStartupSequenceOutput | Write-Output
    if (
        $frontendStartupSequenceExitCode -ne 0 -or
        (($frontendStartupSequenceOutput -join "`n") -notmatch `
            [regex]::Escape($frontendStartupSequencePassPattern))
    ) {
        throw (
            'Frontend startup sequence fixture failed with exit code ' +
            "$frontendStartupSequenceExitCode."
        )
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$videoFramePublicationBehaviorExecutable" `
        $videoFrameConversionObject $videoFramePublicationObject `
        $videoFramePublicationBehaviorObject kernel32.lib
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $videoFramePublicationBehaviorExecutable)
    ) {
        throw 'Failed to link the decoded-frame publication fixture.'
    }

    $videoFramePublicationOutput =
        & $videoFramePublicationBehaviorExecutable 2>&1
    $videoFramePublicationExitCode = $LASTEXITCODE
    $videoFramePublicationOutput | Write-Output
    if (
        $videoFramePublicationExitCode -ne 0 -or
        (($videoFramePublicationOutput -join "`n") -notmatch `
            [regex]::Escape($videoFramePublicationPassPattern))
    ) {
        throw (
            'Decoded-frame publication fixture failed with exit code ' +
            "$videoFramePublicationExitCode."
        )
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$render2DBatchPlanBehaviorExecutable" `
        $render2DBatchPlanObject $render2DBatchPlanBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $render2DBatchPlanBehaviorExecutable)
    ) {
        throw 'Failed to link the Render2D batch-plan fixture.'
    }

    $render2DBatchPlanOutput =
        & $render2DBatchPlanBehaviorExecutable 2>&1
    $render2DBatchPlanExitCode = $LASTEXITCODE
    $render2DBatchPlanOutput | Write-Output
    if (
        $render2DBatchPlanExitCode -ne 0 -or
        (($render2DBatchPlanOutput -join "`n") -notmatch `
            [regex]::Escape($render2DBatchPlanPassPattern))
    ) {
        throw (
            'Render2D batch-plan fixture failed with exit code ' +
            "$render2DBatchPlanExitCode."
        )
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$render2DDrawListAdapterBehaviorExecutable" `
        $render2DDrawListAdapterObject `
        $render2DDrawListAdapterBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath `
            $render2DDrawListAdapterBehaviorExecutable)
    ) {
        throw 'Failed to link the Render2D draw-list adapter fixture.'
    }

    $render2DDrawListAdapterOutput =
        & $render2DDrawListAdapterBehaviorExecutable 2>&1
    $render2DDrawListAdapterExitCode = $LASTEXITCODE
    $render2DDrawListAdapterOutput | Write-Output
    if (
        $render2DDrawListAdapterExitCode -ne 0 -or
        (($render2DDrawListAdapterOutput -join "`n") -notmatch `
            [regex]::Escape($render2DDrawListAdapterPassPattern))
    ) {
        throw (
            'Render2D draw-list adapter fixture failed with exit code ' +
            "$render2DDrawListAdapterExitCode."
        )
    }

    Write-Output "BOOTSTRAP_BUILD PASS configuration=$Configuration executable=$executable"
    Write-Output "STAGE1_STARTUP PASS executable=$stage1Executable boundary=GFMain"
    Write-Output "STAGE2_STARTUP PASS executable=$stage2Executable boundary=GFMainPhase2"
    Write-Output "STAGE3_STARTUP PASS executable=$stage3Executable boundary=GFMainComplete deftable=ExactRelocationMatch permissions=ExactRelocationMatch windowtitle=ExactRelocationMatch phase8=ExactRelocationMatch systemmanager=ExactRelocationMatch cime=ExactRelocationMatch savemetadata=ExactRelocationMatch gfinitialise=ExactRelocationMatch gfuninitialise=ExactRelocationMatch"
    Write-Output "GFINITIALISE_COORDINATOR PASS address=004022b0 parity=RELOCATION_MATCH"
    Write-Output "GFINITIALISE_PROGRESS_INTEGRATION PASS boundary=GFInitialiseTail"
    $visualAssetGrade = if ($visualBootUsesRetailAsset) {
        'RetailFrontendTitleFrame'
    } else {
        'AuthoredFallback'
    }
    Write-Output "VISUAL_BOOT_CHECKPOINT PASS executable=$visualCheckpointExecutable boundary=VerifiedGFInitialiseThenAuthoredVisualCheckpoint asset=$visualAssetGrade presentation=D3D9Render2DRecoveredTrackedSoldPixelShaderAndWorldTransformState"
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}
