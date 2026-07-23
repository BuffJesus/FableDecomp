param(
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'
$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$vcRoot = 'D:\Tools\vc71'
$sourceRoot = Join-Path $rebuildRoot 'src\compiled'
$outDir = Join-Path $rebuildRoot "build\candidates-$Configuration"
$reportDir = Join-Path $rebuildRoot 'compile-gate'
New-Item -ItemType Directory -Path $outDir -Force | Out-Null
New-Item -ItemType Directory -Path $reportDir -Force | Out-Null

$catalog = @(
    [pscustomobject]@{
        Address = '00401f30'; Module = '_global'; Source = 'Global_GFGetBuildNumber2_00401f30.cpp'
        TestSource = 'Global_GFGetBuildNumber2_00401f30_test.cpp'; PassPattern = 'GF_GET_BUILD_NUMBER_2_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00662020'; Module = '_global'; Source = 'Global_GetterFieldE0FC_00662020.cpp'
        TestSource = 'Global_GetterFieldE0FC_00662020_test.cpp'; PassPattern = 'GETTER_FIELD_E0_FC_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a7a650'; Module = 'CNavQuadTree'; Source = 'CNavQuadTree_LinkToNeighbouringMaps_00a7a650.cpp'
        TestSource = 'CNavQuadTree_LinkToNeighbouringMaps_00a7a650_test.cpp'; PassPattern = 'NAV_QUAD_TREE_LINK_NEIGHBOURING_MAPS_TEST PASS'
    },
      [pscustomobject]@{
          Address = '00a7a5e0'; Module = 'CNavQuadTree'; Source = 'CNavQuadTree_InitialiseLines_00a7a5e0.cpp'
          TestSource = 'CNavQuadTree_InitialiseLines_00a7a5e0_test.cpp'; PassPattern = 'NAV_QUAD_TREE_INITIALISE_LINES_TEST PASS'
      },
      [pscustomobject]@{
          Address = '0088edb0'; Module = 'CGameScriptInterface'; Source = 'CGameScriptInterface_CameraCancelScreenEffect_0088edb0.cpp'
          TestSource = 'CGameScriptInterface_CameraCancelScreenEffect_0088edb0_test.cpp'; PassPattern = 'CAMERA_CANCEL_SCREEN_EFFECT_TEST PASS'
      },
      [pscustomobject]@{
          Address = '0088eda0'; Module = 'CGameScriptInterface'; Source = 'CGameScriptInterface_CameraUseScreenEffect_0088eda0.cpp'
          TestSource = 'CGameScriptInterface_CameraUseScreenEffect_0088eda0_test.cpp'; PassPattern = 'CAMERA_USE_SCREEN_EFFECT_TEST PASS'
      },
      [pscustomobject]@{
        Address = '00a77480'; Module = 'CNavQuadTree'; Source = 'CNavQuadTree_JoinNavigationLayers_00a77480.cpp'
        TestSource = 'CNavQuadTree_JoinNavigationLayers_00a77480_test.cpp'; PassPattern = 'NAV_QUAD_TREE_JOIN_NAVIGATION_LAYERS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a77500'; Module = 'CNavQuadTree'; Source = 'CNavQuadTree_VerifyTransitionPoints_00a77500.cpp'
        TestSource = 'CNavQuadTree_VerifyTransitionPoints_00a77500_test.cpp'; PassPattern = 'NAV_QUAD_TREE_VERIFY_TRANSITION_POINTS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0040bb90'; Module = '_global'; Source = 'Global_CUserProfileManagerSaveAndCleanup_0040bb90.cpp'
        TestSource = 'Global_CUserProfileManagerSaveAndCleanup_0040bb90_test.cpp'; PassPattern = 'USER_PROFILE_SAVE_CLEANUP_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00404a90'; Module = '_global'; Source = 'Global_E2_00404a90.cpp'
        TestSource = 'Global_E2_00404a90_test.cpp'; PassPattern = 'GLOBAL_E2_00404A90_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00404310'; Module = '_global'; Source = 'Global_CCharStringComputeCRC32_00404310.cpp'
        TestSource = 'Global_CCharStringComputeCRC32_00404310_test.cpp'; PassPattern = 'CCHARSTRING_COMPUTE_CRC32_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0040138c'; Module = '_global'; Source = 'Global_SEHProlog_0040138c.cpp'
        TestSource = 'Global_SEHProlog_0040138c_test.cpp'; PassPattern = 'GLOBAL_SEH_PROLOG_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00405ba0'; Module = '_global'; Source = 'Global_StdMoveBackward_00405ba0.cpp'
        TestSource = 'Global_StdMoveBackward_00405ba0_test.cpp'; PassPattern = 'STD_MOVE_BACKWARD_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00403c60'; Module = '_global'; Source = 'Global_MemCmpUnsigned16_00403c60.cpp'
        TestSource = 'Global_MemCmpUnsigned16_00403c60_test.cpp'; PassPattern = 'MEMCMP_UNSIGNED16_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00404690'; Module = '_global'; Source = 'Global_StdListNodeInit_00404690.cpp'
        TestSource = 'Global_StdListNodeInit_00404690_test.cpp'; PassPattern = 'STD_LIST_NODE_INIT_TEST PASS'
    },
    [pscustomobject]@{
        Address = '004046f0'; Module = '_global'; Source = 'Global_ListNodeCreateCircular_004046f0.cpp'
        TestSource = 'Global_ListNodeCreateCircular_004046f0_test.cpp'; PassPattern = 'LIST_NODE_CREATE_CIRCULAR_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00404280'; Module = '_global'; Source = 'Global_WideStringEqualsLen_00404280.cpp'
        TestSource = 'Global_WideStringEqualsLen_00404280_test.cpp'; PassPattern = 'WIDESTRING_EQUALS_LEN_TEST PASS'
    },
    [pscustomobject]@{
        Address = '004012ce'
        Module = '_global'
        Source = 'Global_RTCInitialize_004012ce.cpp'
        TestSource = 'Global_RTCInitialize_004012ce_test.cpp'
        PassPattern = 'RTC_INITIALIZE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0040e130'
        Module = '_global'
        Source = 'Global_Vector_FreeElements_0040e130.cpp'
        TestSource = 'Global_Vector_FreeElements_0040e130_test.cpp'
        PassPattern = 'VECTOR_FREE_ELEMENTS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00401356'
        Module = '_global'
        Source = 'Global_InitTerm_00401356.cpp'
        TestSource = 'Global_InitTerm_00401356_test.cpp'
        PassPattern = 'GLOBAL_INITTERM_TEST PASS'
    },
    [pscustomobject]@{
        Address = '004013de'
        Module = '_global'
        Source = 'Global_DllOnExit_004013de.cpp'
        TestSource = 'Global_DllOnExit_004013de_test.cpp'
        PassPattern = 'GLOBAL_DLLONEXIT_TEST PASS'
    },
    [pscustomobject]@{
        Address = '004013c7'
        Module = '_global'
        Source = 'Global_SEHEpilog_004013c7.cpp'
        TestSource = 'Global_SEHEpilog_004013c7_test.cpp'
        PassPattern = 'GLOBAL_SEH_EPILOG_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00401296'
        Module = '_global'
        Source = 'Global_OnExit_00401296.cpp'
        TestSource = 'Global_OnExit_00401296_test.cpp'
        PassPattern = 'GLOBAL_ONEXIT_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0040135c'
        Module = '_global'
        Source = 'Global_SetDefaultPrecision_0040135c.cpp'
        TestSource = 'Global_SetDefaultPrecision_0040135c_test.cpp'
        PassPattern = 'SET_DEFAULT_PRECISION_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0040e170'
        Module = '_global'
        Source = 'Global_Array_LinearSearchInt_0040e170.cpp'
        TestSource = 'Global_Array_LinearSearchInt_0040e170_test.cpp'
        PassPattern = 'ARRAY_LINEAR_SEARCH_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00406770'
        Module = '_global'
        Source = 'Global_E2_00406770.cpp'
        TestSource = 'Global_E2_00406770_test.cpp'
        PassPattern = 'GLOBAL_E2_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0081ee60'
        Module = 'CMap'
        Source = 'CMap_SetEngineThemeAt_0081ee60.cpp'
        TestSource = 'CMap_SetEngineThemeAt_0081ee60_test.cpp'
        PassPattern = 'MAP_SET_ENGINE_THEME_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0081efc0'
        Module = 'CMap'
        Source = 'CMap_SetEngineBlendAt_0081efc0.cpp'
        TestSource = 'CMap_SetEngineBlendAt_0081efc0_test.cpp'
        PassPattern = 'MAP_SET_ENGINE_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0081f090'
        Module = 'CMap'
        Source = 'CMap_GetEngineThemeAt_0081f090.cpp'
        TestSource = 'CMap_GetEngineThemeAt_0081f090_test.cpp'
        PassPattern = 'MAP_GET_ENGINE_THEME_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0081f170'
        Module = 'CMap'
        Source = 'CMap_GetEngineBlendAt_0081f170.cpp'
        TestSource = 'CMap_GetEngineBlendAt_0081f170_test.cpp'
        PassPattern = 'MAP_GET_ENGINE_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0082e330'
        Module = 'CScriptedMapBrush'
        Source = 'CScriptedMapBrush_GetTheme_0082e330.cpp'
        TestSource = 'CScriptedMapBrush_GetTheme_0082e330_test.cpp'
        PassPattern = 'SCRIPTED_BRUSH_GET_THEME_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0082e3d0'
        Module = 'CScriptedMapBrush'
        Source = 'CScriptedMapBrush_GetThemeBlend_0082e3d0.cpp'
        TestSource = 'CScriptedMapBrush_GetThemeBlend_0082e3d0_test.cpp'
        PassPattern = 'SCRIPTED_BRUSH_GET_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0082e420'
        Module = 'CScriptedMapBrush'
        Source = 'CScriptedMapBrush_SetThemeBlend_0082e420.cpp'
        TestSource = 'CScriptedMapBrush_SetThemeBlend_0082e420_test.cpp'
        PassPattern = 'SCRIPTED_BRUSH_SET_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0063a970'
        Module = 'CWorldMap'
        Source = 'CWorldMap_SetThemeAtBlock_0063a970.cpp'
        TestSource = 'CWorldMap_SetThemeAtBlock_0063a970_test.cpp'
        PassPattern = 'WORLD_MAP_SET_THEME_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0063a9e0'
        Module = 'CWorldMap'
        Source = 'CWorldMap_SetThemeBlendAtBlock_0063a9e0.cpp'
        TestSource = 'CWorldMap_SetThemeBlendAtBlock_0063a9e0_test.cpp'
        PassPattern = 'WORLD_MAP_SET_THEME_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0063aa50'
        Module = 'CWorldMap'
        Source = 'CWorldMap_GetThemeAtBlock_0063aa50.cpp'
        TestSource = 'CWorldMap_GetThemeAtBlock_0063aa50_test.cpp'
        PassPattern = 'WORLD_MAP_GET_THEME_TEST PASS'
    },
    [pscustomobject]@{
        Address = '0063aab0'
        Module = 'CWorldMap'
        Source = 'CWorldMap_GetThemeBlendAtBlock_0063aab0.cpp'
        TestSource = 'CWorldMap_GetThemeBlendAtBlock_0063aab0_test.cpp'
        PassPattern = 'WORLD_MAP_GET_THEME_BLEND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf3a90'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_LoadHeader_00bf3a90.cpp'
        TestSource = 'CEngineLandscapePatch_LoadHeader_00bf3a90_test.cpp'
        PassPattern = 'LOADHEADER_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf4130'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_LoadTextures_00bf4130.cpp'
        TestSource = 'CEngineLandscapePatch_LoadTextures_00bf4130_test.cpp'
        PassPattern = 'LOADTEXTURES_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00beaaa0'
        Module = 'CEngineScreenEffectOutlineGlow'
        Source = 'CEngineScreenEffectOutlineGlow_FinishWithDynamicTextures_00beaaa0.cpp'
        TestSource = 'CEngineScreenEffectOutlineGlow_FinishWithDynamicTextures_00beaaa0_test.cpp'
        PassPattern = 'OUTLINEGLOW_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00beb4d0'
        Module = 'CEngineScreenEffectOutlineGlow'
        Source = 'CEngineScreenEffectOutlineGlow_ScalarDeletingDestructor_00beb4d0.cpp'
        TestSource = 'CEngineScreenEffectOutlineGlow_ScalarDeletingDestructor_00beb4d0_test.cpp'
        PassPattern = 'OUTLINEGLOW_SCALAR_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf5c20'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_GetForegroundVertexShader_00bf5c20.cpp'
        TestSource = 'CEngineLandscapeRenderer_GetForegroundVertexShader_00bf5c20_test.cpp'
        PassPattern = 'SHADER_00BF5C20_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf5c60'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_GetForegroundSpotLightVertexShader_00bf5c60.cpp'
        TestSource = 'CEngineLandscapeRenderer_GetForegroundSpotLightVertexShader_00bf5c60_test.cpp'
        PassPattern = 'SHADER_00BF5C60_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf5ca0'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_GetForegroundShadowedVertexShader_00bf5ca0.cpp'
        TestSource = 'CEngineLandscapeRenderer_GetForegroundShadowedVertexShader_00bf5ca0_test.cpp'
        PassPattern = 'SHADER_00BF5CA0_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf5cd0'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_GetForegroundBumpShadowedVertexShader_00bf5cd0.cpp'
        TestSource = 'CEngineLandscapeRenderer_GetForegroundBumpShadowedVertexShader_00bf5cd0_test.cpp'
        PassPattern = 'SHADER_00BF5CD0_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf5d00'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_GetForegroundSpotShadowedVertexShader_00bf5d00.cpp'
        TestSource = 'CEngineLandscapeRenderer_GetForegroundSpotShadowedVertexShader_00bf5d00_test.cpp'
        PassPattern = 'SHADER_00BF5D00_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf6720'
        Module = 'CLandscapeBackgroundTreeNode'
        Source = 'CLandscapeBackgroundTreeNode_UpdateTesselation_00bf6720.cpp'
        TestSource = 'CLandscapeBackgroundTreeNode_UpdateTesselation_00bf6720_test.cpp'
        PassPattern = 'BACKGROUND_TREE_TESSELATION_TEST PASS'
    },
    [pscustomobject]@{
        Address = '008ed590'
        Module = 'CObjectFamilyDef'
        Source = 'CObjectFamilyDef_GetRandomObject_008ed590.cpp'
        TestSource = 'CObjectFamilyDef_GetRandomObject_008ed590_test.cpp'
        PassPattern = 'OBJECT_FAMILY_RANDOM_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bdf010'
        Module = 'CLocalDetailCacheMap'
        Source = 'CLocalDetailCacheMap_OpenStaticMap_00bdf010.cpp'
        TestSource = 'CLocalDetailCacheMap_OpenStaticMap_00bdf010_test.cpp'
        PassPattern = 'LOCAL_DETAIL_OPEN_STATIC_MAP_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bdd0e0'
        Module = 'CEngineLandscapeMap'
        Source = 'CEngineLandscapeMap_OpenStaticMap_00bdd0e0.cpp'
        TestSource = 'CEngineLandscapeMap_OpenStaticMap_00bdd0e0_test.cpp'
        PassPattern = 'LANDSCAPE_MAP_OPEN_STATIC_MAP_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bebed0'
        Module = 'CEnginePrimitiveManagerMeshBase'
        Source = 'CEnginePrimitiveManagerMeshBase_GetComposedTexture_00bebed0.cpp'
        TestSource = 'CEnginePrimitiveManagerMeshBase_GetComposedTexture_00bebed0_test.cpp'
        PassPattern = 'COMPOSED_TEXTURE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bdc180'
        Module = 'CEngineLandscapeMap'
        Source = 'CEngineLandscapeMap_UpdateBackgroundLOD_00bdc180.cpp'
        TestSource = 'CEngineLandscapeMap_UpdateBackgroundLOD_00bdc180_test.cpp'
        PassPattern = 'LANDSCAPE_BACKGROUND_LOD_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00be8920'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_LoadCompressed_00be8920.cpp'
        TestSource = 'CLandscapeBackgroundPatch_LoadCompressed_00be8920_test.cpp'
        PassPattern = 'LANDSCAPE_BACKGROUND_PATCH_COMPRESSED_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bec470'
        Module = 'CEnginePrimitiveManagerMeshBase'
        Source = 'CEnginePrimitiveManagerMeshBase_PreloadBumpTexture_00bec470.cpp'
        TestSource = 'CEnginePrimitiveManagerMeshBase_PreloadBumpTexture_00bec470_test.cpp'
        PassPattern = 'PRELOAD_BUMP_TEXTURE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bec950'
        Module = 'CEnginePrimitiveManagerMeshBase'
        Source = 'CEnginePrimitiveManagerMeshBase_GetOverlaidTextures_00bec950.cpp'
        TestSource = 'CEnginePrimitiveManagerMeshBase_GetOverlaidTextures_00bec950_test.cpp'
        PassPattern = 'GET_OVERLAID_TEXTURES_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab46e0'
        Module = 'CMouseDX'
        Source = 'CMouseDX_GetPos_00ab46e0.cpp'
        TestSource = 'CMouseDX_GetPos_00ab46e0_test.cpp'
        PassPattern = 'MOUSE_DX_GETPOS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf4220'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_UpdateForeground_00bf4220.cpp'
        TestSource = 'CEngineLandscapePatch_UpdateForeground_00bf4220_test.cpp'
        PassPattern = 'LANDSCAPE_PATCH_UPDATE_FOREGROUND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab4700'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetPos_00ab4700.cpp'
        TestSource = 'CMouseDX_SetPos_00ab4700_test.cpp'
        PassPattern = 'MOUSE_DX_SETPOS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf8810'
        Module = 'CLandscapeBackgroundTreeNode'
        Source = 'CLandscapeBackgroundTreeNode_UpdateStaticLOD_00bf8810.cpp'
        TestSource = 'CLandscapeBackgroundTreeNode_UpdateStaticLOD_00bf8810_test.cpp'
        PassPattern = 'BACKGROUND_TREE_STATIC_LOD_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bec000'
        Module = 'CEnginePrimitiveManagerMeshBase'
        Source = 'CEnginePrimitiveManagerMeshBase_PreloadDiffuseTexture_00bec000.cpp'
        TestSource = 'CEnginePrimitiveManagerMeshBase_PreloadDiffuseTexture_00bec000_test.cpp'
        PassPattern = 'PRELOAD_DIFFUSE_TEXTURE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f00'
        Module = 'CMouseDX'
        Source = 'CMouseDX_PeekPos_00ab5f00.cpp'
        TestSource = 'CMouseDX_PeekPos_00ab5f00_test.cpp'
        PassPattern = 'MOUSE_DX_PEEKPOS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f10'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetInputOn_00ab5f10.cpp'
        TestSource = 'CMouseDX_SetInputOn_00ab5f10_test.cpp'
        PassPattern = 'MOUSE_DX_SETINPUTON_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f20'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetRecentringOn_00ab5f20.cpp'
        TestSource = 'CMouseDX_SetRecentringOn_00ab5f20_test.cpp'
        PassPattern = 'MOUSE_DX_SETRECENTRINGON_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f30'
        Module = 'CMouseDX'
        Source = 'CMouseDX_IsRecentringOn_00ab5f30.cpp'
        TestSource = 'CMouseDX_IsRecentringOn_00ab5f30_test.cpp'
        PassPattern = 'MOUSE_DX_ISRECENTRINGON_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f40'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetDrawCursor_00ab5f40.cpp'
        TestSource = 'CMouseDX_SetDrawCursor_00ab5f40_test.cpp'
        PassPattern = 'MOUSE_DX_SETDRAWCURSOR_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f50'
        Module = 'CMouseDX'
        Source = 'CMouseDX_IsDrawingCursor_00ab5f50.cpp'
        TestSource = 'CMouseDX_IsDrawingCursor_00ab5f50_test.cpp'
        PassPattern = 'MOUSE_DX_ISDRAWINGCURSOR_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f60'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetMouseFiltering_00ab5f60.cpp'
        TestSource = 'CMouseDX_SetMouseFiltering_00ab5f60_test.cpp'
        PassPattern = 'MOUSE_DX_SETMOUSEFILTERING_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f70'
        Module = 'CMouseDX'
        Source = 'CMouseDX_IsLMBDown_00ab5f70.cpp'
        TestSource = 'CMouseDX_IsLMBDown_00ab5f70_test.cpp'
        PassPattern = 'MOUSE_DX_ISLMBDOWN_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f80'
        Module = 'CMouseDX'
        Source = 'CMouseDX_IsMMBDown_00ab5f80.cpp'
        TestSource = 'CMouseDX_IsMMBDown_00ab5f80_test.cpp'
        PassPattern = 'MOUSE_DX_ISMMBDOWN_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5f90'
        Module = 'CMouseDX'
        Source = 'CMouseDX_IsRMBDown_00ab5f90.cpp'
        TestSource = 'CMouseDX_IsRMBDown_00ab5f90_test.cpp'
        PassPattern = 'MOUSE_DX_ISRMBDown_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65d30'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_ConsolidateFreeAreas_00a65d30.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_ConsolidateFreeAreas_00a65d30_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_CONSOLIDATE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65d40'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_GET_SIZE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65d50'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_00a65d50.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_00a65d50_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_TOTAL_MEMORY_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a66550'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetFragmentation_00a66550.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetFragmentation_00a66550_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_FRAGMENTATION_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65de0'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_00a65de0.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_00a65de0_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_SIZE_WITH_HEADER_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65d70'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetHeaderOverhead_00a65d70.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetHeaderOverhead_00a65d70_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_HEADER_OVERHEAD_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65df0'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_00a65df0.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_00a65df0_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_LARGEST_FREE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65e20'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetNoFreeAreas_00a65e20.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetNoFreeAreas_00a65e20_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_NO_FREE_AREAS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65e50'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetNoAllocatedAreas_00a65e50.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetNoAllocatedAreas_00a65e50_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_NO_ALLOCATED_AREAS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00a65da0'
        Module = 'CMemoryAllocatorFixedSize'
        Source = 'CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_00a65da0.cpp'
        TestSource = 'CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_00a65da0_test.cpp'
        PassPattern = 'FIXED_ALLOCATOR_TOTAL_ALLOCATED_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bdc130'
        Module = 'CEngineLandscapeMap'
        Source = 'CEngineLandscapeMap_RelocateData_00bdc130.cpp'
        TestSource = 'CEngineLandscapeMap_RelocateData_00bdc130_test.cpp'
        PassPattern = 'LANDSCAPE_MAP_RELOCATE_DATA_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00b6ca10'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_PeekSceneFilterFlags_00b6ca10.cpp'
        TestSource = 'CEngineLandscapeRenderer_PeekSceneFilterFlags_00b6ca10_test.cpp'
        PassPattern = 'LANDSCAPE_RENDERER_SCENE_FILTER_FLAGS_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf3840'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_CanRelocateData_00bf3840.cpp'
        TestSource = 'CEngineLandscapePatch_CanRelocateData_00bf3840_test.cpp'
        PassPattern = 'LANDSCAPE_PATCH_CAN_RELOCATE_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf3980'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_RelocateData_00bf3980.cpp'
        TestSource = 'CEngineLandscapePatch_RelocateData_00bf3980_test.cpp'
        PassPattern = 'LANDSCAPE_PATCH_RELOCATE_DATA_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00b676a0'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_00b676a0.cpp'
        TestSource = 'CEngineLandscapeRenderer_OnReleaseDefaultPoolResources_00b676a0_test.cpp'
        PassPattern = 'LANDSCAPE_RENDERER_RELEASE_POOL_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00ab5390'
        Module = 'CMouseDX'
        Source = 'CMouseDX_SetMouseCursor_00ab5390.cpp'
        TestSource = 'CMouseDX_SetMouseCursor_00ab5390_test.cpp'
        PassPattern = 'MOUSE_DX_SET_MOUSE_CURSOR_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bdc510'
        Module = 'CEngineLandscapeMap'
        Source = 'CEngineLandscapeMap_UpdateForeground_00bdc510.cpp'
        TestSource = 'CEngineLandscapeMap_UpdateForeground_00bdc510_test.cpp'
        PassPattern = 'LANDSCAPE_MAP_UPDATE_FOREGROUND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf7400'
        Module = 'CLandscapeBackgroundTreeNode'
        Source = 'CLandscapeBackgroundTreeNode_LoadHeader_00bf7400.cpp'
        TestSource = 'CLandscapeBackgroundTreeNode_LoadHeader_00bf7400_test.cpp'
        PassPattern = 'BACKGROUND_TREE_LOAD_HEADER_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00b687d0'
        Module = 'CEngineLandscapeRenderer'
        Source = 'CEngineLandscapeRenderer_BuildThemes_00b687d0.cpp'
        TestSource = 'CEngineLandscapeRenderer_BuildThemes_00b687d0_test.cpp'
        PassPattern = 'LANDSCAPE_RENDERER_BUILD_THEMES_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf3b60'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_LoadForegroundPatch_00bf3b60.cpp'
        TestSource = 'CEngineLandscapePatch_LoadForegroundPatch_00bf3b60_test.cpp'
        PassPattern = 'LANDSCAPE_PATCH_LOAD_FOREGROUND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bfe050'
        Module = 'CLandscapeLayerMesh'
        Source = 'CLandscapeLayerMesh_LoadForeground_00bfe050.cpp'
        TestSource = 'CLandscapeLayerMesh_LoadForeground_00bfe050_test.cpp'
        PassPattern = 'LANDSCAPE_LAYER_MESH_LOAD_FOREGROUND_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00be7d70'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_Load_00be7d70.cpp'
        TestSource = 'CLandscapeBackgroundPatch_Load_00be7d70_test.cpp'
        PassPattern = 'LANDSCAPE_BACKGROUND_PATCH_LOAD_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00be6f70'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_Render_00be6f70.cpp'
        TestSource = 'CLandscapeBackgroundPatch_Render_00be6f70_test.cpp'
        PassPattern = 'LANDSCAPE_BACKGROUND_PATCH_RENDER_TEST PASS'
    },
    [pscustomobject]@{
        Address = '00bf4570'
        Module = 'CEngineLandscapePatch'
        Source = 'CEngineLandscapePatch_RenderForeground_00bf4570.cpp'
        TestSource = 'CEngineLandscapePatch_RenderForeground_00bf4570_test.cpp'
        PassPattern = 'LANDSCAPE_PATCH_RENDER_FOREGROUND_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c19f90'
        Module = '_global'
        Source = 'Global_GetterField84_00c19f90.cpp'
        TestSource = 'Global_GetterField84_00c19f90_test.cpp'
        PassPattern = 'GETTER_FIELD84_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c2d7b0'
        Module = '_global'
        Source = 'Global_CTCInventoryCheckSlotTypeNegative_00c2d7b0.cpp'
        TestSource = 'Global_CTCInventoryCheckSlotTypeNegative_00c2d7b0_test.cpp'
        PassPattern = 'CTCINVENTORY_CHECKSLOTTYPENEGATIVE_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042da70'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_IsParentDefCDefClassBaseUBENXZ_0042da70.cpp'
        TestSource = 'CDefClassBase_IsParentDefCDefClassBaseUBENXZ_0042da70_test.cpp'
        PassPattern = 'CDEFCLASSBASE_0042da70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042daa0'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_GetPSubDefInfoMapCDefClassBaseUAEPAV_0042daa0.cpp'
        TestSource = 'CDefClassBase_GetPSubDefInfoMapCDefClassBaseUAEPAV_0042daa0_test.cpp'
        PassPattern = 'CDEFCLASSBASE_0042daa0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cf2c'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0041cf2c.cpp'
        TestSource = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0041cf2c_test.cpp'
        PassPattern = 'CTCINVENTORYBASE_0041cf2c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00ba7fe0'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMorphInfoCEngineInternalPrimitive_00ba7fe0.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMorphInfoCEngineInternalPrimitive_00ba7fe0_test.cpp'
        PassPattern = 'CENGINEINTERNALPRIMITIVEMESHBASE_00ba7fe0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00692460'
        Module = 'CCreatureActionBase'
        Source = 'CCreatureActionBase_GetAnimDelayTimeCCreatureActionBaseU_00692460.cpp'
        TestSource = 'CCreatureActionBase_GetAnimDelayTimeCCreatureActionBaseU_00692460_test.cpp'
        PassPattern = 'CCREATUREACTIONBASE_00692460_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00661ec0'
        Module = 'CThingCreatureBase'
        Source = 'CThingCreatureBase_GetCombatManager_00661ec0.cpp'
        TestSource = 'CThingCreatureBase_GetCombatManager_00661ec0_test.cpp'
        PassPattern = 'CTHINGCREATUREBASE_00661ec0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00661ed0'
        Module = 'CThingCreatureBase'
        Source = 'CThingCreatureBase_GetCombatManager_00661ed0.cpp'
        TestSource = 'CThingCreatureBase_GetCombatManager_00661ed0_test.cpp'
        PassPattern = 'CTHINGCREATUREBASE_00661ed0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00697700'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_GetControlAngleXYCGameCameraManagerU_00697700.cpp'
        TestSource = 'CGameCameraManager_GetControlAngleXYCGameCameraManagerU_00697700_test.cpp'
        PassPattern = 'CGAMECAMERAMANAGER_00697700_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00697710'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_GetNoControlMovementTime_00697710.cpp'
        TestSource = 'CGameCameraManager_GetNoControlMovementTime_00697710_test.cpp'
        PassPattern = 'CGAMECAMERAMANAGER_00697710_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0057a67e'
        Module = 'CTCHeroStats'
        Source = 'CTCHeroStats_GetMoralityMax_0057a67e.cpp'
        TestSource = 'CTCHeroStats_GetMoralityMax_0057a67e_test.cpp'
        PassPattern = 'CTCHEROSTATS_0057a67e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004bc180'
        Module = 'CTCGraphicAppearance'
        Source = 'CTCGraphicAppearance_GetFadeDistance_004bc180.cpp'
        TestSource = 'CTCGraphicAppearance_GetFadeDistance_004bc180_test.cpp'
        PassPattern = 'CTCGRAPHICAPPEARANCE_004bc180_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7990'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c7990.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c7990_test.cpp'
        PassPattern = 'CVIEWBASE_004c7990_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c79d0'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c79d0.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c79d0_test.cpp'
        PassPattern = 'CVIEWBASE_004c79d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008cff10'
        Module = 'CGameScriptThing'
        Source = 'CGameScriptThing_GetAngleXYCGameScriptThingUBEMXZ_008cff10.cpp'
        TestSource = 'CGameScriptThing_GetAngleXYCGameScriptThingUBEMXZ_008cff10_test.cpp'
        PassPattern = 'CGAMESCRIPTTHING_008cff10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00548670'
        Module = 'CEditControlCentre'
        Source = 'CEditControlCentre_PeekConnectedToTrackNode_00548670.cpp'
        TestSource = 'CEditControlCentre_PeekConnectedToTrackNode_00548670_test.cpp'
        PassPattern = 'CEDITCONTROLCENTRE_00548670_TEST PASS'
    }
    [pscustomobject]@{
        Address = '007443f0'
        Module = 'CTCPerceiveThing'
        Source = 'CTCPerceiveThing_IsUpdatingThisFrame_007443f0.cpp'
        TestSource = 'CTCPerceiveThing_IsUpdatingThisFrame_007443f0_test.cpp'
        PassPattern = 'CTCPERCEIVETHING_007443f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b23360'
        Module = 'CIEngine'
        Source = 'CIEngine_GetPrimitiveDrawDistanceFactorCIEngi_00b23360.cpp'
        TestSource = 'CIEngine_GetPrimitiveDrawDistanceFactorCIEngi_00b23360_test.cpp'
        PassPattern = 'CIENGINE_00b23360_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b23470'
        Module = 'CIEngine'
        Source = 'CIEngine_GetStaticMapUpdatePassCountCIEngineU_00b23470.cpp'
        TestSource = 'CIEngine_GetStaticMapUpdatePassCountCIEngineU_00b23470_test.cpp'
        PassPattern = 'CIENGINE_00b23470_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041e5f2'
        Module = '_global'
        Source = 'global_GetInstance_0041e5f2.cpp'
        TestSource = 'global_GetInstance_0041e5f2_test.cpp'
        PassPattern = 'GLOBAL_0041e5f2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00419603'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_00419603.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_00419603_test.cpp'
        PassPattern = 'CENGINEINTERNALPRIMITIVEMESHBASE_00419603_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cc14'
        Module = 'CTCLook'
        Source = 'CTCLook_GetHeadAngularVelocityMaxXY_0041cc14.cpp'
        TestSource = 'CTCLook_GetHeadAngularVelocityMaxXY_0041cc14_test.cpp'
        PassPattern = 'CTCLOOK_0041cc14_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00658604'
        Module = 'CTCVillage'
        Source = 'CTCVillage_GetIsHeroEnemyOfGuards_00658604.cpp'
        TestSource = 'CTCVillage_GetIsHeroEnemyOfGuards_00658604_test.cpp'
        PassPattern = 'CTCVILLAGE_00658604_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b8fde0'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_ClearAllChildPrimitivesCEngineIntern_00b8fde0.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_ClearAllChildPrimitivesCEngineIntern_00b8fde0_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_00b8fde0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b3aa80'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_GetAsOcclusionPrimitiveCEngineIntern_00b3aa80.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_GetAsOcclusionPrimitiveCEngineIntern_00b3aa80_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_00b3aa80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006924d0'
        Module = 'CCreatureActionBase'
        Source = 'CCreatureActionBase_GetAnimExtraFlagsCCreatureActionBase_006924d0.cpp'
        TestSource = 'CCreatureActionBase_GetAnimExtraFlagsCCreatureActionBase_006924d0_test.cpp'
        PassPattern = 'CCreatureActionBase_006924d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006924f0'
        Module = 'CCreatureActionBase'
        Source = 'CCreatureActionBase_IsStrikeEventActiveCCreatureActionBa_006924f0.cpp'
        TestSource = 'CCreatureActionBase_IsStrikeEventActiveCCreatureActionBa_006924f0_test.cpp'
        PassPattern = 'CCreatureActionBase_006924f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00692500'
        Module = 'CCreatureActionBase'
        Source = 'CCreatureActionBase_IsStrikeActionCCreatureActionBaseUBE_00692500.cpp'
        TestSource = 'CCreatureActionBase_IsStrikeActionCCreatureActionBaseUBE_00692500_test.cpp'
        PassPattern = 'CCreatureActionBase_00692500_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0062ea00'
        Module = 'CCombatActionBase'
        Source = 'CCombatActionBase_GetAnimDelayTimeCCombatActionBaseUBE_0062ea00.cpp'
        TestSource = 'CCombatActionBase_GetAnimDelayTimeCCombatActionBaseUBE_0062ea00_test.cpp'
        PassPattern = 'CCombatActionBase_0062ea00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b235d0'
        Module = 'CIEngine'
        Source = 'CIEngine_IsPreloadedCIEngineUBENXZ_00b235d0.cpp'
        TestSource = 'CIEngine_IsPreloadedCIEngineUBENXZ_00b235d0_test.cpp'
        PassPattern = 'CIEngine_00b235d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b23690'
        Module = 'CIEngine'
        Source = 'CIEngine_GetLoadingMemoryPoolCIEngineUAEPAVCM_00b23690.cpp'
        TestSource = 'CIEngine_GetLoadingMemoryPoolCIEngineUAEPAVCM_00b23690_test.cpp'
        PassPattern = 'CIEngine_00b23690_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b236a0'
        Module = 'CIEngine'
        Source = 'CIEngine_GetAnimationMemoryPoolCIEngineUAEPAV_00b236a0.cpp'
        TestSource = 'CIEngine_GetAnimationMemoryPoolCIEngineUAEPAV_00b236a0_test.cpp'
        PassPattern = 'CIEngine_00b236a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00be8dc0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_IsPatchCLandscapeBackgroundPatchEAEN_00be8dc0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_IsPatchCLandscapeBackgroundPatchEAEN_00be8dc0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00be8dc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008cff30'
        Module = 'CGameScriptThing'
        Source = 'CGameScriptThing_IsUnconsciousCGameScriptThingUBENXZ_008cff30.cpp'
        TestSource = 'CGameScriptThing_IsUnconsciousCGameScriptThingUBENXZ_008cff30_test.cpp'
        PassPattern = 'CGameScriptThing_008cff30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008fcfe0'
        Module = 'CAIStateGroupBase'
        Source = 'CAIStateGroupBase_GetDebugTextCAIStateGroupBaseUBEPBVC_008fcfe0.cpp'
        TestSource = 'CAIStateGroupBase_GetDebugTextCAIStateGroupBaseUBEPBVC_008fcfe0_test.cpp'
        PassPattern = 'CAIStateGroupBase_008fcfe0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0049bb50'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0049bb50.cpp'
        TestSource = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0049bb50_test.cpp'
        PassPattern = 'CTCInventoryBase_0049bb50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005bc501'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCanItemBeQuickAssigned_005bc501.cpp'
        TestSource = 'CTCInventoryBase_GetCanItemBeQuickAssigned_005bc501_test.cpp'
        PassPattern = 'CTCInventoryBase_005bc501_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b23550'
        Module = 'CIEngine'
        Source = 'CIEngine_IsActiveCIEngineUBENXZ_00b23550.cpp'
        TestSource = 'CIEngine_IsActiveCIEngineUBENXZ_00b23550_test.cpp'
        PassPattern = 'CIEngine_00b23550_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b3a980'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_GetChildPrimitiveCEngineInternalPrim_00b3a980.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_GetChildPrimitiveCEngineInternalPrim_00b3a980_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_00b3a980_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b3a9c0'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_GetWorldPositionCEngineInternalPrimi_00b3a9c0.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_GetWorldPositionCEngineInternalPrimi_00b3a9c0_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_00b3a9c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0062ea20'
        Module = 'CCombatActionBase'
        Source = 'CCombatActionBase_GetAnimExtraFlagsCCombatActionBaseUB_0062ea20.cpp'
        TestSource = 'CCombatActionBase_GetAnimExtraFlagsCCombatActionBaseUB_0062ea20_test.cpp'
        PassPattern = 'CCombatActionBase_0062ea20_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0051e2e0'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0051e2e0.cpp'
        TestSource = 'CTCInventoryBase_GetCanItemBeQuickAssigned_0051e2e0_test.cpp'
        PassPattern = 'CTCInventoryBase_0051e2e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00670000'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_GetAirResistance_00670000.cpp'
        TestSource = 'CTCPhysicsBase_GetAirResistance_00670000_test.cpp'
        PassPattern = 'CTCPhysicsBase_00670000_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00697750'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_SetChangeInControlDirectionCGameCame_00697750.cpp'
        TestSource = 'CGameCameraManager_SetChangeInControlDirectionCGameCame_00697750_test.cpp'
        PassPattern = 'CGameCameraManager_00697750_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c79e0'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c79e0.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c79e0_test.cpp'
        PassPattern = 'CViewBase_004c79e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c79f0'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c79f0.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c79f0_test.cpp'
        PassPattern = 'CViewBase_004c79f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7a00'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c7a00.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c7a00_test.cpp'
        PassPattern = 'CViewBase_004c7a00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7a70'
        Module = 'CViewBase'
        Source = 'CViewBase_GetDisplayEngine_004c7a70.cpp'
        TestSource = 'CViewBase_GetDisplayEngine_004c7a70_test.cpp'
        PassPattern = 'CViewBase_004c7a70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00437640'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00437640.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00437640_test.cpp'
        PassPattern = 'CTCShotDefinitionBase_00437640_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00437870'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00437870.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00437870_test.cpp'
        PassPattern = 'CTCShotDefinitionBase_00437870_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00445b80'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445b80.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445b80_test.cpp'
        PassPattern = 'CTCShotDefinitionBase_00445b80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00445b90'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445b90.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445b90_test.cpp'
        PassPattern = 'CTCShotDefinitionBase_00445b90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006bd830'
        Module = 'CTCPhysicsNavigator'
        Source = 'CTCPhysicsNavigator_EnableNavigator_006bd830.cpp'
        TestSource = 'CTCPhysicsNavigator_EnableNavigator_006bd830_test.cpp'
        PassPattern = 'CTCPhysicsNavigator_006bd830_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0072f0e0'
        Module = 'CTCPhysicsNavigator'
        Source = 'CTCPhysicsNavigator_EnableNavigator_0072f0e0.cpp'
        TestSource = 'CTCPhysicsNavigator_EnableNavigator_0072f0e0_test.cpp'
        PassPattern = 'CTCPhysicsNavigator_0072f0e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00b23f60'
        Module = 'CEngine'
        Source = 'CEngine_GetScratchBufferTextureManagerCEngin_00b23f60.cpp'
        TestSource = 'CEngine_GetScratchBufferTextureManagerCEngin_00b23f60_test.cpp'
        PassPattern = 'CEngine_00b23f60_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c6b6'
        Module = '_global'
        Source = 'global_IsAllocated_0044c6b6.cpp'
        TestSource = 'global_IsAllocated_0044c6b6_test.cpp'
        PassPattern = '_global_0044c6b6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00661e70'
        Module = 'CTCPhysicsStandard'
        Source = 'CTCPhysicsStandard_CanCollideWithLandscape_00661e70.cpp'
        TestSource = 'CTCPhysicsStandard_CanCollideWithLandscape_00661e70_test.cpp'
        PassPattern = 'CTCPhysicsStandard_00661e70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7a80'
        Module = 'CAnimateThing'
        Source = 'CAnimateThing_GetMeshBank_004c7a80.cpp'
        TestSource = 'CAnimateThing_GetMeshBank_004c7a80_test.cpp'
        PassPattern = 'CAnimateThing_004c7a80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008fcfd0'
        Module = 'CAIStateGroupBase'
        Source = 'CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp'
        TestSource = 'CAIStateGroupBase_GetPWorldSeed_008fcfd0_test.cpp'
        PassPattern = 'CAIStateGroupBase_008fcfd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00692470'
        Module = 'CCreatureActionBase'
        Source = 'CCreatureActionBase_GetAnimStartFrame_00692470.cpp'
        TestSource = 'CCreatureActionBase_GetAnimStartFrame_00692470_test.cpp'
        PassPattern = 'CCreatureActionBase_00692470_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00662090'
        Module = 'CThingCreatureBase'
        Source = 'CThingCreatureBase_GetDazedHealthThreshold_00662090.cpp'
        TestSource = 'CThingCreatureBase_GetDazedHealthThreshold_00662090_test.cpp'
        PassPattern = 'CThingCreatureBase_00662090_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041c965'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041c965.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041c965_test.cpp'
        PassPattern = 'CObservable_0041c965_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041c972'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041c972.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041c972_test.cpp'
        PassPattern = 'CObservable_0041c972_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041c994'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041c994.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041c994_test.cpp'
        PassPattern = 'CObservable_0041c994_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ca31'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041ca31.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041ca31_test.cpp'
        PassPattern = 'CObservable_0041ca31_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006620f0'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_GetAirResistance_006620f0.cpp'
        TestSource = 'CTCPhysicsBase_GetAirResistance_006620f0_test.cpp'
        PassPattern = 'CTCPhysicsBase_006620f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00662100'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_GetAirResistance_00662100.cpp'
        TestSource = 'CTCPhysicsBase_GetAirResistance_00662100_test.cpp'
        PassPattern = 'CTCPhysicsBase_00662100_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00662190'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_GetAirResistance_00662190.cpp'
        TestSource = 'CTCPhysicsBase_GetAirResistance_00662190_test.cpp'
        PassPattern = 'CTCPhysicsBase_00662190_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005bc0c6'
        Module = 'CComponent'
        Source = 'CComponent_GetCurrentState_005bc0c6.cpp'
        TestSource = 'CComponent_GetCurrentState_005bc0c6_test.cpp'
        PassPattern = 'CComponent_005bc0c6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008fd000'
        Module = 'CAIStateGroupBase'
        Source = 'CAIStateGroupBase_PeekThingSearchTools_008fd000.cpp'
        TestSource = 'CAIStateGroupBase_PeekThingSearchTools_008fd000_test.cpp'
        PassPattern = 'CAIStateGroupBase_008fd000_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0062ea30'
        Module = 'CCombatActionBase'
        Source = 'CCombatActionBase_IsStrikeActionCCombatActionBaseUBENX_0062ea30.cpp'
        TestSource = 'CCombatActionBase_IsStrikeActionCCombatActionBaseUBENX_0062ea30_test.cpp'
        PassPattern = 'CCombatActionBase_0062ea30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008fcff0'
        Module = 'CAIStateGroupBase'
        Source = 'CAIStateGroupBase_PeekWorldMap_008fcff0.cpp'
        TestSource = 'CAIStateGroupBase_PeekWorldMap_008fcff0_test.cpp'
        PassPattern = 'CAIStateGroupBase_008fcff0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0050e690'
        Module = '_global'
        Source = 'global_SetLungedThisFrame_0050e690.cpp'
        TestSource = 'global_SetLungedThisFrame_0050e690_test.cpp'
        PassPattern = '_global_0050e690_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c79a0'
        Module = 'CAnimateThing'
        Source = 'CAnimateThing_GetMeshBank_004c79a0.cpp'
        TestSource = 'CAnimateThing_GetMeshBank_004c79a0_test.cpp'
        PassPattern = 'CAnimateThing_004c79a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7d90'
        Module = 'CAnimateThing'
        Source = 'CAnimateThing_GetMeshBank_004c7d90.cpp'
        TestSource = 'CAnimateThing_GetMeshBank_004c7d90_test.cpp'
        PassPattern = 'CAnimateThing_004c7d90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00cb13a0'
        Module = 'CEditControlCentre'
        Source = 'CEditControlCentre_PeekConnectedToTrackNode_00cb13a0.cpp'
        TestSource = 'CEditControlCentre_PeekConnectedToTrackNode_00cb13a0_test.cpp'
        PassPattern = 'CEditControlCentre_00cb13a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7930'
        Module = 'CThing'
        Source = 'CThing_IsLevelLess_004c7930.cpp'
        TestSource = 'CThing_IsLevelLess_004c7930_test.cpp'
        PassPattern = 'CThing_004c7930_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0057a89d'
        Module = 'CTCHeroStats'
        Source = 'CTCHeroStats_GetMaxDrunkenness_0057a89d.cpp'
        TestSource = 'CTCHeroStats_GetMaxDrunkenness_0057a89d_test.cpp'
        PassPattern = 'CTCHeroStats_0057a89d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006ceae0'
        Module = 'CTCHeroExperience'
        Source = 'CTCHeroExperience_AddSuccessfulBlockParry_006ceae0.cpp'
        TestSource = 'CTCHeroExperience_AddSuccessfulBlockParry_006ceae0_test.cpp'
        PassPattern = 'CTCHeroExperience_006ceae0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c38af0'
        Module = 'CComponent'
        Source = 'CComponent_GetCurrentState_00c38af0.cpp'
        TestSource = 'CComponent_GetCurrentState_00c38af0_test.cpp'
        PassPattern = 'CComponent_00c38af0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005486d0'
        Module = 'CEditControlCentre'
        Source = 'CEditControlCentre_PeekConnectedToTrackNode_005486d0.cpp'
        TestSource = 'CEditControlCentre_PeekConnectedToTrackNode_005486d0_test.cpp'
        PassPattern = 'CEditControlCentre_005486d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005486f0'
        Module = 'CEditControlCentre'
        Source = 'CEditControlCentre_PeekConnectedToTrackNode_005486f0.cpp'
        TestSource = 'CEditControlCentre_PeekConnectedToTrackNode_005486f0_test.cpp'
        PassPattern = 'CEditControlCentre_005486f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009acca0'
        Module = 'CDefinitionManager'
        Source = 'CDefinitionManager_GetDefClassIndexFromGlobalIndex_009acca0.cpp'
        TestSource = 'CDefinitionManager_GetDefClassIndexFromGlobalIndex_009acca0_test.cpp'
        PassPattern = 'CDefinitionManager_009acca0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '007331a0'
        Module = 'CTCCombat'
        Source = 'CTCCombat_IsAbleToBeKnockedDown_007331a0.cpp'
        TestSource = 'CTCCombat_IsAbleToBeKnockedDown_007331a0_test.cpp'
        PassPattern = 'CTCCombat_007331a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0049c780'
        Module = 'CThingSearchTools'
        Source = 'CThingSearchTools_PeekTypeList_0049c780.cpp'
        TestSource = 'CThingSearchTools_PeekTypeList_0049c780_test.cpp'
        PassPattern = 'CThingSearchTools_0049c780_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005bc99c'
        Module = 'CCategory'
        Source = 'CCategory_GetHighlightedItem_005bc99c.cpp'
        TestSource = 'CCategory_GetHighlightedItem_005bc99c_test.cpp'
        PassPattern = 'CCategory_005bc99c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006585ee'
        Module = 'CTCHeroExperience'
        Source = 'CTCHeroExperience_AddSuccessfulBlockParry_006585ee.cpp'
        TestSource = 'CTCHeroExperience_AddSuccessfulBlockParry_006585ee_test.cpp'
        PassPattern = 'CTCHeroExperience_006585ee_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c8b9b0'
        Module = 'CCategory'
        Source = 'CCategory_GetHighlightedItem_00c8b9b0.cpp'
        TestSource = 'CCategory_GetHighlightedItem_00c8b9b0_test.cpp'
        PassPattern = 'CCategory_00c8b9b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00661f70'
        Module = 'CThingCreatureBase'
        Source = 'CThingCreatureBase_IsChild_00661f70.cpp'
        TestSource = 'CThingCreatureBase_IsChild_00661f70_test.cpp'
        PassPattern = 'CThingCreatureBase_00661f70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0066e7bd'
        Module = 'CManager'
        Source = 'CManager_GetCurrentScreen_0066e7bd.cpp'
        TestSource = 'CManager_GetCurrentScreen_0066e7bd_test.cpp'
        PassPattern = 'CManager_0066e7bd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00668a80'
        Module = 'CAnimateThing'
        Source = 'CAnimateThing_IsAnimationFinished_00668a80.cpp'
        TestSource = 'CAnimateThing_IsAnimationFinished_00668a80_test.cpp'
        PassPattern = 'CAnimateThing_00668a80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7a10'
        Module = 'CThing'
        Source = 'CThing_GetPlayer_004c7a10.cpp'
        TestSource = 'CThing_GetPlayer_004c7a10_test.cpp'
        PassPattern = 'CThing_004c7a10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004c7a30'
        Module = 'CThing'
        Source = 'CThing_GetPlayer_004c7a30.cpp'
        TestSource = 'CThing_GetPlayer_004c7a30_test.cpp'
        PassPattern = 'CThing_004c7a30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004af290'
        Module = 'CQuestManager'
        Source = 'CQuestManager_IsBetweenQuestStartAndFinishScreens_004af290.cpp'
        TestSource = 'CQuestManager_IsBetweenQuestStartAndFinishScreens_004af290_test.cpp'
        PassPattern = 'CQuestManager_004af290_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00638180'
        Module = 'CTCLook'
        Source = 'CTCLook_IsTurningBody_00638180.cpp'
        TestSource = 'CTCLook_IsTurningBody_00638180_test.cpp'
        PassPattern = 'CTCLook_00638180_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005e9c30'
        Module = 'CTCTavernGame'
        Source = 'CTCTavernGame_IsBetting_005e9c30.cpp'
        TestSource = 'CTCTavernGame_IsBetting_005e9c30_test.cpp'
        PassPattern = 'CTCTavernGame_005e9c30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005bd4ce'
        Module = 'CTCWeapon'
        Source = 'CTCWeapon_IsAnalogueLoading_005bd4ce.cpp'
        TestSource = 'CTCWeapon_IsAnalogueLoading_005bd4ce_test.cpp'
        PassPattern = 'CTCWeapon_005bd4ce_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006cb9d0'
        Module = 'CTCCreatureNavigation'
        Source = 'CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects_006cb9d0.cpp'
        TestSource = 'CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects_006cb9d0_test.cpp'
        PassPattern = 'CTCCreatureNavigation_006cb9d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '007331c0'
        Module = 'CTCCombat'
        Source = 'CTCCombat_IsVulnerableToUnblockableAttack_007331c0.cpp'
        TestSource = 'CTCCombat_IsVulnerableToUnblockableAttack_007331c0_test.cpp'
        PassPattern = 'CTCCombat_007331c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006bbf20'
        Module = 'CRegion'
        Source = 'CRegion_GetDisplayName_006bbf20.cpp'
        TestSource = 'CRegion_GetDisplayName_006bbf20_test.cpp'
        PassPattern = 'CRegion_006bbf20_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006d6570'
        Module = 'CTCHeroExperience'
        Source = 'CTCHeroExperience_GetExperiencePickUpMultiplier_006d6570.cpp'
        TestSource = 'CTCHeroExperience_GetExperiencePickUpMultiplier_006d6570_test.cpp'
        PassPattern = 'CTCHeroExperience_006d6570_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005d9270'
        Module = 'CTCWeapon'
        Source = 'CTCWeapon_IsAnalogueLoading_005d9270.cpp'
        TestSource = 'CTCWeapon_IsAnalogueLoading_005d9270_test.cpp'
        PassPattern = 'CTCWeapon_005d9270_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00713290'
        Module = 'CCategory'
        Source = 'CCategory_GetHighlightedItemCountInCategory_00713290.cpp'
        TestSource = 'CCategory_GetHighlightedItemCountInCategory_00713290_test.cpp'
        PassPattern = 'CCategory_00713290_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00698470'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_SetProjectileWeaponFired_00698470.cpp'
        TestSource = 'CGameCameraManager_SetProjectileWeaponFired_00698470_test.cpp'
        PassPattern = 'CGameCameraManager_00698470_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0057a753'
        Module = 'CTCHeroExperience'
        Source = 'CTCHeroExperience_AddSuccessfulBlockParry_0057a753.cpp'
        TestSource = 'CTCHeroExperience_AddSuccessfulBlockParry_0057a753_test.cpp'
        PassPattern = 'CTCHeroExperience_0057a753_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006cba20'
        Module = 'CTCCreatureNavigation'
        Source = 'CTCCreatureNavigation_IsPosNavLineOfSightTo_006cba20.cpp'
        TestSource = 'CTCCreatureNavigation_IsPosNavLineOfSightTo_006cba20_test.cpp'
        PassPattern = 'CTCCreatureNavigation_006cba20_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004083b0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetDefaultValuesGameplay_004083b0.cpp'
        TestSource = 'CUserProfileManager_SetDefaultValuesGameplay_004083b0_test.cpp'
        PassPattern = 'CUserProfileManager_004083b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00697ae0'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_IsUsingScriptedCameraMode_00697ae0.cpp'
        TestSource = 'CGameCameraManager_IsUsingScriptedCameraMode_00697ae0_test.cpp'
        PassPattern = 'CGameCameraManager_00697ae0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00988320'
        Module = 'CShaderRenderManager'
        Source = 'CShaderRenderManager_SetFogColour_00988320.cpp'
        TestSource = 'CShaderRenderManager_SetFogColour_00988320_test.cpp'
        PassPattern = 'CShaderRenderManager_00988320_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006cb8f0'
        Module = 'CTCCreatureNavigation'
        Source = 'CTCCreatureNavigation_IsCurrentlyNavigating_006cb8f0.cpp'
        TestSource = 'CTCCreatureNavigation_IsCurrentlyNavigating_006cb8f0_test.cpp'
        PassPattern = 'CTCCreatureNavigation_006cb8f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006a4e40'
        Module = 'CParticleSystem'
        Source = 'CParticleSystem_GetNormalisedTime_006a4e40.cpp'
        TestSource = 'CParticleSystem_GetNormalisedTime_006a4e40_test.cpp'
        PassPattern = 'CParticleSystem_006a4e40_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00661f90'
        Module = 'CThingCreatureBase'
        Source = 'CThingCreatureBase_GetCombatRadius_00661f90.cpp'
        TestSource = 'CThingCreatureBase_GetCombatRadius_00661f90_test.cpp'
        PassPattern = 'CThingCreatureBase_00661f90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0057a690'
        Module = 'CTCHeroStats'
        Source = 'CTCHeroStats_GetMoralityPercentage_0057a690.cpp'
        TestSource = 'CTCHeroStats_GetMoralityPercentage_0057a690_test.cpp'
        PassPattern = 'CTCHeroStats_0057a690_TEST PASS'
    }
    [pscustomobject]@{
        Address = '005bd96b'
        Module = 'CCategory'
        Source = 'CCategory_GetFirstEmptySlot_005bd96b.cpp'
        TestSource = 'CCategory_GetFirstEmptySlot_005bd96b_test.cpp'
        PassPattern = 'CCategory_005bd96b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0057a874'
        Module = 'CTCHeroStats'
        Source = 'CTCHeroStats_IsDrunk_0057a874.cpp'
        TestSource = 'CTCHeroStats_IsDrunk_0057a874_test.cpp'
        PassPattern = 'CTCHeroStats_0057a874_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00726750'
        Module = 'CTCPhysicsNavigator'
        Source = 'CTCPhysicsNavigator_SetPosition_00726750.cpp'
        TestSource = 'CTCPhysicsNavigator_SetPosition_00726750_test.cpp'
        PassPattern = 'CTCPhysicsNavigator_00726750_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009be800'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_IsSupportingCompressedTextures_009be800.cpp'
        TestSource = 'CDisplayManager_IsSupportingCompressedTextures_009be800_test.cpp'
        PassPattern = 'CDisplayManager_009be800_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00be5c30'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_ClearTextures_00be5c30.cpp'
        TestSource = 'CLandscapeBackgroundPatch_ClearTextures_00be5c30_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00be5c30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f0e0'
        Module = '_global'
        Source = 'global_ResetDimensions_0040f0e0.cpp'
        TestSource = 'global_ResetDimensions_0040f0e0_test.cpp'
        PassPattern = '_global_0040f0e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008d0440'
        Module = 'CGameScriptThing'
        Source = 'CGameScriptThing_GetPThingCGameScriptThingUBEPAVCThin_008d0440.cpp'
        TestSource = 'CGameScriptThing_GetPThingCGameScriptThingUBEPAVCThin_008d0440_test.cpp'
        PassPattern = 'CGameScriptThing_008d0440_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c3e730'
        Module = 'CManager'
        Source = 'CManager_ClearErrorMessage_00c3e730.cpp'
        TestSource = 'CManager_ClearErrorMessage_00c3e730_test.cpp'
        PassPattern = 'CManager_00c3e730_TEST_PASS'
    }
    [pscustomobject]@{
        Address = '0042a5f3'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a5f3.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a5f3_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveMeshBase_0042a5f3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042a622'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a622.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a622_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveMeshBase_0042a622_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042a665'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a665.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a665_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveMeshBase_0042a665_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042a694'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a694.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a694_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveMeshBase_0042a694_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00aed1b0'
        Module = 'CParticleEmitter'
        Source = 'CParticleEmitter_HasAllocatedDecals_00aed1b0.cpp'
        TestSource = 'CParticleEmitter_HasAllocatedDecals_00aed1b0_test.cpp'
        PassPattern = 'CParticleEmitter_00aed1b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00aed220'
        Module = 'CParticleEmitter'
        Source = 'CParticleEmitter_HasAllocatedDecals_00aed220.cpp'
        TestSource = 'CParticleEmitter_HasAllocatedDecals_00aed220_test.cpp'
        PassPattern = 'CParticleEmitter_00aed220_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006818a0'
        Module = 'CTCCreatureOpinionOfHero'
        Source = 'CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero_006818a0.cpp'
        TestSource = 'CTCCreatureOpinionOfHero_IsWillingToStartFollowingHero_006818a0_test.cpp'
        PassPattern = 'CTCCreatureOpinionOfHero_006818a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412130'
        Module = 'CCombatWheel'
        Source = 'CCombatWheel_ResetRings_00412130.cpp'
        TestSource = 'CCombatWheel_ResetRings_00412130_test.cpp'
        PassPattern = 'CCombatWheel_00412130_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00414ea0'
        Module = 'CCombatWheel'
        Source = 'CCombatWheel_ResetRings_00414ea0.cpp'
        TestSource = 'CCombatWheel_ResetRings_00414ea0_test.cpp'
        PassPattern = 'CCombatWheel_00414ea0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00414ef0'
        Module = 'CCombatWheel'
        Source = 'CCombatWheel_ResetRings_00414ef0.cpp'
        TestSource = 'CCombatWheel_ResetRings_00414ef0_test.cpp'
        PassPattern = 'CCombatWheel_00414ef0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00433040'
        Module = 'CCombatWheel'
        Source = 'CCombatWheel_ResetRings_00433040.cpp'
        TestSource = 'CCombatWheel_ResetRings_00433040_test.cpp'
        PassPattern = 'CCombatWheel_00433040_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00723a10'
        Module = 'CTCPhysicsStandard'
        Source = 'CTCPhysicsStandard_SetRHSetAsFlushOnGround_00723a10.cpp'
        TestSource = 'CTCPhysicsStandard_SetRHSetAsFlushOnGround_00723a10_test.cpp'
        PassPattern = 'CTCPhysicsStandard_00723a10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004bf9a0'
        Module = 'CTCGraphicAppearance'
        Source = 'CTCGraphicAppearance_ClearAllPrimitives_004bf9a0.cpp'
        TestSource = 'CTCGraphicAppearance_ClearAllPrimitives_004bf9a0_test.cpp'
        PassPattern = 'CTCGraphicAppearance_004bf9a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ce6a'
        Module = 'CManager'
        Source = 'CManager_ClearErrorMessage_0041ce6a.cpp'
        TestSource = 'CManager_ClearErrorMessage_0041ce6a_test.cpp'
        PassPattern = 'CManager_0041ce6a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008d0110'
        Module = 'CGameScriptThing'
        Source = 'CGameScriptThing_IsNullCGameScriptThingUBENXZ_008d0110.cpp'
        TestSource = 'CGameScriptThing_IsNullCGameScriptThingUBENXZ_008d0110_test.cpp'
        PassPattern = 'CGameScriptThing_008d0110_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00744830'
        Module = 'CTCPerceiveThing'
        Source = 'CTCPerceiveThing_IsPlayerExtendedLineOfSight_00744830.cpp'
        TestSource = 'CTCPerceiveThing_IsPlayerExtendedLineOfSight_00744830_test.cpp'
        PassPattern = 'CTCPerceiveThing_00744830_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00744870'
        Module = 'CTCPerceiveThing'
        Source = 'CTCPerceiveThing_IsPlayerExtendedLineOfSight_00744870.cpp'
        TestSource = 'CTCPerceiveThing_IsPlayerExtendedLineOfSight_00744870_test.cpp'
        PassPattern = 'CTCPerceiveThing_00744870_TEST PASS'
    }
    [pscustomobject]@{
        Address = '007333d0'
        Module = 'CTCCombat'
        Source = 'CTCCombat_IsCurrentActionBlockable_007333d0.cpp'
        TestSource = 'CTCCombat_IsCurrentActionBlockable_007333d0_test.cpp'
        PassPattern = 'CTCCombat_007333d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006bc1d0'
        Module = 'CRegion'
        Source = 'CRegion_SetRegionDefName_006bc1d0.cpp'
        TestSource = 'CRegion_SetRegionDefName_006bc1d0_test.cpp'
        PassPattern = 'CRegion_006bc1d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004bf960'
        Module = 'CTCGraphicAppearance'
        Source = 'CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRan_004bf960.cpp'
        TestSource = 'CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRan_004bf960_test.cpp'
        PassPattern = 'CTCGraphicAppearance_004bf960_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009be8f0'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_IsValidTextureFormat_009be8f0.cpp'
        TestSource = 'CDisplayManager_IsValidTextureFormat_009be8f0_test.cpp'
        PassPattern = 'CDisplayManager_009be8f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00af3f30'
        Module = 'CParticleSystem'
        Source = 'CParticleSystem_GetNoActiveDecals_00af3f30.cpp'
        TestSource = 'CParticleSystem_GetNoActiveDecals_00af3f30_test.cpp'
        PassPattern = 'CParticleSystem_00af3f30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00af3f70'
        Module = 'CParticleSystem'
        Source = 'CParticleSystem_GetNoActiveDecals_00af3f70.cpp'
        TestSource = 'CParticleSystem_GetNoActiveDecals_00af3f70_test.cpp'
        PassPattern = 'CParticleSystem_00af3f70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00aed050'
        Module = 'CParticleEmitter'
        Source = 'CParticleEmitter_GetApproximateTotalSplineLength_00aed050.cpp'
        TestSource = 'CParticleEmitter_GetApproximateTotalSplineLength_00aed050_test.cpp'
        PassPattern = 'CParticleEmitter_00aed050_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00438570'
        Module = '_global'
        Source = 'global_PeekGuiDef_00438570.cpp'
        TestSource = 'global_PeekGuiDef_00438570_test.cpp'
        PassPattern = '_global_00438570_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009d3100'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_ClearTextures_009d3100.cpp'
        TestSource = 'CLandscapeBackgroundPatch_ClearTextures_009d3100_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_009d3100_TEST PASS'
    }
    [pscustomobject]@{
        Address = '007236e0'
        Module = 'CTCPhysicsStandard'
        Source = 'CTCPhysicsStandard_ResetInterpolation_007236e0.cpp'
        TestSource = 'CTCPhysicsStandard_ResetInterpolation_007236e0_test.cpp'
        PassPattern = 'CTCPhysicsStandard_007236e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00744580'
        Module = 'CTCPerceiveThing'
        Source = 'CTCPerceiveThing_GetExtendedSightRadius_00744580.cpp'
        TestSource = 'CTCPerceiveThing_GetExtendedSightRadius_00744580_test.cpp'
        PassPattern = 'CTCPerceiveThing_00744580_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0052e6b0'
        Module = 'CComponent'
        Source = 'CComponent_ResetState_0052e6b0.cpp'
        TestSource = 'CComponent_ResetState_0052e6b0_test.cpp'
        PassPattern = 'CComponent_0052e6b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00658a35'
        Module = 'CTCVillage'
        Source = 'CTCVillage_IsCookTime_00658a35.cpp'
        TestSource = 'CTCVillage_IsCookTime_00658a35_test.cpp'
        PassPattern = 'CTCVillage_00658a35_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00658a80'
        Module = 'CTCVillage'
        Source = 'CTCVillage_IsCookTime_00658a80.cpp'
        TestSource = 'CTCVillage_IsCookTime_00658a80_test.cpp'
        PassPattern = 'CTCVillage_00658a80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00658b20'
        Module = 'CTCVillage'
        Source = 'CTCVillage_IsCookTime_00658b20.cpp'
        TestSource = 'CTCVillage_IsCookTime_00658b20_test.cpp'
        PassPattern = 'CTCVillage_00658b20_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00adf1e0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_ClearTextures_00adf1e0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_ClearTextures_00adf1e0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00adf1e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00723a60'
        Module = 'CTCPhysicsStandard'
        Source = 'CTCPhysicsStandard_IsOnAnyStationarySurface_00723a60.cpp'
        TestSource = 'CTCPhysicsStandard_IsOnAnyStationarySurface_00723a60_test.cpp'
        PassPattern = 'CTCPhysicsStandard_00723a60_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009be530'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_IsTextureBitDepthSupported_009be530.cpp'
        TestSource = 'CDisplayManager_IsTextureBitDepthSupported_009be530_test.cpp'
        PassPattern = 'CDisplayManager_009be530_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00733100'
        Module = 'CTCCombat'
        Source = 'CTCCombat_IsUsingUnarmedCombatIdle_00733100.cpp'
        TestSource = 'CTCCombat_IsUsingUnarmedCombatIdle_00733100_test.cpp'
        PassPattern = 'CTCCombat_00733100_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00744730'
        Module = 'CTCPerceiveThing'
        Source = 'CTCPerceiveThing_SetLastAwareInfo_00744730.cpp'
        TestSource = 'CTCPerceiveThing_SetLastAwareInfo_00744730_test.cpp'
        PassPattern = 'CTCPerceiveThing_00744730_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004a9ef0'
        Module = 'CThingSearchTools'
        Source = 'CThingSearchTools_GetPFirstLeafThingCFilterBind2CThing_004a9ef0.cpp'
        TestSource = 'CThingSearchTools_GetPFirstLeafThingCFilterBind2CThing_004a9ef0_test.cpp'
        PassPattern = 'CThingSearchTools_004a9ef0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004b0540'
        Module = 'CQuestManager'
        Source = 'CQuestManager_GetActiveExclusiveQuestName_004b0540.cpp'
        TestSource = 'CQuestManager_GetActiveExclusiveQuestName_004b0540_test.cpp'
        PassPattern = 'CQuestManager_004b0540_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c6a5d0'
        Module = 'CEditControlCentre'
        Source = 'CEditControlCentre_GetCurrentMask_00c6a5d0.cpp'
        TestSource = 'CEditControlCentre_GetCurrentMask_00c6a5d0_test.cpp'
        PassPattern = 'CEditControlCentre_00c6a5d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00988290'
        Module = 'CShaderRenderManager'
        Source = 'CShaderRenderManager_ResetWorldTransform_00988290.cpp'
        TestSource = 'CShaderRenderManager_ResetWorldTransform_00988290_test.cpp'
        PassPattern = 'CShaderRenderManager_00988290_TEST PASS'
    }
    [pscustomobject]@{
        Address = '009ad950'
        Module = 'CDefinitionManager'
        Source = 'CDefinitionManager_ClearInstantiatedDefs_009ad950.cpp'
        TestSource = 'CDefinitionManager_ClearInstantiatedDefs_009ad950_test.cpp'
        PassPattern = 'CDefinitionManager_009ad950_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00988350'
        Module = 'CShaderRenderManager'
        Source = 'CShaderRenderManager_SetViewTransform_00988350.cpp'
        TestSource = 'CShaderRenderManager_SetViewTransform_00988350_test.cpp'
        PassPattern = 'CShaderRenderManager_00988350_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00699090'
        Module = 'CGameCameraManager'
        Source = 'CGameCameraManager_SetAutomaticCameraControl_00699090.cpp'
        TestSource = 'CGameCameraManager_SetAutomaticCameraControl_00699090_test.cpp'
        PassPattern = 'CGameCameraManager_00699090_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088e210'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_DisplayTime_0088e210.cpp'
        TestSource = 'CGameScriptInterface_DisplayTime_0088e210_test.cpp'
        PassPattern = 'CGameScriptInterface_0088e210_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fa90'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetPlayerUsingMeleeDummies_0088fa90.cpp'
        TestSource = 'CGameScriptInterface_GetPlayerUsingMeleeDummies_0088fa90_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fa90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fab0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetPlayerUsingRangedDummies_0088fab0.cpp'
        TestSource = 'CGameScriptInterface_GetPlayerUsingRangedDummies_0088fab0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fab0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fad0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetPlayerUsingWillDummies_0088fad0.cpp'
        TestSource = 'CGameScriptInterface_GetPlayerUsingWillDummies_0088fad0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fad0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088faf0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetCheapHeadLooking_0088faf0.cpp'
        TestSource = 'CGameScriptInterface_GetCheapHeadLooking_0088faf0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088faf0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fb10'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetQuitTavernGame_0088fb10.cpp'
        TestSource = 'CGameScriptInterface_GetQuitTavernGame_0088fb10_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fb10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fb30'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetPrizeTavernTable_0088fb30.cpp'
        TestSource = 'CGameScriptInterface_GetPrizeTavernTable_0088fb30_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fb30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fb50'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetBettingActive_0088fb50.cpp'
        TestSource = 'CGameScriptInterface_GetBettingActive_0088fb50_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fb50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fb70'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetBettingAccept_0088fb70.cpp'
        TestSource = 'CGameScriptInterface_GetBettingAccept_0088fb70_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fb70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fbb0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetCountBetMoneyDown_0088fbb0.cpp'
        TestSource = 'CGameScriptInterface_GetCountBetMoneyDown_0088fbb0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fbb0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fbd0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetSpotTheAdditionBeaten_0088fbd0.cpp'
        TestSource = 'CGameScriptInterface_GetSpotTheAdditionBeaten_0088fbd0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fbd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fc30'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetBoastingEnabled_0088fc30.cpp'
        TestSource = 'CGameScriptInterface_GetBoastingEnabled_0088fc30_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fc30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00893040'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetActiveGossipCategoriesSize_00893040.cpp'
        TestSource = 'CGameScriptInterface_GetActiveGossipCategoriesSize_00893040_test.cpp'
        PassPattern = 'CGameScriptInterface_00893040_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00893050'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetIsGossipForPlayerSize_00893050.cpp'
        TestSource = 'CGameScriptInterface_GetIsGossipForPlayerSize_00893050_test.cpp'
        PassPattern = 'CGameScriptInterface_00893050_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fbf0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetGlobalTargetingDistanceOffset_0088fbf0.cpp'
        TestSource = 'CGameScriptInterface_GetGlobalTargetingDistanceOffset_0088fbf0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fbf0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fc10'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetTradingPriceMult_0088fc10.cpp'
        TestSource = 'CGameScriptInterface_GetTradingPriceMult_0088fc10_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fc10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006e7290'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_EndLetterBox_006e7290.cpp'
        TestSource = 'CGameScriptInterface_EndLetterBox_006e7290_test.cpp'
        PassPattern = 'CGameScriptInterface_006e7290_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006e7d40'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ResetCreatureCreationDelayFrames_006e7d40.cpp'
        TestSource = 'CGameScriptInterface_ResetCreatureCreationDelayFrames_006e7d40_test.cpp'
        PassPattern = 'CGameScriptInterface_006e7d40_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088e400'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_MiniMapSetAsEnabled_0088e400.cpp'
        TestSource = 'CGameScriptInterface_MiniMapSetAsEnabled_0088e400_test.cpp'
        PassPattern = 'CGameScriptInterface_0088e400_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f420'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsTeleportingActive_0088f420.cpp'
        TestSource = 'CGameScriptInterface_IsTeleportingActive_0088f420_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f420_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f450'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsHeroSleepingEnabled_0088f450.cpp'
        TestSource = 'CGameScriptInterface_IsHeroSleepingEnabled_0088f450_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f450_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f900'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetCountdownTimer_0088f900.cpp'
        TestSource = 'CGameScriptInterface_GetCountdownTimer_0088f900_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f900_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f950'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsSavingEnabled_0088f950.cpp'
        TestSource = 'CGameScriptInterface_IsSavingEnabled_0088f950_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f950_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f9e0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetGuildSealRecallAngleXY_0088f9e0.cpp'
        TestSource = 'CGameScriptInterface_GetGuildSealRecallAngleXY_0088f9e0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f9e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891320'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_EnableDangerMusic_00891320.cpp'
        TestSource = 'CGameScriptInterface_EnableDangerMusic_00891320_test.cpp'
        PassPattern = 'CGameScriptInterface_00891320_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891510'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuil_00891510.cpp'
        TestSource = 'CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuil_00891510_test.cpp'
        PassPattern = 'CGameScriptInterface_00891510_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891520'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_FailAllActiveQuests_00891520.cpp'
        TestSource = 'CGameScriptInterface_FailAllActiveQuests_00891520_test.cpp'
        PassPattern = 'CGameScriptInterface_00891520_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891870'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsHeroOnQuest_00891870.cpp'
        TestSource = 'CGameScriptInterface_IsHeroOnQuest_00891870_test.cpp'
        PassPattern = 'CGameScriptInterface_00891870_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00892270'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsInCutscene_00892270.cpp'
        TestSource = 'CGameScriptInterface_IsInCutscene_00892270_test.cpp'
        PassPattern = 'CGameScriptInterface_00892270_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088e4c0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_FadeScreenIn_0088e4c0.cpp'
        TestSource = 'CGameScriptInterface_FadeScreenIn_0088e4c0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088e4c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00890170'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsRadialBlurFadeActive_00890170.cpp'
        TestSource = 'CGameScriptInterface_IsRadialBlurFadeActive_00890170_test.cpp'
        PassPattern = 'CGameScriptInterface_00890170_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891c30'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_RemoveAllQuestInfoElements_00891c30.cpp'
        TestSource = 'CGameScriptInterface_RemoveAllQuestInfoElements_00891c30_test.cpp'
        PassPattern = 'CGameScriptInterface_00891c30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088e500'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_FixMovieSequenceCamera_0088e500.cpp'
        TestSource = 'CGameScriptInterface_FixMovieSequenceCamera_0088e500_test.cpp'
        PassPattern = 'CGameScriptInterface_0088e500_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00890150'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_RadialBlurFadeOut_00890150.cpp'
        TestSource = 'CGameScriptInterface_RadialBlurFadeOut_00890150_test.cpp'
        PassPattern = 'CGameScriptInterface_00890150_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00890180'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_CancelRadialBlurFade_00890180.cpp'
        TestSource = 'CGameScriptInterface_CancelRadialBlurFade_00890180_test.cpp'
        PassPattern = 'CGameScriptInterface_00890180_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008901e0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_DisplacementMonochromeEffectColourFa_008901e0.cpp'
        TestSource = 'CGameScriptInterface_DisplacementMonochromeEffectColourFa_008901e0_test.cpp'
        PassPattern = 'CGameScriptInterface_008901e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00892b30'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsSafeToDisplayGameInfo_00892b30.cpp'
        TestSource = 'CGameScriptInterface_IsSafeToDisplayGameInfo_00892b30_test.cpp'
        PassPattern = 'CGameScriptInterface_00892b30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004aacc0'
        Module = 'CScriptThing'
        Source = 'CScriptThing_MsgPerformedAnySpecialAbility_004aacc0.cpp'
        TestSource = 'CScriptThing_MsgPerformedAnySpecialAbility_004aacc0_test.cpp'
        PassPattern = 'CScriptThing_004aacc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004aace0'
        Module = 'CScriptThing'
        Source = 'CScriptThing_MsgPerformedAnyAggressiveSpecialAbil_004aace0.cpp'
        TestSource = 'CScriptThing_MsgPerformedAnyAggressiveSpecialAbil_004aace0_test.cpp'
        PassPattern = 'CScriptThing_004aace0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004aad00'
        Module = 'CScriptThing'
        Source = 'CScriptThing_MsgPerformedAnyNonAggressiveSpecialA_004aad00.cpp'
        TestSource = 'CScriptThing_MsgPerformedAnyNonAggressiveSpecialA_004aad00_test.cpp'
        PassPattern = 'CScriptThing_004aad00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004aada0'
        Module = 'CScriptThing'
        Source = 'CScriptThing_MsgOpenedChest_004aada0.cpp'
        TestSource = 'CScriptThing_MsgOpenedChest_004aada0_test.cpp'
        PassPattern = 'CScriptThing_004aada0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004aaf80'
        Module = 'CScriptThing'
        Source = 'CScriptThing_MsgIsKicked_004aaf80.cpp'
        TestSource = 'CScriptThing_MsgIsKicked_004aaf80_test.cpp'
        PassPattern = 'CScriptThing_004aaf80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004ab040'
        Module = 'CScriptThing'
        Source = 'CScriptThing_SetAsUsable_004ab040.cpp'
        TestSource = 'CScriptThing_SetAsUsable_004ab040_test.cpp'
        PassPattern = 'CScriptThing_004ab040_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00892030'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_MiniMapRemoveAllMarkers_00892030.cpp'
        TestSource = 'CGameScriptInterface_MiniMapRemoveAllMarkers_00892030_test.cpp'
        PassPattern = 'CGameScriptInterface_00892030_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00891c40'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_DisplayMoneyBag_00891c40.cpp'
        TestSource = 'CGameScriptInterface_DisplayMoneyBag_00891c40_test.cpp'
        PassPattern = 'CGameScriptInterface_00891c40_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088ff10'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ResetToDefaultTheme_0088ff10.cpp'
        TestSource = 'CGameScriptInterface_ResetToDefaultTheme_0088ff10_test.cpp'
        PassPattern = 'CGameScriptInterface_0088ff10_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f3e0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsTutorialSystemEnabled_0088f3e0.cpp'
        TestSource = 'CGameScriptInterface_IsTutorialSystemEnabled_0088f3e0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f3e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088fff0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ResetToDefaultThemeExternals_0088fff0.cpp'
        TestSource = 'CGameScriptInterface_ResetToDefaultThemeExternals_0088fff0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088fff0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088ecc0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsCameraInScriptedMode_0088ecc0.cpp'
        TestSource = 'CGameScriptInterface_IsCameraInScriptedMode_0088ecc0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088ecc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008368f0'
        Module = '_global'
        Source = 'global_ConditionalVirtualDispatchOnLevelUnl_008368f0.cpp'
        TestSource = 'global_ConditionalVirtualDispatchOnLevelUnl_008368f0_test.cpp'
        PassPattern = '_global_008368f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088e4d0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsScreenFadingOut_0088e4d0.cpp'
        TestSource = 'CGameScriptInterface_IsScreenFadingOut_0088e4d0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088e4d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088ec90'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_CameraSetCameraPreloadFlag_0088ec90.cpp'
        TestSource = 'CGameScriptInterface_CameraSetCameraPreloadFlag_0088ec90_test.cpp'
        PassPattern = 'CGameScriptInterface_0088ec90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f4a0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_EnableDecals_0088f4a0.cpp'
        TestSource = 'CGameScriptInterface_EnableDecals_0088f4a0_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f4a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00890fc0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_CameraResetToViewBehindHero_00890fc0.cpp'
        TestSource = 'CGameScriptInterface_CameraResetToViewBehindHero_00890fc0_test.cpp'
        PassPattern = 'CGameScriptInterface_00890fc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006e7340'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetGameAngleXY_006e7340.cpp'
        TestSource = 'CGameScriptInterface_GetGameAngleXY_006e7340_test.cpp'
        PassPattern = 'CGameScriptInterface_006e7340_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008902b0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsHeroChild_008902b0.cpp'
        TestSource = 'CGameScriptInterface_IsHeroChild_008902b0_test.cpp'
        PassPattern = 'CGameScriptInterface_008902b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008913c0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsHeroInTavernGame_008913c0.cpp'
        TestSource = 'CGameScriptInterface_IsHeroInTavernGame_008913c0_test.cpp'
        PassPattern = 'CGameScriptInterface_008913c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00890370'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetHeroHealth_00890370.cpp'
        TestSource = 'CGameScriptInterface_GetHeroHealth_00890370_test.cpp'
        PassPattern = 'CGameScriptInterface_00890370_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008903a0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetHeroHealthMax_008903a0.cpp'
        TestSource = 'CGameScriptInterface_GetHeroHealthMax_008903a0_test.cpp'
        PassPattern = 'CGameScriptInterface_008903a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008903d0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GetHeroHealthPercentage_008903d0.cpp'
        TestSource = 'CGameScriptInterface_GetHeroHealthPercentage_008903d0_test.cpp'
        PassPattern = 'CGameScriptInterface_008903d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c3b900'
        Module = 'CLandscapeLayerMesh'
        Source = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00c3b900.cpp'
        TestSource = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00c3b900_test.cpp'
        PassPattern = 'CLandscapeLayerMesh_00c3b900_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0088f620'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_EnableSounds_0088f620.cpp'
        TestSource = 'CGameScriptInterface_EnableSounds_0088f620_test.cpp'
        PassPattern = 'CGameScriptInterface_0088f620_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00713140'
        Module = 'CActionDoCreatureAction'
        Source = 'CActionDoCreatureAction_CActionDoCreatureAction_00713140.cpp'
        TestSource = 'CActionDoCreatureAction_CActionDoCreatureAction_00713140_test.cpp'
        PassPattern = 'CActionDoCreatureAction_00713140_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008942c0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_MsgOnChestOpeningCancelled_008942c0.cpp'
        TestSource = 'CGameScriptInterface_MsgOnChestOpeningCancelled_008942c0_test.cpp'
        PassPattern = 'CGameScriptInterface_008942c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008a78e0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ClearActiveGossipCategories_008a78e0.cpp'
        TestSource = 'CGameScriptInterface_ClearActiveGossipCategories_008a78e0_test.cpp'
        PassPattern = 'CGameScriptInterface_008a78e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008a7930'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ClearIsGossipForPlayer_008a7930.cpp'
        TestSource = 'CGameScriptInterface_ClearIsGossipForPlayer_008a7930_test.cpp'
        PassPattern = 'CGameScriptInterface_008a7930_TEST PASS'
    }
    [pscustomobject]@{
        Address = '006b0730'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_CTCPhysicsBase_006b0730.cpp'
        TestSource = 'CTCPhysicsBase_CTCPhysicsBase_006b0730_test.cpp'
        PassPattern = 'CTCPhysicsBase_006b0730_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00c43f80'
        Module = '_global'
        Source = 'global_WeaponProcessAnimFrameIfReady_00c43f80.cpp'
        TestSource = 'global_WeaponProcessAnimFrameIfReady_00c43f80_test.cpp'
        PassPattern = '_global_00c43f80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00bfddd0'
        Module = 'CLandscapeLayerMesh'
        Source = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00bfddd0.cpp'
        TestSource = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00bfddd0_test.cpp'
        PassPattern = 'CLandscapeLayerMesh_00bfddd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00894370'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_MsgIsGameInfoClickedPast_00894370.cpp'
        TestSource = 'CGameScriptInterface_MsgIsGameInfoClickedPast_00894370_test.cpp'
        PassPattern = 'CGameScriptInterface_00894370_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00896fd0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefaul_00896fd0.cpp'
        TestSource = 'CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefaul_00896fd0_test.cpp'
        PassPattern = 'CGameScriptInterface_00896fd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008970f0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ResetWhetherToFailQuestOnDeathToDefa_008970f0.cpp'
        TestSource = 'CGameScriptInterface_ResetWhetherToFailQuestOnDeathToDefa_008970f0_test.cpp'
        PassPattern = 'CGameScriptInterface_008970f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '008987e0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_ConfiscateAllHeroWeapons_008987e0.cpp'
        TestSource = 'CGameScriptInterface_ConfiscateAllHeroWeapons_008987e0_test.cpp'
        PassPattern = 'CGameScriptInterface_008987e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0089b390'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_RemoveHeroClothing_0089b390.cpp'
        TestSource = 'CGameScriptInterface_RemoveHeroClothing_0089b390_test.cpp'
        PassPattern = 'CGameScriptInterface_0089b390_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00bfd420'
        Module = 'CLandscapeLayerMesh'
        Source = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00bfd420.cpp'
        TestSource = 'CLandscapeLayerMesh_CLandscapeLayerMesh_00bfd420_test.cpp'
        PassPattern = 'CLandscapeLayerMesh_00bfd420_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0089b330'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsHeroNaked_0089b330.cpp'
        TestSource = 'CGameScriptInterface_IsHeroNaked_0089b330_test.cpp'
        PassPattern = 'CGameScriptInterface_0089b330_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00899ba0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_IsPlayerCreatureBlocking_00899ba0.cpp'
        TestSource = 'CGameScriptInterface_IsPlayerCreatureBlocking_00899ba0_test.cpp'
        PassPattern = 'CGameScriptInterface_00899ba0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401650'
        Module = 'CAFile'
        Source = 'CAFile_IsIOFinished_00401650.cpp'
        TestSource = 'CAFile_IsIOFinished_00401650_test.cpp'
        PassPattern = 'CAFile_00401650_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00407030'
        Module = 'CThingFilter_IsUnconscious'
        Source = 'CThingFilterIsUnconscious_operator_00407030.cpp'
        TestSource = 'CThingFilterIsUnconscious_operator_00407030_test.cpp'
        PassPattern = 'CThingFilter_IsUnconscious_00407030_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040e160'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_AddChildPrimitive_0040e160.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_AddChildPrimitive_0040e160_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_0040e160_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401670'
        Module = 'CDiskFileWin32'
        Source = 'CDiskFileWin32_GetLength_00401670.cpp'
        TestSource = 'CDiskFileWin32_GetLength_00401670_test.cpp'
        PassPattern = 'CDiskFileWin32_00401670_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401680'
        Module = 'CDiskFileWin32'
        Source = 'CDiskFileWin32_IsOpen_00401680.cpp'
        TestSource = 'CDiskFileWin32_IsOpen_00401680_test.cpp'
        PassPattern = 'CDiskFileWin32_00401680_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412050'
        Module = 'CMemoryFile'
        Source = 'CMemoryFile_GetPosition_00412050.cpp'
        TestSource = 'CMemoryFile_GetPosition_00412050_test.cpp'
        PassPattern = 'CMemoryFile_00412050_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412060'
        Module = 'CMemoryFile'
        Source = 'CMemoryFile_GetLength_00412060.cpp'
        TestSource = 'CMemoryFile_GetLength_00412060_test.cpp'
        PassPattern = 'CMemoryFile_00412060_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401660'
        Module = 'CAFile'
        Source = 'CAFile_GetAlignment_00401660.cpp'
        TestSource = 'CAFile_GetAlignment_00401660_test.cpp'
        PassPattern = 'CAFile_00401660_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00404a80'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_AddChildPrimitive_00404a80.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_AddChildPrimitive_00404a80_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_00404a80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004067b0'
        Module = 'CEngineInternalPrimitiveBase'
        Source = 'CEngineInternalPrimitiveBase_AddChildPrimitive_004067b0.cpp'
        TestSource = 'CEngineInternalPrimitiveBase_AddChildPrimitive_004067b0_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveBase_004067b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00406880'
        Module = 'CModeBlock'
        Source = 'CModeBlock_GetAnimFilter_00406880.cpp'
        TestSource = 'CModeBlock_GetAnimFilter_00406880_test.cpp'
        PassPattern = 'CModeBlock_00406880_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00403a00'
        Module = 'CCharString'
        Source = 'CCharString_GetLength_00403a00.cpp'
        TestSource = 'CCharString_GetLength_00403a00_test.cpp'
        PassPattern = 'CCharString_00403a00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004067f0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_OnPreDeviceReset_004067f0.cpp'
        TestSource = 'CUserProfileManager_OnPreDeviceReset_004067f0_test.cpp'
        PassPattern = 'CUserProfileManager_004067f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f220'
        Module = 'CSmallVector<CANavQuadTreeNode_const*'
        Source = 'CSmallVectorCANavQuadTreeNodeconst_operator_0040f220.cpp'
        TestSource = 'CSmallVectorCANavQuadTreeNodeconst_operator_0040f220_test.cpp'
        PassPattern = 'CSmallVectorNav_0040f220_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00404ba0'
        Module = 'CNetworkClient'
        Source = 'CNetworkClient_GetGameEventPackageSet_00404ba0.cpp'
        TestSource = 'CNetworkClient_GetGameEventPackageSet_00404ba0_test.cpp'
        PassPattern = 'CNetworkClient_00404ba0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401490'
        Module = 'CEngineMap'
        Source = 'CEngineMap_PeekWaterHasType_00401490.cpp'
        TestSource = 'CEngineMap_PeekWaterHasType_00401490_test.cpp'
        PassPattern = 'CEngineMap_00401490_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00407360'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCategoryWithIndex_00407360.cpp'
        TestSource = 'CTCInventoryBase_GetCategoryWithIndex_00407360_test.cpp'
        PassPattern = 'CTCInventoryBase_00407360_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004075b0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_CaptureBackBufferForUIBackgroundIfNo_004075b0.cpp'
        TestSource = 'CUserProfileManager_CaptureBackBufferForUIBackgroundIfNo_004075b0_test.cpp'
        PassPattern = 'CUserProfileManager_004075b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004012bc'
        Module = '_global'
        Source = 'global_atexit_004012bc.cpp'
        TestSource = 'global_atexit_004012bc_test.cpp'
        PassPattern = '_global_004012bc_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00408340'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetSoundAsMuted_00408340.cpp'
        TestSource = 'CUserProfileManager_SetSoundAsMuted_00408340_test.cpp'
        PassPattern = 'CUserProfileManager_00408340_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040dce0'
        Module = '_global'
        Source = 'global_ISTHINGALIVECThingBuilding_0040dce0.cpp'
        TestSource = 'global_ISTHINGALIVECThingBuilding_0040dce0_test.cpp'
        PassPattern = '_global_0040dce0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401690'
        Module = 'CDiskFileWin32'
        Source = 'CDiskFileWin32_GetPathnameCDiskFileWin32UBEAVCWideS_00401690.cpp'
        TestSource = 'CDiskFileWin32_GetPathnameCDiskFileWin32UBEAVCWideS_00401690_test.cpp'
        PassPattern = 'CDiskFileWin32_00401690_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412070'
        Module = 'CMemoryFile'
        Source = 'CMemoryFile_GetPathnameCMemoryFileUBEAVCWideStri_00412070.cpp'
        TestSource = 'CMemoryFile_GetPathnameCMemoryFileUBEAVCWideStri_00412070_test.cpp'
        PassPattern = 'CMemoryFile_00412070_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00404b80'
        Module = 'CIME'
        Source = 'CIME_FinaliseString_00404b80.cpp'
        TestSource = 'CIME_FinaliseString_00404b80_test.cpp'
        PassPattern = 'CIME_00404b80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004088e0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_GetAssignedInputsPassive_004088e0.cpp'
        TestSource = 'CUserProfileManager_GetAssignedInputsPassive_004088e0_test.cpp'
        PassPattern = 'CUserProfileManager_004088e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040bc80'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_Apply_0040bc80.cpp'
        TestSource = 'CUserProfileManager_Apply_0040bc80_test.cpp'
        PassPattern = 'CUserProfileManager_0040bc80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412880'
        Module = 'vector<EWeaponClass'
        Source = 'vectorEWeaponClass_UmoveEWeaponClass_00412880.cpp'
        TestSource = 'vectorEWeaponClass_UmoveEWeaponClass_00412880_test.cpp'
        PassPattern = 'vector_00412880_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00403e20'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00403e20.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00403e20_test.cpp'
        PassPattern = 'DestVal_00403e20_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040d8b0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040d8b0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040d8b0_test.cpp'
        PassPattern = 'Dest_val_0040d8b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f270'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040f270.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040f270_test.cpp'
        PassPattern = '_Dest_val_0040f270_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040fbd0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040fbd0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0040fbd0_test.cpp'
        PassPattern = 'Dest_val_0040fbd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401630'
        Module = 'CAIStateGroup_CreatureUseCoverBase'
        Source = 'CAIStateGroupCreatureUseCoverBase_CAIStateGroupCreatureUseCoverBase_00401630.cpp'
        TestSource = 'CAIStateGroupCreatureUseCoverBase_CAIStateGroupCreatureUseCoverBase_00401630_test.cpp'
        PassPattern = 'CAIStateGroup_CreatureUseCoverBase_00401630_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004107c0'
        Module = 'CEngineSceneGrid'
        Source = 'CEngineSceneGrid_GetCellBoundingBox_004107c0.cpp'
        TestSource = 'CEngineSceneGrid_GetCellBoundingBox_004107c0_test.cpp'
        PassPattern = 'CEngineSceneGrid_004107c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040ea50'
        Module = '_global'
        Source = 'global_fillENavigatorTypeENavigatorType_0040ea50.cpp'
        TestSource = 'global_fillENavigatorTypeENavigatorType_0040ea50_test.cpp'
        PassPattern = '_global_0040ea50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00410800'
        Module = 'CPersistContext'
        Source = 'CPersistContext_Transferlong_00410800.cpp'
        TestSource = 'CPersistContext_Transferlong_00410800_test.cpp'
        PassPattern = 'CPersistContext_00410800_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004047a0'
        Module = '_Dest_val<std'
        Source = 'Destvalstd_allocatorCMacroCMacro_004047a0.cpp'
        TestSource = 'Destvalstd_allocatorCMacroCMacro_004047a0_test.cpp'
        PassPattern = '_Dest_val_004047a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00404b50'
        Module = 'CIME'
        Source = 'CIME_FinaliseString_00404b50.cpp'
        TestSource = 'CIME_FinaliseString_00404b50_test.cpp'
        PassPattern = 'CIME_00404b50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040bc50'
        Module = 'CEngine'
        Source = 'CEngine_GetEditorAnimation_0040bc50.cpp'
        TestSource = 'CEngine_GetEditorAnimation_0040bc50_test.cpp'
        PassPattern = 'CEngine_0040bc50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004067c0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_IsEnoughSpaceForProfile_004067c0.cpp'
        TestSource = 'CUserProfileManager_IsEnoughSpaceForProfile_004067c0_test.cpp'
        PassPattern = 'CUserProfileManager_004067c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040e9a0'
        Module = '_global'
        Source = 'global_FillCHeroMarriageInfoCHeroMarriageIn_0040e9a0.cpp'
        TestSource = 'global_FillCHeroMarriageInfoCHeroMarriageIn_0040e9a0_test.cpp'
        PassPattern = '_global_0040e9a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00411570'
        Module = '_global'
        Source = 'global_global_00411570.cpp'
        TestSource = 'global_global_00411570_test.cpp'
        PassPattern = '_global_00411570_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f5b0'
        Module = '_global'
        Source = 'global_Fillnltunsignedlongunsignedintunsign_0040f5b0.cpp'
        TestSource = 'global_Fillnltunsignedlongunsignedintunsign_0040f5b0_test.cpp'
        PassPattern = '_global_0040f5b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f470'
        Module = 'CBankFileAsyncData'
        Source = 'CBankFileAsyncData_FreeBuffer_0040f470.cpp'
        TestSource = 'CBankFileAsyncData_FreeBuffer_0040f470_test.cpp'
        PassPattern = 'CBankFileAsyncData_0040f470_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00412020'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00412020.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00412020_test.cpp'
        PassPattern = 'CTCInventoryItem_00412020_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401780'
        Module = '_global'
        Source = 'global_GFClearBadExitFlag_00401780.cpp'
        TestSource = 'global_GFClearBadExitFlag_00401780_test.cpp'
        PassPattern = '_global_00401780_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040ded0'
        Module = '_global'
        Source = 'global_CopyimplCActionInputControlCActionIn_0040ded0.cpp'
        TestSource = 'global_CopyimplCActionInputControlCActionIn_0040ded0_test.cpp'
        PassPattern = '_global_0040ded0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040af90'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040af90.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040af90_test.cpp'
        PassPattern = 'CUserProfileManager_0040af90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040afd0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040afd0.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040afd0_test.cpp'
        PassPattern = 'CUserProfileManager_0040afd0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b010'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b010.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b010_test.cpp'
        PassPattern = 'CUserProfileManager_0040b010_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b050'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b050.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b050_test.cpp'
        PassPattern = 'CUserProfileManager_0040b050_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b090'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b090.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b090_test.cpp'
        PassPattern = 'CUserProfileManager_0040b090_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b0d0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b0d0.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b0d0_test.cpp'
        PassPattern = 'CUserProfileManager_0040b0d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b110'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b110.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b110_test.cpp'
        PassPattern = 'CUserProfileManager_0040b110_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b150'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b150.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b150_test.cpp'
        PassPattern = 'CUserProfileManager_0040b150_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b190'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b190.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b190_test.cpp'
        PassPattern = 'CUserProfileManager_0040b190_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b1d0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b1d0.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b1d0_test.cpp'
        PassPattern = 'CUserProfileManager_0040b1d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b210'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b210.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b210_test.cpp'
        PassPattern = 'CUserProfileManager_0040b210_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b250'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b250.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b250_test.cpp'
        PassPattern = 'CUserProfileManager_0040b250_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b290'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b290.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b290_test.cpp'
        PassPattern = 'CUserProfileManager_0040b290_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b410'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b410.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b410_test.cpp'
        PassPattern = 'CUserProfileManager_0040b410_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b450'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b450.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b450_test.cpp'
        PassPattern = 'CUserProfileManager_0040b450_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b490'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b490.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b490_test.cpp'
        PassPattern = 'CUserProfileManager_0040b490_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b4d0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetAntialiasing_0040b4d0.cpp'
        TestSource = 'CUserProfileManager_SetAntialiasing_0040b4d0_test.cpp'
        PassPattern = 'CUserProfileManager_0040b4d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b510'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b510.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b510_test.cpp'
        PassPattern = 'CUserProfileManager_0040b510_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b6d0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetAntialiasing_0040b6d0.cpp'
        TestSource = 'CUserProfileManager_SetAntialiasing_0040b6d0_test.cpp'
        PassPattern = 'CUserProfileManager_0040b6d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b710'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetAntialiasing_0040b710.cpp'
        TestSource = 'CUserProfileManager_SetAntialiasing_0040b710_test.cpp'
        PassPattern = 'CUserProfileManager_0040b710_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b750'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetCameraResetting_0040b750.cpp'
        TestSource = 'CUserProfileManager_SetCameraResetting_0040b750_test.cpp'
        PassPattern = 'CUserProfileManager_0040b750_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004016b0'
        Module = 'CThingTrackNode'
        Source = 'CThingTrackNode_vectordeletingdestructor_004016b0.cpp'
        TestSource = 'CThingTrackNode_vectordeletingdestructor_004016b0_test.cpp'
        PassPattern = 'CThingTrackNode_004016b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b310'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetAntialiasing_0040b310.cpp'
        TestSource = 'CUserProfileManager_SetAntialiasing_0040b310_test.cpp'
        PassPattern = 'CUserProfileManager_0040b310_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040b3d0'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_SetAntialiasing_0040b3d0.cpp'
        TestSource = 'CUserProfileManager_SetAntialiasing_0040b3d0_test.cpp'
        PassPattern = 'CUserProfileManager_0040b3d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00408360'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_ForceClose_00408360.cpp'
        TestSource = 'CPlayerGui_ForceClose_00408360_test.cpp'
        PassPattern = 'CPlayerGui_00408360_TEST PASS'
    }
)

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
$results = @()
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = "$(Join-Path $vcRoot 'include');$(Join-Path $rebuildRoot 'include')"
    $env:LIB = Join-Path $vcRoot 'lib'

    foreach ($entry in $catalog) {
        $source = Join-Path $sourceRoot $entry.Source
        $object = Join-Path $outDir "$($entry.Address).obj"
        $log = Join-Path $outDir "$($entry.Address).compile.log"
        Remove-Item -LiteralPath $object -Force -ErrorAction SilentlyContinue
        $output = & (Join-Path $vcRoot 'bin\cl.exe') /nologo /c /O2 /Oy /W3 /Fo$object $source 2>&1
        $exitCode = $LASTEXITCODE
        $output | Set-Content -LiteralPath $log -Encoding UTF8
        $passed = $exitCode -eq 0 -and (Test-Path -LiteralPath $object)
        $testSource = Join-Path $rebuildRoot "tests\$($entry.TestSource)"
        $testObject = Join-Path $outDir "$($entry.Address).test.obj"
        $testCompileLog = Join-Path $outDir "$($entry.Address).test-compile.log"
        $testExe = Join-Path $outDir "$($entry.Address).test.exe"
        $testRunLog = Join-Path $outDir "$($entry.Address).test-run.log"
        Remove-Item -LiteralPath $testObject -Force -ErrorAction SilentlyContinue
        Remove-Item -LiteralPath $testExe -Force -ErrorAction SilentlyContinue
        $testOutput = & (Join-Path $vcRoot 'bin\cl.exe') /nologo /c /Od /W3 /Fo$testObject $testSource 2>&1
        $testCompileExit = $LASTEXITCODE
        $testOutput | Set-Content -LiteralPath $testCompileLog -Encoding UTF8
        if ($testCompileExit -ne 0 -or -not (Test-Path -LiteralPath $testObject)) {
            throw "Candidate test compile failed: $($entry.Address)"
        }
        $linkOutput = & (Join-Path $vcRoot 'bin\link.exe') /nologo /out:$testExe $object $testObject 2>&1
        $linkExit = $LASTEXITCODE
        $linkOutput | Add-Content -LiteralPath $testCompileLog -Encoding UTF8
        if ($linkExit -ne 0 -or -not (Test-Path -LiteralPath $testExe)) {
            throw "Candidate test link failed: $($entry.Address)"
        }
        $runOutput = & $testExe 2>&1
        $runExit = $LASTEXITCODE
        $runOutput | Set-Content -LiteralPath $testRunLog -Encoding UTF8
        $behaviorPassed = $runExit -eq 0 -and ($runOutput -match $entry.PassPattern)
        $results += [pscustomobject]@{
            address = $entry.Address
            module = $entry.Module
            compiler = 'Microsoft Visual C++ 7.1'
            status = if ($passed) { 'PASS' } else { 'FAIL' }
            behavior_test = if ($behaviorPassed) { 'PASS' } else { 'FAIL' }
            source = $source
            object = if ($passed) { $object } else { '' }
            log = $log
            test_log = $testRunLog
        }
        if (-not $passed) {
            throw "Candidate compile failed: $($entry.Address) $($entry.Source)"
        }
        if (-not $behaviorPassed) {
            throw "Candidate behavior test failed: $($entry.Address)"
        }
    }
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}

$report = Join-Path $reportDir 'vc71-compiled.tsv'
$results | Export-Csv -LiteralPath "$report.tmp" -Delimiter "`t" -NoTypeInformation -Encoding UTF8
Move-Item -LiteralPath "$report.tmp" -Destination $report -Force
Write-Output "CANDIDATE_BUILD PASS objects=$($results.Count) out=$outDir"
