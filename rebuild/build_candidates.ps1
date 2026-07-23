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
    [pscustomobject]@{
        Address = '00415920'
        Module = 'CGameComponent'
        Source = 'CGameComponent_Init_00415920.cpp'
        TestSource = 'CGameComponent_Init_00415920_test.cpp'
        PassPattern = 'PASS_00415920'
    }
    [pscustomobject]@{
        Address = '00415930'
        Module = 'CGameComponent'
        Source = 'CGameComponent_SetQuit_00415930.cpp'
        TestSource = 'CGameComponent_SetQuit_00415930_test.cpp'
        PassPattern = 'PASS_00415930'
    }
    [pscustomobject]@{
        Address = '00415e83'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_OnPreDeviceReset_00415e83.cpp'
        TestSource = 'CMainGameComponent_OnPreDeviceReset_00415e83_test.cpp'
        PassPattern = 'PASS_00415e83'
    }
    [pscustomobject]@{
        Address = '004197b0'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_IsEditorActive_004197b0.cpp'
        TestSource = 'CMainGameComponent_IsEditorActive_004197b0_test.cpp'
        PassPattern = 'PASS_004197b0'
    }
    [pscustomobject]@{
        Address = '00415fbc'
        Module = '_global'
        Source = 'global_GFGetPreMainMemoryUsed_00415fbc.cpp'
        TestSource = 'global_GFGetPreMainMemoryUsed_00415fbc_test.cpp'
        PassPattern = 'PASS_00415fbc'
    }
    [pscustomobject]@{
        Address = '004167c8'
        Module = '_global'
        Source = 'global_GFGetPreMainMemoryUsed_004167c8.cpp'
        TestSource = 'global_GFGetPreMainMemoryUsed_004167c8_test.cpp'
        PassPattern = 'PASS_004167c8'
    }
    [pscustomobject]@{
        Address = '004194b5'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004194b5.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004194b5_test.cpp'
        PassPattern = 'PASS_004194b5'
    }
    [pscustomobject]@{
        Address = '004197a0'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_SetQuit_004197a0.cpp'
        TestSource = 'CMainGameComponent_SetQuit_004197a0_test.cpp'
        PassPattern = 'PASS_004197a0'
    }
    [pscustomobject]@{
        Address = '0041c7a0'
        Module = 'list<NUISystem::CList*'
        Source = 'listNUISystemCList_begin_0041c7a0.cpp'
        TestSource = 'listNUISystemCList_begin_0041c7a0_test.cpp'
        PassPattern = 'PASS_0041c7a0'
    }
    [pscustomobject]@{
        Address = '0041cf40'
        Module = 'CMap'
        Source = 'CMap_DrawGetSeaLevel_0041cf40.cpp'
        TestSource = 'CMap_DrawGetSeaLevel_0041cf40_test.cpp'
        PassPattern = 'PASS_0041cf40'
    }
    [pscustomobject]@{
        Address = '0041c7b0'
        Module = '_List_const_iterator<std::_List_val<CVsyncCallbackTimer::CPendingQuery'
        Source = 'ListconstiteratorstdListvalCVsyncCallbackTimerCPendingQuery_operator_0041c7b0.cpp'
        TestSource = 'ListconstiteratorstdListvalCVsyncCallbackTimerCPendingQuery_operator_0041c7b0_test.cpp'
        PassPattern = 'PASS_0041c7b0'
    }
    [pscustomobject]@{
        Address = '0041cd89'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041cd89.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041cd89_test.cpp'
        PassPattern = 'PASS_0041cd89'
    }
    [pscustomobject]@{
        Address = '004175da'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_OnPostDeviceReset_004175da.cpp'
        TestSource = 'CMainGameComponent_OnPostDeviceReset_004175da_test.cpp'
        PassPattern = 'PASS_004175da'
    }
    [pscustomobject]@{
        Address = '00416219'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00416219.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00416219_test.cpp'
        PassPattern = 'PASS_00416219'
    }
    [pscustomobject]@{
        Address = '00416225'
        Module = 'CWorld'
        Source = 'CWorld_DrawGetEnvironment_00416225.cpp'
        TestSource = 'CWorld_DrawGetEnvironment_00416225_test.cpp'
        PassPattern = 'PASS_00416225'
    }
    [pscustomobject]@{
        Address = '00416359'
        Module = 'CWorld'
        Source = 'CWorld_DrawGetEnvironment_00416359.cpp'
        TestSource = 'CWorld_DrawGetEnvironment_00416359_test.cpp'
        PassPattern = 'PASS_00416359'
    }
    [pscustomobject]@{
        Address = '0041cf1f'
        Module = 'CTCPhysicsNavigator'
        Source = 'CTCPhysicsNavigator_EnableNavigator_0041cf1f.cpp'
        TestSource = 'CTCPhysicsNavigator_EnableNavigator_0041cf1f_test.cpp'
        PassPattern = 'PASS_0041cf1f'
    }
    [pscustomobject]@{
        Address = '0041cf33'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041cf33.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041cf33_test.cpp'
        PassPattern = 'PASS_0041cf33'
    }
    [pscustomobject]@{
        Address = '00416047'
        Module = 'CEditInputProcessCopyPaste'
        Source = 'CEditInputProcessCopyPaste_AddToInterface_00416047.cpp'
        TestSource = 'CEditInputProcessCopyPaste_AddToInterface_00416047_test.cpp'
        PassPattern = 'PASS_00416047'
    }
    [pscustomobject]@{
        Address = '0041c820'
        Module = 'CPixelFormat'
        Source = 'CPixelFormat_IsEquivalent_0041c820.cpp'
        TestSource = 'CPixelFormat_IsEquivalent_0041c820_test.cpp'
        PassPattern = 'PASS_0041c820'
    }
    [pscustomobject]@{
        Address = '00419036'
        Module = '_global'
        Source = 'global_DeleteData_00419036.cpp'
        TestSource = 'global_DeleteData_00419036_test.cpp'
        PassPattern = 'PASS_00419036'
    }
    [pscustomobject]@{
        Address = '00415bd0'
        Module = 'CSurface'
        Source = 'CSurface_GetWidth_00415bd0.cpp'
        TestSource = 'CSurface_GetWidth_00415bd0_test.cpp'
        PassPattern = 'PASS_00415bd0'
    }
    [pscustomobject]@{
        Address = '00415fc1'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00415fc1.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00415fc1_test.cpp'
        PassPattern = 'PASS_00415fc1'
    }
    [pscustomobject]@{
        Address = '00415fe0'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_ProcessEvent_00415fe0.cpp'
        TestSource = 'CMainGameComponent_ProcessEvent_00415fe0_test.cpp'
        PassPattern = 'PASS_00415fe0'
    }
    [pscustomobject]@{
        Address = '00415530'
        Module = 'CActionDoCreatureAction'
        Source = 'CActionDoCreatureAction_GetActionName_00415530.cpp'
        TestSource = 'CActionDoCreatureAction_GetActionName_00415530_test.cpp'
        PassPattern = 'PASS_00415530'
    }
    [pscustomobject]@{
        Address = '00415bf0'
        Module = 'CSurface'
        Source = 'CSurface_GetHeight_00415bf0.cpp'
        TestSource = 'CSurface_GetHeight_00415bf0_test.cpp'
        PassPattern = 'PASS_00415bf0'
    }
    [pscustomobject]@{
        Address = '00415ff2'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_EventIsSystemEvent_00415ff2.cpp'
        TestSource = 'CMainGameComponent_EventIsSystemEvent_00415ff2_test.cpp'
        PassPattern = 'PASS_00415ff2'
    }
    [pscustomobject]@{
        Address = '00419352'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_00419352.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_00419352_test.cpp'
        PassPattern = 'PASS_00419352'
    }
    [pscustomobject]@{
        Address = '0041ca3e'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041ca3e.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041ca3e_test.cpp'
        PassPattern = 'PASS_0041ca3e'
    }
    [pscustomobject]@{
        Address = '0041d1b4'
        Module = 'CBankFile'
        Source = 'CBankFile_GetBankHandle_0041d1b4.cpp'
        TestSource = 'CBankFile_GetBankHandle_0041d1b4_test.cpp'
        PassPattern = 'PASS_0041d1b4'
    }
    [pscustomobject]@{
        Address = '0041c7c0'
        Module = '_Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString'
        Source = 'TreeconstiteratorstdTreevalstdTmaptraitsCCharString_operator_0041c7c0.cpp'
        TestSource = 'TreeconstiteratorstdTreevalstdTmaptraitsCCharString_operator_0041c7c0_test.cpp'
        PassPattern = 'PASS_0041c7c0'
    }
    [pscustomobject]@{
        Address = '0041c830'
        Module = '_Tree_const_iterator<std::_Tree_val<std::_Tmap_traits<CCharString'
        Source = 'TreeconstiteratorstdTreevalstdTmaptraitsCCharString_operator_0041c830.cpp'
        TestSource = 'TreeconstiteratorstdTreevalstdTmaptraitsCCharString_operator_0041c830_test.cpp'
        PassPattern = 'PASS_0041c830'
    }
    [pscustomobject]@{
        Address = '0041ce9d'
        Module = 'CManager'
        Source = 'CManager_UpdateErrorMessage_0041ce9d.cpp'
        TestSource = 'CManager_UpdateErrorMessage_0041ce9d_test.cpp'
        PassPattern = 'PASS_0041ce9d'
    }
    [pscustomobject]@{
        Address = '00416202'
        Module = '_global'
        Source = 'global_operatornew_00416202.cpp'
        TestSource = 'global_operatornew_00416202_test.cpp'
        PassPattern = 'PASS_00416202'
    }
    [pscustomobject]@{
        Address = '00416268'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_BeginInputLoading_00416268.cpp'
        TestSource = 'CMainGameComponent_BeginInputLoading_00416268_test.cpp'
        PassPattern = 'PASS_00416268'
    }
    [pscustomobject]@{
        Address = '0041627f'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_BeginInputLoading_0041627f.cpp'
        TestSource = 'CMainGameComponent_BeginInputLoading_0041627f_test.cpp'
        PassPattern = 'PASS_0041627f'
    }
    [pscustomobject]@{
        Address = '00419463'
        Module = 'CCountedPointer<CTexture>'
        Source = 'CCountedPointerCTexture_operator_00419463.cpp'
        TestSource = 'CCountedPointerCTexture_operator_00419463_test.cpp'
        PassPattern = 'PASS_00419463'
    }
    [pscustomobject]@{
        Address = '004194ba'
        Module = 'CCountedPointer<CTexture>'
        Source = 'CCountedPointerCTexture_operator_004194ba.cpp'
        TestSource = 'CCountedPointerCTexture_operator_004194ba_test.cpp'
        PassPattern = 'PASS_004194ba'
    }
    [pscustomobject]@{
        Address = '00419554'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419554.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419554_test.cpp'
        PassPattern = 'PASS_00419554'
    }
    [pscustomobject]@{
        Address = '00414f70'
        Module = 'CConsoleCommandBase'
        Source = 'CConsoleCommandBase_GetTypeAsStringCConsoleCommandBaseMB_00414f70.cpp'
        TestSource = 'CConsoleCommandBase_GetTypeAsStringCConsoleCommandBaseMB_00414f70_test.cpp'
        PassPattern = 'PASS_00414f70'
    }
    [pscustomobject]@{
        Address = '00415d70'
        Module = 'CDefString'
        Source = 'CDefString_operatorclassCCharString_00415d70.cpp'
        TestSource = 'CDefString_operatorclassCCharString_00415d70_test.cpp'
        PassPattern = 'PASS_00415d70'
    }
    [pscustomobject]@{
        Address = '00419764'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00419764.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00419764_test.cpp'
        PassPattern = 'PASS_00419764'
    }
    [pscustomobject]@{
        Address = '0041c7e0'
        Module = 'CMemoryAllocatorVariableSize'
        Source = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_0041c7e0.cpp'
        TestSource = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_0041c7e0_test.cpp'
        PassPattern = 'PASS_0041c7e0'
    }
    [pscustomobject]@{
        Address = '0041c800'
        Module = 'CMemoryAllocatorVariableSize'
        Source = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_0041c800.cpp'
        TestSource = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_0041c800_test.cpp'
        PassPattern = 'PASS_0041c800'
    }
    [pscustomobject]@{
        Address = '0041bae0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bae0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bae0_test.cpp'
        PassPattern = 'PASS_0041bae0'
    }
    [pscustomobject]@{
        Address = '0041bd00'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd00.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd00_test.cpp'
        PassPattern = 'PASS_0041bd00'
    }
    [pscustomobject]@{
        Address = '0041bd60'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd60.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd60_test.cpp'
        PassPattern = 'PASS_0041bd60'
    }
    [pscustomobject]@{
        Address = '0041cc50'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041cc50.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041cc50_test.cpp'
        PassPattern = 'PASS_0041cc50'
    }
    [pscustomobject]@{
        Address = '00415e67'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415e67.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415e67_test.cpp'
        PassPattern = 'PASS_00415e67'
    }
    [pscustomobject]@{
        Address = '0041940c'
        Module = '_global'
        Source = 'global_DeleteData_0041940c.cpp'
        TestSource = 'global_DeleteData_0041940c_test.cpp'
        PassPattern = 'PASS_0041940c'
    }
    [pscustomobject]@{
        Address = '0041977d'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041977d.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041977d_test.cpp'
        PassPattern = 'PASS_0041977d'
    }
    [pscustomobject]@{
        Address = '0041d198'
        Module = 'CManager'
        Source = 'CManager_SetGraphicsBank_0041d198.cpp'
        TestSource = 'CManager_SetGraphicsBank_0041d198_test.cpp'
        PassPattern = 'PASS_0041d198'
    }
    [pscustomobject]@{
        Address = '00412b00'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00412b00.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00412b00_test.cpp'
        PassPattern = 'PASS_00412b00'
    }
    [pscustomobject]@{
        Address = '00412c50'
        Module = 'vector<EWeaponClass'
        Source = 'vectorEWeaponClass_UmoveEWeaponClass_00412c50.cpp'
        TestSource = 'vectorEWeaponClass_UmoveEWeaponClass_00412c50_test.cpp'
        PassPattern = 'PASS_00412c50'
    }
    [pscustomobject]@{
        Address = '00415050'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415050.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415050_test.cpp'
        PassPattern = 'PASS_00415050'
    }
    [pscustomobject]@{
        Address = '0041c760'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041c760.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041c760_test.cpp'
        PassPattern = 'PASS_0041c760'
    }
    [pscustomobject]@{
        Address = '00416296'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_IsControllerDisconnected_00416296.cpp'
        TestSource = 'CMainGameComponent_IsControllerDisconnected_00416296_test.cpp'
        PassPattern = 'PASS_00416296'
    }
    [pscustomobject]@{
        Address = '00414e00'
        Module = '_Destroy_range<std'
        Source = 'Destroyrangestd_allocatorCSpeechFrame_00414e00.cpp'
        TestSource = 'Destroyrangestd_allocatorCSpeechFrame_00414e00_test.cpp'
        PassPattern = 'PASS_00414e00'
    }
    [pscustomobject]@{
        Address = '004167a7'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_004167a7.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_004167a7_test.cpp'
        PassPattern = 'PASS_004167a7'
    }
    [pscustomobject]@{
        Address = '004131d0'
        Module = '_Dest_val<std'
        Source = 'Destvalstd_allocatorCMacroCMacro_004131d0.cpp'
        TestSource = 'Destvalstd_allocatorCMacroCMacro_004131d0_test.cpp'
        PassPattern = 'PASS_004131d0'
    }
    [pscustomobject]@{
        Address = '0041bb00'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041bb00.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041bb00_test.cpp'
        PassPattern = 'PASS_0041bb00'
    }
    [pscustomobject]@{
        Address = '004193a0'
        Module = '_global'
        Source = 'global_DeleteData_004193a0.cpp'
        TestSource = 'global_DeleteData_004193a0_test.cpp'
        PassPattern = 'PASS_004193a0'
    }
    [pscustomobject]@{
        Address = '004193c4'
        Module = '_global'
        Source = 'global_DeleteData_004193c4.cpp'
        TestSource = 'global_DeleteData_004193c4_test.cpp'
        PassPattern = 'PASS_004193c4'
    }
    [pscustomobject]@{
        Address = '004193e8'
        Module = '_global'
        Source = 'global_DeleteData_004193e8.cpp'
        TestSource = 'global_DeleteData_004193e8_test.cpp'
        PassPattern = 'PASS_004193e8'
    }
    [pscustomobject]@{
        Address = '00416392'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_PeekWorldChecksum_00416392.cpp'
        TestSource = 'CMainGameComponent_PeekWorldChecksum_00416392_test.cpp'
        PassPattern = 'PASS_00416392'
    }
    [pscustomobject]@{
        Address = '0041919c'
        Module = '_global'
        Source = 'global_GFLimitReturndouble_0041919c.cpp'
        TestSource = 'global_GFLimitReturndouble_0041919c_test.cpp'
        PassPattern = 'PASS_0041919c'
    }
    [pscustomobject]@{
        Address = '0041cc2b'
        Module = 'CTCLook'
        Source = 'CTCLook_GetHeadAngularVelocityMaxXY_0041cc2b.cpp'
        TestSource = 'CTCLook_GetHeadAngularVelocityMaxXY_0041cc2b_test.cpp'
        PassPattern = 'PASS_0041cc2b'
    }
    [pscustomobject]@{
        Address = '004160a6'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_UninitEventPackageLoading_004160a6.cpp'
        TestSource = 'CMainGameComponent_UninitEventPackageLoading_004160a6_test.cpp'
        PassPattern = 'PASS_004160a6'
    }
    [pscustomobject]@{
        Address = '00416122'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_UninitEventPackageLoading_00416122.cpp'
        TestSource = 'CMainGameComponent_UninitEventPackageLoading_00416122_test.cpp'
        PassPattern = 'PASS_00416122'
    }
    [pscustomobject]@{
        Address = '004190e2'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_004190e2.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_004190e2_test.cpp'
        PassPattern = 'PASS_004190e2'
    }
    [pscustomobject]@{
        Address = '00419134'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_00419134.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_00419134_test.cpp'
        PassPattern = 'PASS_00419134'
    }
    [pscustomobject]@{
        Address = '00419047'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419047.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419047_test.cpp'
        PassPattern = 'PASS_00419047'
    }
    [pscustomobject]@{
        Address = '00419650'
        Module = 'CThing'
        Source = 'CThing_GetPBaseDef_00419650.cpp'
        TestSource = 'CThing_GetPBaseDef_00419650_test.cpp'
        PassPattern = 'PASS_00419650'
    }
    [pscustomobject]@{
        Address = '0041ba70'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041ba70.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041ba70_test.cpp'
        PassPattern = 'PASS_0041ba70'
    }
    [pscustomobject]@{
        Address = '00414f90'
        Module = 'CCompressedMemoryDataInputStream'
        Source = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_00414f90.cpp'
        TestSource = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_00414f90_test.cpp'
        PassPattern = 'PASS_00414f90'
    }
    [pscustomobject]@{
        Address = '0041971e'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0041971e.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0041971e_test.cpp'
        PassPattern = 'PASS_0041971e'
    }
    [pscustomobject]@{
        Address = '004190b6'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004190b6.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004190b6_test.cpp'
        PassPattern = 'PASS_004190b6'
    }
    [pscustomobject]@{
        Address = '00419108'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00419108.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00419108_test.cpp'
        PassPattern = 'PASS_00419108'
    }
    [pscustomobject]@{
        Address = '00412f60'
        Module = 'CInitBaseClass'
        Source = 'CInitBaseClass_Uninitialise_00412f60.cpp'
        TestSource = 'CInitBaseClass_Uninitialise_00412f60_test.cpp'
        PassPattern = 'PASS_00412f60'
    }
    [pscustomobject]@{
        Address = '00416365'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_PostChangeResolution_00416365.cpp'
        TestSource = 'CMainGameComponent_PostChangeResolution_00416365_test.cpp'
        PassPattern = 'PASS_00416365'
    }
    [pscustomobject]@{
        Address = '0041bcd0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bcd0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bcd0_test.cpp'
        PassPattern = 'PASS_0041bcd0'
    }
    [pscustomobject]@{
        Address = '0041bd80'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_0041bd80.cpp'
        TestSource = 'CTCMapwho_OnDie_0041bd80_test.cpp'
        PassPattern = 'PASS_0041bd80'
    }
    [pscustomobject]@{
        Address = '0041732a'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_InitPlayerManager_0041732a.cpp'
        TestSource = 'CMainGameComponent_InitPlayerManager_0041732a_test.cpp'
        PassPattern = 'PASS_0041732a'
    }
    [pscustomobject]@{
        Address = '00419680'
        Module = 'CBulletTimeManager'
        Source = 'CBulletTimeManager_IsBulletTimeActive_00419680.cpp'
        TestSource = 'CBulletTimeManager_IsBulletTimeActive_00419680_test.cpp'
        PassPattern = 'PASS_00419680'
    }
    [pscustomobject]@{
        Address = '0041baa0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041baa0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041baa0_test.cpp'
        PassPattern = 'PASS_0041baa0'
    }
    [pscustomobject]@{
        Address = '0041d165'
        Module = '_global'
        Source = 'global_OnCloseMenus_0041d165.cpp'
        TestSource = 'global_OnCloseMenus_0041d165_test.cpp'
        PassPattern = 'PASS_0041d165'
    }
    [pscustomobject]@{
        Address = '00413310'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00413310.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00413310_test.cpp'
        PassPattern = 'PASS_00413310'
    }
    [pscustomobject]@{
        Address = '0041a810'
        Module = 'C2DBoxF'
        Source = 'C2DBoxF_Move_0041a810.cpp'
        TestSource = 'C2DBoxF_Move_0041a810_test.cpp'
        PassPattern = 'PASS_0041a810'
    }
    [pscustomobject]@{
        Address = '0041bc90'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0041bc90.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0041bc90_test.cpp'
        PassPattern = 'PASS_0041bc90'
    }
    [pscustomobject]@{
        Address = '00412c10'
        Module = 'vector<EWeaponClass'
        Source = 'vectorEWeaponClass_UmoveEWeaponClass_00412c10.cpp'
        TestSource = 'vectorEWeaponClass_UmoveEWeaponClass_00412c10_test.cpp'
        PassPattern = 'PASS_00412c10'
    }
    [pscustomobject]@{
        Address = '00416670'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_ProcessEventPackage_00416670.cpp'
        TestSource = 'CMainGameComponent_ProcessEventPackage_00416670_test.cpp'
        PassPattern = 'PASS_00416670'
    }
    [pscustomobject]@{
        Address = '0041ab00'
        Module = 'CSprite'
        Source = 'CSprite_GetDefaultGraphicIndex_0041ab00.cpp'
        TestSource = 'CSprite_GetDefaultGraphicIndex_0041ab00_test.cpp'
        PassPattern = 'PASS_0041ab00'
    }
    [pscustomobject]@{
        Address = '0041be70'
        Module = 'CEnginePrimitiveHandle'
        Source = 'CEnginePrimitiveHandle_Clear_0041be70.cpp'
        TestSource = 'CEnginePrimitiveHandle_Clear_0041be70_test.cpp'
        PassPattern = 'PASS_0041be70'
    }
    [pscustomobject]@{
        Address = '004166a8'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_CreatePlayers_004166a8.cpp'
        TestSource = 'CMainGameComponent_CreatePlayers_004166a8_test.cpp'
        PassPattern = 'PASS_004166a8'
    }
    [pscustomobject]@{
        Address = '00419365'
        Module = 'CAssoc'
        Source = 'CAssoc_scalardeletingdestructor_00419365.cpp'
        TestSource = 'CAssoc_scalardeletingdestructor_00419365_test.cpp'
        PassPattern = 'PASS_00419365'
    }
    [pscustomobject]@{
        Address = '00419428'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_00419428.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_00419428_test.cpp'
        PassPattern = 'PASS_00419428'
    }
    [pscustomobject]@{
        Address = '0041947a'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_0041947a.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_0041947a_test.cpp'
        PassPattern = 'PASS_0041947a'
    }
    [pscustomobject]@{
        Address = '00419518'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419518.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00419518_test.cpp'
        PassPattern = 'PASS_00419518'
    }
    [pscustomobject]@{
        Address = '0041bdb0'
        Module = 'CGraphicFrameData'
        Source = 'CGraphicFrameData_scalardeletingdestructor_0041bdb0.cpp'
        TestSource = 'CGraphicFrameData_scalardeletingdestructor_0041bdb0_test.cpp'
        PassPattern = 'PASS_0041bdb0'
    }
    [pscustomobject]@{
        Address = '0041bd20'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd20.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041bd20_test.cpp'
        PassPattern = 'PASS_0041bd20'
    }
    [pscustomobject]@{
        Address = '0041d1c8'
        Module = 'CTCBoastingCrowdControl'
        Source = 'CTCBoastingCrowdControl_OnKillCrowd_0041d1c8.cpp'
        TestSource = 'CTCBoastingCrowdControl_OnKillCrowd_0041d1c8_test.cpp'
        PassPattern = 'PASS_0041d1c8'
    }
    [pscustomobject]@{
        Address = '00413350'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00413350.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00413350_test.cpp'
        PassPattern = 'PASS_00413350'
    }
    [pscustomobject]@{
        Address = '00412b20'
        Module = 'vector<std::pair<CCharString'
        Source = 'vectorstdpairCCharString_resize_00412b20.cpp'
        TestSource = 'vectorstdpairCCharString_resize_00412b20_test.cpp'
        PassPattern = 'PASS_00412b20'
    }
    [pscustomobject]@{
        Address = '00416005'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_InitDefinitions_00416005.cpp'
        TestSource = 'CMainGameComponent_InitDefinitions_00416005_test.cpp'
        PassPattern = 'PASS_00416005'
    }
    [pscustomobject]@{
        Address = '0041915a'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0041915a.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0041915a_test.cpp'
        PassPattern = 'PASS_0041915a'
    }
    [pscustomobject]@{
        Address = '0041d122'
        Module = '_global'
        Source = 'global_OnOpenMenus_0041d122.cpp'
        TestSource = 'global_OnOpenMenus_0041d122_test.cpp'
        PassPattern = 'PASS_0041d122'
    }
    [pscustomobject]@{
        Address = '0041649c'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_AddGameEvent_0041649c.cpp'
        TestSource = 'CMainGameComponent_AddGameEvent_0041649c_test.cpp'
        PassPattern = 'PASS_0041649c'
    }
    [pscustomobject]@{
        Address = '00414c90'
        Module = '_global'
        Source = 'global_Get_00414c90.cpp'
        TestSource = 'global_Get_00414c90_test.cpp'
        PassPattern = 'PASS_00414c90'
    }
    [pscustomobject]@{
        Address = '00415dd0'
        Module = 'CDefString'
        Source = 'CDefString_operatorclassCCharString_00415dd0.cpp'
        TestSource = 'CDefString_operatorclassCCharString_00415dd0_test.cpp'
        PassPattern = 'PASS_00415dd0'
    }
    [pscustomobject]@{
        Address = '004194d1'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_004194d1.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_004194d1_test.cpp'
        PassPattern = 'PASS_004194d1'
    }
    [pscustomobject]@{
        Address = '00413230'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00413230.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00413230_test.cpp'
        PassPattern = 'PASS_00413230'
    }
    [pscustomobject]@{
        Address = '0041906e'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041906e.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0041906e_test.cpp'
        PassPattern = 'PASS_0041906e'
    }
    [pscustomobject]@{
        Address = '00412a50'
        Module = 'vector<unsigned_int'
        Source = 'vectorunsignedint_resize_00412a50.cpp'
        TestSource = 'vectorunsignedint_resize_00412a50_test.cpp'
        PassPattern = 'PASS_00412a50'
    }
    [pscustomobject]@{
        Address = '00416056'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_InitEventPackageLoading_00416056.cpp'
        TestSource = 'CMainGameComponent_InitEventPackageLoading_00416056_test.cpp'
        PassPattern = 'PASS_00416056'
    }
    [pscustomobject]@{
        Address = '00415940'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415940.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00415940_test.cpp'
        PassPattern = 'PASS_00415940'
    }
    [pscustomobject]@{
        Address = '0041799b'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_ChangeTextureColourDepthCMainGameCom_0041799b.cpp'
        TestSource = 'CMainGameComponent_ChangeTextureColourDepthCMainGameCom_0041799b_test.cpp'
        PassPattern = 'PASS_0041799b'
    }
    [pscustomobject]@{
        Address = '00415830'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00415830.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00415830_test.cpp'
        PassPattern = 'PASS_00415830'
    }
    [pscustomobject]@{
        Address = '004195af'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_004195af.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_004195af_test.cpp'
        PassPattern = 'PASS_004195af'
    }
    [pscustomobject]@{
        Address = '00416c35'
        Module = '_global'
        Source = 'global_ConsoleForceUpdateTickSpeed_00416c35.cpp'
        TestSource = 'global_ConsoleForceUpdateTickSpeed_00416c35_test.cpp'
        PassPattern = 'PASS_00416c35'
    }
    [pscustomobject]@{
        Address = '0041bdf0'
        Module = 'CGraphicFrameData'
        Source = 'CGraphicFrameData_scalardeletingdestructor_0041bdf0.cpp'
        TestSource = 'CGraphicFrameData_scalardeletingdestructor_0041bdf0_test.cpp'
        PassPattern = 'PASS_0041bdf0'
    }
    [pscustomobject]@{
        Address = '004160cc'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_InitEventPackageSaving_004160cc.cpp'
        TestSource = 'CMainGameComponent_InitEventPackageSaving_004160cc_test.cpp'
        PassPattern = 'PASS_004160cc'
    }
    [pscustomobject]@{
        Address = '004167da'
        Module = '_global'
        Source = 'global_AppReinitFunc_004167da.cpp'
        TestSource = 'global_AppReinitFunc_004167da_test.cpp'
        PassPattern = 'PASS_004167da'
    }
    [pscustomobject]@{
        Address = '00412aa0'
        Module = 'vector<EWeaponClass'
        Source = 'vectorEWeaponClass_UmoveEWeaponClass_00412aa0.cpp'
        TestSource = 'vectorEWeaponClass_UmoveEWeaponClass_00412aa0_test.cpp'
        PassPattern = 'PASS_00412aa0'
    }
    [pscustomobject]@{
        Address = '004161a7'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_AddEventPackageSetToSave_004161a7.cpp'
        TestSource = 'CMainGameComponent_AddEventPackageSetToSave_004161a7_test.cpp'
        PassPattern = 'PASS_004161a7'
    }
    [pscustomobject]@{
        Address = '004129f0'
        Module = 'vector<CBrainBehaviour'
        Source = 'vectorCBrainBehaviour_resize_004129f0.cpp'
        TestSource = 'vectorCBrainBehaviour_resize_004129f0_test.cpp'
        PassPattern = 'PASS_004129f0'
    }
    [pscustomobject]@{
        Address = '0041674a'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_IsTimeForServerUpdate_0041674a.cpp'
        TestSource = 'CMainGameComponent_IsTimeForServerUpdate_0041674a_test.cpp'
        PassPattern = 'PASS_0041674a'
    }
    [pscustomobject]@{
        Address = '0041c4a0'
        Module = 'CObject'
        Source = 'CObject_CObject_0041c4a0.cpp'
        TestSource = 'CObject_CObject_0041c4a0_test.cpp'
        PassPattern = 'PASS_0041c4a0'
    }
    [pscustomobject]@{
        Address = '004191c1'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004191c1.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004191c1_test.cpp'
        PassPattern = 'PASS_004191c1'
    }
    [pscustomobject]@{
        Address = '0041c700'
        Module = 'CGraphicFrame'
        Source = 'CGraphicFrame_UnInitialise_0041c700.cpp'
        TestSource = 'CGraphicFrame_UnInitialise_0041c700_test.cpp'
        PassPattern = 'PASS_0041c700'
    }
    [pscustomobject]@{
        Address = '00416148'
        Module = 'CMainGameComponent'
        Source = 'CMainGameComponent_GetEventPackageSetFromSave_00416148.cpp'
        TestSource = 'CMainGameComponent_GetEventPackageSetFromSave_00416148_test.cpp'
        PassPattern = 'PASS_00416148'
    }
    [pscustomobject]@{
        Address = '0042d960'
        Module = 'CResource'
        Source = 'CResource_OnResourceLoaded_0042d960.cpp'
        TestSource = 'CResource_OnResourceLoaded_0042d960_test.cpp'
        PassPattern = 'PASS_0042d960'
    }
    [pscustomobject]@{
        Address = '0042d940'
        Module = 'CResource'
        Source = 'CResource_UnloadResource_0042d940.cpp'
        TestSource = 'CResource_UnloadResource_0042d940_test.cpp'
        PassPattern = 'PASS_0042d940'
    }
    [pscustomobject]@{
        Address = '0042d950'
        Module = 'CResource'
        Source = 'CResource_IsResourceLoaded_0042d950.cpp'
        TestSource = 'CResource_IsResourceLoaded_0042d950_test.cpp'
        PassPattern = 'PASS_0042d950'
    }
    [pscustomobject]@{
        Address = '0042da20'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_SetInstantiationName_0042da20.cpp'
        TestSource = 'CDefClassBase_SetInstantiationName_0042da20_test.cpp'
        PassPattern = 'PASS_0042da20'
    }
    [pscustomobject]@{
        Address = '0042da30'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_AddSubDef_0042da30.cpp'
        TestSource = 'CDefClassBase_AddSubDef_0042da30_test.cpp'
        PassPattern = 'PASS_0042da30'
    }
    [pscustomobject]@{
        Address = '00429c53'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429c53.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429c53_test.cpp'
        PassPattern = 'PASS_00429c53'
    }
    [pscustomobject]@{
        Address = '00429d8e'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429d8e.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429d8e_test.cpp'
        PassPattern = 'PASS_00429d8e'
    }
    [pscustomobject]@{
        Address = '0042d970'
        Module = 'CResource'
        Source = 'CResource_CanAbortResourceLoad_0042d970.cpp'
        TestSource = 'CResource_CanAbortResourceLoad_0042d970_test.cpp'
        PassPattern = 'PASS_0042d970'
    }
    [pscustomobject]@{
        Address = '0042d930'
        Module = 'CIVCountedPointeeBase'
        Source = 'CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp'
        TestSource = 'CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930_test.cpp'
        PassPattern = 'PASS_0042d930'
    }
    [pscustomobject]@{
        Address = '00429a79'
        Module = 'CSmallVector<CANavQuadTreeNode_const*'
        Source = 'CSmallVectorCANavQuadTreeNodeconst_operator_00429a79.cpp'
        TestSource = 'CSmallVectorCANavQuadTreeNodeconst_operator_00429a79_test.cpp'
        PassPattern = 'PASS_00429a79'
    }
    [pscustomobject]@{
        Address = '00428b10'
        Module = '_global'
        Source = 'global_DeleteData_00428b10.cpp'
        TestSource = 'global_DeleteData_00428b10_test.cpp'
        PassPattern = 'PASS_00428b10'
    }
    [pscustomobject]@{
        Address = '0042b636'
        Module = '_global'
        Source = 'global_CalcStringLength_0042b636.cpp'
        TestSource = 'global_CalcStringLength_0042b636_test.cpp'
        PassPattern = 'PASS_0042b636'
    }
    [pscustomobject]@{
        Address = '0042b677'
        Module = '_global'
        Source = 'global_CalcStringLength_0042b677.cpp'
        TestSource = 'global_CalcStringLength_0042b677_test.cpp'
        PassPattern = 'PASS_0042b677'
    }
    [pscustomobject]@{
        Address = '004293f2'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_004293f2.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_004293f2_test.cpp'
        PassPattern = 'PASS_004293f2'
    }
    [pscustomobject]@{
        Address = '00429cea'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429cea.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429cea_test.cpp'
        PassPattern = 'PASS_00429cea'
    }
    [pscustomobject]@{
        Address = '0042a2b9'
        Module = 'SetStaticCast<NUISystem'
        Source = 'SetStaticCastNUISystem_CComponent_0042a2b9.cpp'
        TestSource = 'SetStaticCastNUISystem_CComponent_0042a2b9_test.cpp'
        PassPattern = 'PASS_0042a2b9'
    }
    [pscustomobject]@{
        Address = '0042a2ca'
        Module = 'SetStaticCast<NUISystem'
        Source = 'SetStaticCastNUISystem_CComponent_0042a2ca.cpp'
        TestSource = 'SetStaticCastNUISystem_CComponent_0042a2ca_test.cpp'
        PassPattern = 'PASS_0042a2ca'
    }
    [pscustomobject]@{
        Address = '0042a2db'
        Module = 'SetStaticCast<NUISystem'
        Source = 'SetStaticCastNUISystem_CComponent_0042a2db.cpp'
        TestSource = 'SetStaticCastNUISystem_CComponent_0042a2db_test.cpp'
        PassPattern = 'PASS_0042a2db'
    }
    [pscustomobject]@{
        Address = '00429e16'
        Module = 'CPair'
        Source = 'CPair_CPair_00429e16.cpp'
        TestSource = 'CPair_CPair_00429e16_test.cpp'
        PassPattern = 'PASS_00429e16'
    }
    [pscustomobject]@{
        Address = '0042bead'
        Module = 'CTCInventoryAbilities'
        Source = 'CTCInventoryAbilities_AddLearnedExpression_0042bead.cpp'
        TestSource = 'CTCInventoryAbilities_AddLearnedExpression_0042bead_test.cpp'
        PassPattern = 'PASS_0042bead'
    }
    [pscustomobject]@{
        Address = '0042d437'
        Module = 'CTCInventoryAbilities'
        Source = 'CTCInventoryAbilities_AddLearnedExpression_0042d437.cpp'
        TestSource = 'CTCInventoryAbilities_AddLearnedExpression_0042d437_test.cpp'
        PassPattern = 'PASS_0042d437'
    }
    [pscustomobject]@{
        Address = '0042d44a'
        Module = 'CTCInventoryAbilities'
        Source = 'CTCInventoryAbilities_AddLearnedExpression_0042d44a.cpp'
        TestSource = 'CTCInventoryAbilities_AddLearnedExpression_0042d44a_test.cpp'
        PassPattern = 'PASS_0042d44a'
    }
    [pscustomobject]@{
        Address = '0041d207'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_0041d207.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_0041d207_test.cpp'
        PassPattern = 'PASS_0041d207'
    }
    [pscustomobject]@{
        Address = '0042bec0'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_0042bec0.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_0042bec0_test.cpp'
        PassPattern = 'PASS_0042bec0'
    }
    [pscustomobject]@{
        Address = '0042bef2'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042bef2.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042bef2_test.cpp'
        PassPattern = 'PASS_0042bef2'
    }
    [pscustomobject]@{
        Address = '00429403'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429403.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429403_test.cpp'
        PassPattern = 'PASS_00429403'
    }
    [pscustomobject]@{
        Address = '004293db'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_004293db.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_004293db_test.cpp'
        PassPattern = 'PASS_004293db'
    }
    [pscustomobject]@{
        Address = '00429a62'
        Module = 'CCountedPointer<CTexture>'
        Source = 'CCountedPointerCTexture_operator_00429a62.cpp'
        TestSource = 'CCountedPointerCTexture_operator_00429a62_test.cpp'
        PassPattern = 'PASS_00429a62'
    }
    [pscustomobject]@{
        Address = '00429c58'
        Module = 'CCountedPointer<CTexture>'
        Source = 'CCountedPointerCTexture_operator_00429c58.cpp'
        TestSource = 'CCountedPointerCTexture_operator_00429c58_test.cpp'
        PassPattern = 'PASS_00429c58'
    }
    [pscustomobject]@{
        Address = '00429db7'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429db7.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429db7_test.cpp'
        PassPattern = 'PASS_00429db7'
    }
    [pscustomobject]@{
        Address = '0042ab1c'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042ab1c.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042ab1c_test.cpp'
        PassPattern = 'PASS_0042ab1c'
    }
    [pscustomobject]@{
        Address = '0042ab73'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042ab73.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042ab73_test.cpp'
        PassPattern = 'PASS_0042ab73'
    }
    [pscustomobject]@{
        Address = '0042abca'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042abca.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042abca_test.cpp'
        PassPattern = 'PASS_0042abca'
    }
    [pscustomobject]@{
        Address = '0042b755'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042b755.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042b755_test.cpp'
        PassPattern = 'PASS_0042b755'
    }
    [pscustomobject]@{
        Address = '0042b796'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042b796.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042b796_test.cpp'
        PassPattern = 'PASS_0042b796'
    }
    [pscustomobject]@{
        Address = '0042b7d7'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042b7d7.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042b7d7_test.cpp'
        PassPattern = 'PASS_0042b7d7'
    }
    [pscustomobject]@{
        Address = '0042ce63'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042ce63.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042ce63_test.cpp'
        PassPattern = 'PASS_0042ce63'
    }
    [pscustomobject]@{
        Address = '00428b1f'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428b1f.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428b1f_test.cpp'
        PassPattern = 'PASS_00428b1f'
    }
    [pscustomobject]@{
        Address = '00428bd2'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428bd2.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428bd2_test.cpp'
        PassPattern = 'PASS_00428bd2'
    }
    [pscustomobject]@{
        Address = '00428bea'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428bea.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428bea_test.cpp'
        PassPattern = 'PASS_00428bea'
    }
    [pscustomobject]@{
        Address = '00428c02'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428c02.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428c02_test.cpp'
        PassPattern = 'PASS_00428c02'
    }
    [pscustomobject]@{
        Address = '00428c8f'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428c8f.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428c8f_test.cpp'
        PassPattern = 'PASS_00428c8f'
    }
    [pscustomobject]@{
        Address = '00428ca7'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428ca7.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00428ca7_test.cpp'
        PassPattern = 'PASS_00428ca7'
    }
    [pscustomobject]@{
        Address = '00429caf'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429caf.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429caf_test.cpp'
        PassPattern = 'PASS_00429caf'
    }
    [pscustomobject]@{
        Address = '00429d59'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d59.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d59_test.cpp'
        PassPattern = 'PASS_00429d59'
    }
    [pscustomobject]@{
        Address = '00429dce'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429dce.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429dce_test.cpp'
        PassPattern = 'PASS_00429dce'
    }
    [pscustomobject]@{
        Address = '00429de6'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429de6.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429de6_test.cpp'
        PassPattern = 'PASS_00429de6'
    }
    [pscustomobject]@{
        Address = '00429dfe'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429dfe.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429dfe_test.cpp'
        PassPattern = 'PASS_00429dfe'
    }
    [pscustomobject]@{
        Address = '0042d405'
        Module = 'CTCInventoryClothing'
        Source = 'CTCInventoryClothing_SetClothingMappingToMenuEntry_0042d405.cpp'
        TestSource = 'CTCInventoryClothing_SetClothingMappingToMenuEntry_0042d405_test.cpp'
        PassPattern = 'PASS_0042d405'
    }
    [pscustomobject]@{
        Address = '0042d41e'
        Module = 'CTCInventoryClothing'
        Source = 'CTCInventoryClothing_SetClothingMappingToMenuEntry_0042d41e.cpp'
        TestSource = 'CTCInventoryClothing_SetClothingMappingToMenuEntry_0042d41e_test.cpp'
        PassPattern = 'PASS_0042d41e'
    }
    [pscustomobject]@{
        Address = '004299f0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_004299f0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_004299f0_test.cpp'
        PassPattern = 'PASS_004299f0'
    }
    [pscustomobject]@{
        Address = '0042d980'
        Module = 'CResource'
        Source = 'CResource_OnResourceLoadAborted_0042d980.cpp'
        TestSource = 'CResource_OnResourceLoadAborted_0042d980_test.cpp'
        PassPattern = 'PASS_0042d980'
    }
    [pscustomobject]@{
        Address = '004298f3'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004298f3.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004298f3_test.cpp'
        PassPattern = 'PASS_004298f3'
    }
    [pscustomobject]@{
        Address = '0042990f'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042990f.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042990f_test.cpp'
        PassPattern = 'PASS_0042990f'
    }
    [pscustomobject]@{
        Address = '0042992b'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042992b.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042992b_test.cpp'
        PassPattern = 'PASS_0042992b'
    }
    [pscustomobject]@{
        Address = '00429bf9'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429bf9.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429bf9_test.cpp'
        PassPattern = 'PASS_00429bf9'
    }
    [pscustomobject]@{
        Address = '00429d3c'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d3c.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d3c_test.cpp'
        PassPattern = 'PASS_00429d3c'
    }
    [pscustomobject]@{
        Address = '00429d71'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d71.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d71_test.cpp'
        PassPattern = 'PASS_00429d71'
    }
    [pscustomobject]@{
        Address = '0042b44a'
        Module = 'CEngineSceneGrid'
        Source = 'CEngineSceneGrid_GetCellBoundingBox_0042b44a.cpp'
        TestSource = 'CEngineSceneGrid_GetCellBoundingBox_0042b44a_test.cpp'
        PassPattern = 'PASS_0042b44a'
    }
    [pscustomobject]@{
        Address = '0042be11'
        Module = 'CSurface'
        Source = 'CSurface_GetHeight_0042be11.cpp'
        TestSource = 'CSurface_GetHeight_0042be11_test.cpp'
        PassPattern = 'PASS_0042be11'
    }
    [pscustomobject]@{
        Address = '0042bed4'
        Module = 'CBankFile'
        Source = 'CBankFile_GetBankHandle_0042bed4.cpp'
        TestSource = 'CBankFile_GetBankHandle_0042bed4_test.cpp'
        PassPattern = 'PASS_0042bed4'
    }
    [pscustomobject]@{
        Address = '0042bf67'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_0042bf67.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_0042bf67_test.cpp'
        PassPattern = 'PASS_0042bf67'
    }
    [pscustomobject]@{
        Address = '0042d28b'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_0042d28b.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_0042d28b_test.cpp'
        PassPattern = 'PASS_0042d28b'
    }
    [pscustomobject]@{
        Address = '00429a10'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429a10.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429a10_test.cpp'
        PassPattern = 'PASS_00429a10'
    }
    [pscustomobject]@{
        Address = '0042b587'
        Module = '_global'
        Source = 'global_Alloc_0042b587.cpp'
        TestSource = 'global_Alloc_0042b587_test.cpp'
        PassPattern = 'PASS_0042b587'
    }
    [pscustomobject]@{
        Address = '0042bdf0'
        Module = '_global'
        Source = 'global_Alloc_0042bdf0.cpp'
        TestSource = 'global_Alloc_0042bdf0_test.cpp'
        PassPattern = 'PASS_0042bdf0'
    }
    [pscustomobject]@{
        Address = '0042be2f'
        Module = '_global'
        Source = 'global_Alloc_0042be2f.cpp'
        TestSource = 'global_Alloc_0042be2f_test.cpp'
        PassPattern = 'PASS_0042be2f'
    }
    [pscustomobject]@{
        Address = '00429418'
        Module = '_Find<std::_List_unchecked_iterator<std::_List_val<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>_>,NUISystem'
        Source = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_00429418.cpp'
        TestSource = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_00429418_test.cpp'
        PassPattern = 'PASS_00429418'
    }
    [pscustomobject]@{
        Address = '0042a77f'
        Module = '_Find<std::_List_unchecked_iterator<std::_List_val<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>_>,NUISystem'
        Source = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_0042a77f.cpp'
        TestSource = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_0042a77f_test.cpp'
        PassPattern = 'PASS_0042a77f'
    }
    [pscustomobject]@{
        Address = '00429cc7'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429cc7.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429cc7_test.cpp'
        PassPattern = 'PASS_00429cc7'
    }
    [pscustomobject]@{
        Address = '00429d93'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d93.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_00429d93_test.cpp'
        PassPattern = 'PASS_00429d93'
    }
    [pscustomobject]@{
        Address = '0042b707'
        Module = '_global'
        Source = 'global_CalcStringLength_0042b707.cpp'
        TestSource = 'global_CalcStringLength_0042b707_test.cpp'
        PassPattern = 'PASS_0042b707'
    }
    [pscustomobject]@{
        Address = '0042d9a0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042d9a0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042d9a0_test.cpp'
        PassPattern = 'PASS_0042d9a0'
    }
    [pscustomobject]@{
        Address = '004294ac'
        Module = '_Find<std::_List_unchecked_iterator<std::_List_val<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>_>,NUISystem'
        Source = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_004294ac.cpp'
        TestSource = 'FindstdListuncheckediteratorstdListvalNUISystemCComponentstdallocatorNUISystemCComponentNUISystem_CComponent_004294ac_test.cpp'
        PassPattern = 'PASS_004294ac'
    }
    [pscustomobject]@{
        Address = '0041e61b'
        Module = 'CSpellContainerList'
        Source = 'CSpellContainerList_AddChild_0041e61b.cpp'
        TestSource = 'CSpellContainerList_AddChild_0041e61b_test.cpp'
        PassPattern = 'PASS_0041e61b'
    }
    [pscustomobject]@{
        Address = '0042913a'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_0042913a.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_0042913a_test.cpp'
        PassPattern = 'PASS_0042913a'
    }
    [pscustomobject]@{
        Address = '004291b8'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_004291b8.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_004291b8_test.cpp'
        PassPattern = 'PASS_004291b8'
    }
    [pscustomobject]@{
        Address = '0042920a'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_0042920a.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_0042920a_test.cpp'
        PassPattern = 'PASS_0042920a'
    }
    [pscustomobject]@{
        Address = '0042943a'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_0042943a.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_0042943a_test.cpp'
        PassPattern = 'PASS_0042943a'
    }
    [pscustomobject]@{
        Address = '00429460'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_00429460.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_00429460_test.cpp'
        PassPattern = 'PASS_00429460'
    }
    [pscustomobject]@{
        Address = '00429486'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_00429486.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_00429486_test.cpp'
        PassPattern = 'PASS_00429486'
    }
    [pscustomobject]@{
        Address = '0042a141'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0042a141.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0042a141_test.cpp'
        PassPattern = 'PASS_0042a141'
    }
    [pscustomobject]@{
        Address = '0042a168'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0042a168.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0042a168_test.cpp'
        PassPattern = 'PASS_0042a168'
    }
    [pscustomobject]@{
        Address = '0042a18f'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0042a18f.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0042a18f_test.cpp'
        PassPattern = 'PASS_0042a18f'
    }
    [pscustomobject]@{
        Address = '0042a1e3'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0042a1e3.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0042a1e3_test.cpp'
        PassPattern = 'PASS_0042a1e3'
    }
    [pscustomobject]@{
        Address = '00428ae7'
        Module = '_global'
        Source = 'global_DeleteData_00428ae7.cpp'
        TestSource = 'global_DeleteData_00428ae7_test.cpp'
        PassPattern = 'PASS_00428ae7'
    }
    [pscustomobject]@{
        Address = '0042aab3'
        Module = 'less<CCharString>'
        Source = 'lessCCharString_operator_0042aab3.cpp'
        TestSource = 'lessCCharString_operator_0042aab3_test.cpp'
        PassPattern = 'PASS_0042aab3'
    }
    [pscustomobject]@{
        Address = '0042abe1'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0042abe1.cpp'
        TestSource = 'CTCCarryable_OnKill_0042abe1_test.cpp'
        PassPattern = 'PASS_0042abe1'
    }
    [pscustomobject]@{
        Address = '0042ac76'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0042ac76.cpp'
        TestSource = 'CTCCarryable_OnKill_0042ac76_test.cpp'
        PassPattern = 'PASS_0042ac76'
    }
    [pscustomobject]@{
        Address = '0042acc3'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0042acc3.cpp'
        TestSource = 'CTCCarryable_OnKill_0042acc3_test.cpp'
        PassPattern = 'PASS_0042acc3'
    }
    [pscustomobject]@{
        Address = '0042ade5'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0042ade5.cpp'
        TestSource = 'CTCCarryable_OnKill_0042ade5_test.cpp'
        PassPattern = 'PASS_0042ade5'
    }
    [pscustomobject]@{
        Address = '0042c02f'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0042c02f.cpp'
        TestSource = 'CTCCarryable_OnKill_0042c02f_test.cpp'
        PassPattern = 'PASS_0042c02f'
    }
    [pscustomobject]@{
        Address = '0042c058'
        Module = 'CompareDataOffset'
        Source = 'CompareDataOffset_operator_0042c058.cpp'
        TestSource = 'CompareDataOffset_operator_0042c058_test.cpp'
        PassPattern = 'PASS_0042c058'
    }
    [pscustomobject]@{
        Address = '0042b6dc'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042b6dc.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042b6dc_test.cpp'
        PassPattern = 'PASS_0042b6dc'
    }
    [pscustomobject]@{
        Address = '00429160'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429160.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429160_test.cpp'
        PassPattern = 'PASS_00429160'
    }
    [pscustomobject]@{
        Address = '0042918c'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042918c.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042918c_test.cpp'
        PassPattern = 'PASS_0042918c'
    }
    [pscustomobject]@{
        Address = '004291de'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004291de.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004291de_test.cpp'
        PassPattern = 'PASS_004291de'
    }
    [pscustomobject]@{
        Address = '00429230'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429230.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429230_test.cpp'
        PassPattern = 'PASS_00429230'
    }
    [pscustomobject]@{
        Address = '0042925c'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042925c.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042925c_test.cpp'
        PassPattern = 'PASS_0042925c'
    }
    [pscustomobject]@{
        Address = '0042a1b6'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0042a1b6.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0042a1b6_test.cpp'
        PassPattern = 'PASS_0042a1b6'
    }
    [pscustomobject]@{
        Address = '0042a20a'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0042a20a.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0042a20a_test.cpp'
        PassPattern = 'PASS_0042a20a'
    }
    [pscustomobject]@{
        Address = '0042a237'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0042a237.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0042a237_test.cpp'
        PassPattern = 'PASS_0042a237'
    }
    [pscustomobject]@{
        Address = '0042a264'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0042a264.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0042a264_test.cpp'
        PassPattern = 'PASS_0042a264'
    }
    [pscustomobject]@{
        Address = '0042ac25'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0042ac25.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0042ac25_test.cpp'
        PassPattern = 'PASS_0042ac25'
    }
    [pscustomobject]@{
        Address = '0042a6c3'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a6c3.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a6c3_test.cpp'
        PassPattern = 'PASS_0042a6c3'
    }
    [pscustomobject]@{
        Address = '0042a6f2'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a6f2.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a6f2_test.cpp'
        PassPattern = 'PASS_0042a6f2'
    }
    [pscustomobject]@{
        Address = '0042a721'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a721.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a721_test.cpp'
        PassPattern = 'PASS_0042a721'
    }
    [pscustomobject]@{
        Address = '0042a750'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a750.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a750_test.cpp'
        PassPattern = 'PASS_0042a750'
    }
    [pscustomobject]@{
        Address = '0042a7a1'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a7a1.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a7a1_test.cpp'
        PassPattern = 'PASS_0042a7a1'
    }
    [pscustomobject]@{
        Address = '0042b4c3'
        Module = '_Insert<std::_List_const_iterator<std::_List_val<CClass*,std'
        Source = 'InsertstdListconstiteratorstdListvalCClassstd_allocatorCClass_0042b4c3.cpp'
        TestSource = 'InsertstdListconstiteratorstdListvalCClassstd_allocatorCClass_0042b4c3_test.cpp'
        PassPattern = 'PASS_0042b4c3'
    }
    [pscustomobject]@{
        Address = '0042bf35'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_0042bf35.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_0042bf35_test.cpp'
        PassPattern = 'PASS_0042bf35'
    }
    [pscustomobject]@{
        Address = '00429a2f'
        Module = 'CAnimFilterNoGroup'
        Source = 'CAnimFilterNoGroup_Clone_00429a2f.cpp'
        TestSource = 'CAnimFilterNoGroup_Clone_00429a2f_test.cpp'
        PassPattern = 'PASS_00429a2f'
    }
    [pscustomobject]@{
        Address = '00429a85'
        Module = 'list<NUISystem::CList*'
        Source = 'listNUISystemCList_begin_00429a85.cpp'
        TestSource = 'listNUISystemCList_begin_00429a85_test.cpp'
        PassPattern = 'PASS_00429a85'
    }
    [pscustomobject]@{
        Address = '00429288'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429288.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00429288_test.cpp'
        PassPattern = 'PASS_00429288'
    }
    [pscustomobject]@{
        Address = '004299a8'
        Module = '_global'
        Source = 'global_SetIsUsingRelativeCoords_004299a8.cpp'
        TestSource = 'global_SetIsUsingRelativeCoords_004299a8_test.cpp'
        PassPattern = 'PASS_004299a8'
    }
    [pscustomobject]@{
        Address = '0041d8a8'
        Module = 'CTCBoastingCrowdControl'
        Source = 'CTCBoastingCrowdControl_OnKillCrowd_0041d8a8.cpp'
        TestSource = 'CTCBoastingCrowdControl_OnKillCrowd_0041d8a8_test.cpp'
        PassPattern = 'PASS_0041d8a8'
    }
    [pscustomobject]@{
        Address = '0042951b'
        Module = 'CMemoryAllocatorVariableSize'
        Source = 'CMemoryAllocatorVariableSize_RotateLeft_0042951b.cpp'
        TestSource = 'CMemoryAllocatorVariableSize_RotateLeft_0042951b_test.cpp'
        PassPattern = 'PASS_0042951b'
    }
    [pscustomobject]@{
        Address = '00429c6f'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_00429c6f.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_00429c6f_test.cpp'
        PassPattern = 'PASS_00429c6f'
    }
    [pscustomobject]@{
        Address = '00429cfb'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429cfb.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_00429cfb_test.cpp'
        PassPattern = 'PASS_00429cfb'
    }
    [pscustomobject]@{
        Address = '0042d201'
        Module = 'map<NUISystem::CManager::EUsedKeys'
        Source = 'mapNUISystemCManagerEUsedKeys_operator_0042d201.cpp'
        TestSource = 'mapNUISystemCManagerEUsedKeys_operator_0042d201_test.cpp'
        PassPattern = 'PASS_0042d201'
    }
    [pscustomobject]@{
        Address = '0042d246'
        Module = 'map<NUISystem::CManager::EUsedKeys'
        Source = 'mapNUISystemCManagerEUsedKeys_operator_0042d246.cpp'
        TestSource = 'mapNUISystemCManagerEUsedKeys_operator_0042d246_test.cpp'
        PassPattern = 'PASS_0042d246'
    }
    [pscustomobject]@{
        Address = '0042b945'
        Module = 'CEngineSceneGrid'
        Source = 'CEngineSceneGrid_GetCellBoundingBox_0042b945.cpp'
        TestSource = 'CEngineSceneGrid_GetCellBoundingBox_0042b945_test.cpp'
        PassPattern = 'PASS_0042b945'
    }
    [pscustomobject]@{
        Address = '0042b402'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042b402.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042b402_test.cpp'
        PassPattern = 'PASS_0042b402'
    }
    [pscustomobject]@{
        Address = '0042b8fc'
        Module = '_global'
        Source = 'global_ClearPrimitiveList_0042b8fc.cpp'
        TestSource = 'global_ClearPrimitiveList_0042b8fc_test.cpp'
        PassPattern = 'PASS_0042b8fc'
    }
    [pscustomobject]@{
        Address = '0042d1b7'
        Module = 'map<NUISystem::CComponent*'
        Source = 'mapNUISystemCComponent_operator_0042d1b7.cpp'
        TestSource = 'mapNUISystemCComponent_operator_0042d1b7_test.cpp'
        PassPattern = 'PASS_0042d1b7'
    }
    [pscustomobject]@{
        Address = '00429950'
        Module = 'CBasicString<char>'
        Source = 'CBasicStringchar_operator_00429950.cpp'
        TestSource = 'CBasicStringchar_operator_00429950_test.cpp'
        PassPattern = 'PASS_00429950'
    }
    [pscustomobject]@{
        Address = '00429f43'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00429f43.cpp'
        TestSource = 'CTCMapwho_OnDie_00429f43_test.cpp'
        PassPattern = 'PASS_00429f43'
    }
    [pscustomobject]@{
        Address = '0042a7d0'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a7d0.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042a7d0_test.cpp'
        PassPattern = 'PASS_0042a7d0'
    }
    [pscustomobject]@{
        Address = '0041e16e'
        Module = 'CManager'
        Source = 'CManager_IsKeyHeld_0041e16e.cpp'
        TestSource = 'CManager_IsKeyHeld_0041e16e_test.cpp'
        PassPattern = 'PASS_0041e16e'
    }
    [pscustomobject]@{
        Address = '0042d131'
        Module = 'map<CCharString'
        Source = 'mapCCharString_operator_0042d131.cpp'
        TestSource = 'mapCCharString_operator_0042d131_test.cpp'
        PassPattern = 'PASS_0042d131'
    }
    [pscustomobject]@{
        Address = '0042da80'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_PostTransfer_0042da80.cpp'
        TestSource = 'CDefClassBase_PostTransfer_0042da80_test.cpp'
        PassPattern = 'PASS_0042da80'
    }
    [pscustomobject]@{
        Address = '0042dc5f'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_OnPreDeviceReset_0042dc5f.cpp'
        TestSource = 'CNewFrontendGameComponent_OnPreDeviceReset_0042dc5f_test.cpp'
        PassPattern = 'PASS_0042dc5f'
    }
    [pscustomobject]@{
        Address = '0043368b'
        Module = 'CStartupDefinitionManager'
        Source = 'CStartupDefinitionManager_InitAndCompile_0043368b.cpp'
        TestSource = 'CStartupDefinitionManager_InitAndCompile_0043368b_test.cpp'
        PassPattern = 'PASS_0043368b'
    }
    [pscustomobject]@{
        Address = '0042da40'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_RemoveSubDef_0042da40.cpp'
        TestSource = 'CDefClassBase_RemoveSubDef_0042da40_test.cpp'
        PassPattern = 'PASS_0042da40'
    }
    [pscustomobject]@{
        Address = '0042da90'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_Validate_0042da90.cpp'
        TestSource = 'CDefClassBase_Validate_0042da90_test.cpp'
        PassPattern = 'PASS_0042da90'
    }
    [pscustomobject]@{
        Address = '0042dc5c'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_ChangeTextureColourDepth_0042dc5c.cpp'
        TestSource = 'CNewFrontendGameComponent_ChangeTextureColourDepth_0042dc5c_test.cpp'
        PassPattern = 'PASS_0042dc5c'
    }
    [pscustomobject]@{
        Address = '0042dc60'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_OnPostDeviceReset_0042dc60.cpp'
        TestSource = 'CNewFrontendGameComponent_OnPostDeviceReset_0042dc60_test.cpp'
        PassPattern = 'PASS_0042dc60'
    }
    [pscustomobject]@{
        Address = '004303d0'
        Module = 'CParentDefClassBase'
        Source = 'CParentDefClassBase_IsParentDef_004303d0.cpp'
        TestSource = 'CParentDefClassBase_IsParentDef_004303d0_test.cpp'
        PassPattern = 'PASS_004303d0'
    }
    [pscustomobject]@{
        Address = '004303e0'
        Module = 'CParentDefClassBase'
        Source = 'CParentDefClassBase_GetPSubDefInfoMap_004303e0.cpp'
        TestSource = 'CParentDefClassBase_GetPSubDefInfoMap_004303e0_test.cpp'
        PassPattern = 'PASS_004303e0'
    }
    [pscustomobject]@{
        Address = '0042da50'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_GetPSubDefInfo_0042da50.cpp'
        TestSource = 'CDefClassBase_GetPSubDefInfo_0042da50_test.cpp'
        PassPattern = 'PASS_0042da50'
    }
    [pscustomobject]@{
        Address = '0042da60'
        Module = 'CDefClassBase'
        Source = 'CDefClassBase_GetPSubDefInfo_0042da60.cpp'
        TestSource = 'CDefClassBase_GetPSubDefInfo_0042da60_test.cpp'
        PassPattern = 'PASS_0042da60'
    }
    [pscustomobject]@{
        Address = '00430340'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_SetQuit_00430340.cpp'
        TestSource = 'CNewFrontendGameComponent_SetQuit_00430340_test.cpp'
        PassPattern = 'PASS_00430340'
    }
    [pscustomobject]@{
        Address = '00431e70'
        Module = 'CControlsDef'
        Source = 'CControlsDef_Copy_00431e70.cpp'
        TestSource = 'CControlsDef_Copy_00431e70_test.cpp'
        PassPattern = 'PASS_00431e70'
    }
    [pscustomobject]@{
        Address = '00432e50'
        Module = 'CFrontEndDef'
        Source = 'CFrontEndDef_CopyCFrontEndDefUAEXPBVCDefClassBase_00432e50.cpp'
        TestSource = 'CFrontEndDef_CopyCFrontEndDefUAEXPBVCDefClassBase_00432e50_test.cpp'
        PassPattern = 'PASS_00432e50'
    }
    [pscustomobject]@{
        Address = '00434360'
        Module = 'CEngineDef'
        Source = 'CEngineDef_Copy_00434360.cpp'
        TestSource = 'CEngineDef_Copy_00434360_test.cpp'
        PassPattern = 'PASS_00434360'
    }
    [pscustomobject]@{
        Address = '00434440'
        Module = 'CEngineVideoOptionsDef'
        Source = 'CEngineVideoOptionsDef_CopyCEngineVideoOptionsDefUAEXPBVCDe_00434440.cpp'
        TestSource = 'CEngineVideoOptionsDef_CopyCEngineVideoOptionsDefUAEXPBVCDe_00434440_test.cpp'
        PassPattern = 'PASS_00434440'
    }
    [pscustomobject]@{
        Address = '00434590'
        Module = 'CConfigOptionsDefaultsDef'
        Source = 'CConfigOptionsDefaultsDef_CopyCConfigOptionsDefaultsDefUAEXPBV_00434590.cpp'
        TestSource = 'CConfigOptionsDefaultsDef_CopyCConfigOptionsDefaultsDefUAEXPBV_00434590_test.cpp'
        PassPattern = 'PASS_00434590'
    }
    [pscustomobject]@{
        Address = '00430470'
        Module = 'CControlsDef'
        Source = 'CControlsDef_GetSizeofClass_00430470.cpp'
        TestSource = 'CControlsDef_GetSizeofClass_00430470_test.cpp'
        PassPattern = 'PASS_00430470'
    }
    [pscustomobject]@{
        Address = '00431e30'
        Module = 'CFrontEndDef'
        Source = 'CFrontEndDef_GetSizeofClass_00431e30.cpp'
        TestSource = 'CFrontEndDef_GetSizeofClass_00431e30_test.cpp'
        PassPattern = 'PASS_00431e30'
    }
    [pscustomobject]@{
        Address = '0043368d'
        Module = '_global'
        Source = 'global_GFGetPreMainMemoryUsed_0043368d.cpp'
        TestSource = 'global_GFGetPreMainMemoryUsed_0043368d_test.cpp'
        PassPattern = 'PASS_0043368d'
    }
    [pscustomobject]@{
        Address = '00433fd0'
        Module = 'CEngineDef'
        Source = 'CEngineDef_GetSizeofClass_00433fd0.cpp'
        TestSource = 'CEngineDef_GetSizeofClass_00433fd0_test.cpp'
        PassPattern = 'PASS_00433fd0'
    }
    [pscustomobject]@{
        Address = '00434110'
        Module = 'CEngineVideoOptionsDef'
        Source = 'CEngineVideoOptionsDef_GetSizeofClass_00434110.cpp'
        TestSource = 'CEngineVideoOptionsDef_GetSizeofClass_00434110_test.cpp'
        PassPattern = 'PASS_00434110'
    }
    [pscustomobject]@{
        Address = '00434280'
        Module = 'CConfigOptionsDefaultsDef'
        Source = 'CConfigOptionsDefaultsDef_GetSizeofClass_00434280.cpp'
        TestSource = 'CConfigOptionsDefaultsDef_GetSizeofClass_00434280_test.cpp'
        PassPattern = 'PASS_00434280'
    }
    [pscustomobject]@{
        Address = '004349e0'
        Module = 'CTCInventoryBase'
        Source = 'CTCInventoryBase_GetCanItemBeQuickAssigned_004349e0.cpp'
        TestSource = 'CTCInventoryBase_GetCanItemBeQuickAssigned_004349e0_test.cpp'
        PassPattern = 'PASS_004349e0'
    }
    [pscustomobject]@{
        Address = '004349f0'
        Module = 'CMap'
        Source = 'CMap_DrawGetSeaLevel_004349f0.cpp'
        TestSource = 'CMap_DrawGetSeaLevel_004349f0_test.cpp'
        PassPattern = 'PASS_004349f0'
    }
    [pscustomobject]@{
        Address = '00437570'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437570.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437570_test.cpp'
        PassPattern = 'PASS_00437570'
    }
    [pscustomobject]@{
        Address = '00434a00'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00434a00.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00434a00_test.cpp'
        PassPattern = 'PASS_00434a00'
    }
    [pscustomobject]@{
        Address = '004375e0'
        Module = 'CMap'
        Source = 'CMap_DrawGetSeaLevel_004375e0.cpp'
        TestSource = 'CMap_DrawGetSeaLevel_004375e0_test.cpp'
        PassPattern = 'PASS_004375e0'
    }
    [pscustomobject]@{
        Address = '004303c0'
        Module = 'CParentDefClassBase'
        Source = 'CParentDefClassBase_SetInstantiationName_004303c0.cpp'
        TestSource = 'CParentDefClassBase_SetInstantiationName_004303c0_test.cpp'
        PassPattern = 'PASS_004303c0'
    }
    [pscustomobject]@{
        Address = '00437660'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437660.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437660_test.cpp'
        PassPattern = 'PASS_00437660'
    }
    [pscustomobject]@{
        Address = '00437830'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437830.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437830_test.cpp'
        PassPattern = 'PASS_00437830'
    }
    [pscustomobject]@{
        Address = '0042dbee'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_0042dbee.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_0042dbee_test.cpp'
        PassPattern = 'PASS_0042dbee'
    }
    [pscustomobject]@{
        Address = '004336b0'
        Module = '_global'
        Source = 'global_operatornew_004336b0.cpp'
        TestSource = 'global_operatornew_004336b0_test.cpp'
        PassPattern = 'PASS_004336b0'
    }
    [pscustomobject]@{
        Address = '00437820'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437820.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437820_test.cpp'
        PassPattern = 'PASS_00437820'
    }
    [pscustomobject]@{
        Address = '0043211d'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0043211d.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0043211d_test.cpp'
        PassPattern = 'PASS_0043211d'
    }
    [pscustomobject]@{
        Address = '004374f0'
        Module = 'C3DMeshInfo'
        Source = 'C3DMeshInfo_HasPhysicsMesh_004374f0.cpp'
        TestSource = 'C3DMeshInfo_HasPhysicsMesh_004374f0_test.cpp'
        PassPattern = 'PASS_004374f0'
    }
    [pscustomobject]@{
        Address = '0042dec5'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_InitialiseInput_0042dec5.cpp'
        TestSource = 'CNewFrontendGameComponent_InitialiseInput_0042dec5_test.cpp'
        PassPattern = 'PASS_0042dec5'
    }
    [pscustomobject]@{
        Address = '004349d0'
        Module = 'CTCCreditsUI'
        Source = 'CTCCreditsUI_Deactivate_004349d0.cpp'
        TestSource = 'CTCCreditsUI_Deactivate_004349d0_test.cpp'
        PassPattern = 'PASS_004349d0'
    }
    [pscustomobject]@{
        Address = '00437650'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_AddMoralityChange_00437650.cpp'
        TestSource = 'CPlayerGui_AddMoralityChange_00437650_test.cpp'
        PassPattern = 'PASS_00437650'
    }
    [pscustomobject]@{
        Address = '00437470'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_GetCameraSensitivity_00437470.cpp'
        TestSource = 'CUserProfileManager_GetCameraSensitivity_00437470_test.cpp'
        PassPattern = 'PASS_00437470'
    }
    [pscustomobject]@{
        Address = '00437490'
        Module = 'CUserProfileManager'
        Source = 'CUserProfileManager_GetCameraSensitivity_00437490.cpp'
        TestSource = 'CUserProfileManager_GetCameraSensitivity_00437490_test.cpp'
        PassPattern = 'PASS_00437490'
    }
    [pscustomobject]@{
        Address = '004374b0'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_Open_004374b0.cpp'
        TestSource = 'CPlayerGui_Open_004374b0_test.cpp'
        PassPattern = 'PASS_004374b0'
    }
    [pscustomobject]@{
        Address = '0042dbd8'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_ReleaseXMVCode_0042dbd8.cpp'
        TestSource = 'CNewFrontendGameComponent_ReleaseXMVCode_0042dbd8_test.cpp'
        PassPattern = 'PASS_0042dbd8'
    }
    [pscustomobject]@{
        Address = '0042f70b'
        Module = '_global'
        Source = 'global_CompileDefs_0042f70b.cpp'
        TestSource = 'global_CompileDefs_0042f70b_test.cpp'
        PassPattern = 'PASS_0042f70b'
    }
    [pscustomobject]@{
        Address = '004301f3'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004301f3.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004301f3_test.cpp'
        PassPattern = 'PASS_004301f3'
    }
    [pscustomobject]@{
        Address = '0042fc4e'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042fc4e.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042fc4e_test.cpp'
        PassPattern = 'PASS_0042fc4e'
    }
    [pscustomobject]@{
        Address = '004374d0'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_ForceOpen_004374d0.cpp'
        TestSource = 'CPlayerGui_ForceOpen_004374d0_test.cpp'
        PassPattern = 'PASS_004374d0'
    }
    [pscustomobject]@{
        Address = '0042fe34'
        Module = 'CCountedPointer<CTexture>'
        Source = 'CCountedPointerCTexture_operator_0042fe34.cpp'
        TestSource = 'CCountedPointerCTexture_operator_0042fe34_test.cpp'
        PassPattern = 'PASS_0042fe34'
    }
    [pscustomobject]@{
        Address = '004372a0'
        Module = 'CSubtitleRenderer'
        Source = 'CSubtitleRenderer_GetTextWidthFromText_004372a0.cpp'
        TestSource = 'CSubtitleRenderer_GetTextWidthFromText_004372a0_test.cpp'
        PassPattern = 'PASS_004372a0'
    }
    [pscustomobject]@{
        Address = '00437800'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437800.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTypeCParticleMorphsCEntrystdpairenumEHeroMorphTypeclassCParticleMorphs_CEntryconst_00437800_test.cpp'
        PassPattern = 'PASS_00437800'
    }
    [pscustomobject]@{
        Address = '004330d2'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004330d2.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004330d2_test.cpp'
        PassPattern = 'PASS_004330d2'
    }
    [pscustomobject]@{
        Address = '004354a0'
        Module = 'CDisplayEngine'
        Source = 'CDisplayEngine_UninitEngineCDisplayEngineMAEXXZ_004354a0.cpp'
        TestSource = 'CDisplayEngine_UninitEngineCDisplayEngineMAEXXZ_004354a0_test.cpp'
        PassPattern = 'PASS_004354a0'
    }
    [pscustomobject]@{
        Address = '0042db20'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042db20.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042db20_test.cpp'
        PassPattern = 'PASS_0042db20'
    }
    [pscustomobject]@{
        Address = '00430345'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_00430345.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_00430345_test.cpp'
        PassPattern = 'PASS_00430345'
    }
    [pscustomobject]@{
        Address = '00430480'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00430480.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00430480_test.cpp'
        PassPattern = 'PASS_00430480'
    }
    [pscustomobject]@{
        Address = '004317b0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004317b0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004317b0_test.cpp'
        PassPattern = 'PASS_004317b0'
    }
    [pscustomobject]@{
        Address = '00431dd0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00431dd0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00431dd0_test.cpp'
        PassPattern = 'PASS_00431dd0'
    }
    [pscustomobject]@{
        Address = '004340b0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004340b0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004340b0_test.cpp'
        PassPattern = 'PASS_004340b0'
    }
    [pscustomobject]@{
        Address = '00434220'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00434220.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00434220_test.cpp'
        PassPattern = 'PASS_00434220'
    }
    [pscustomobject]@{
        Address = '00434340'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00434340.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00434340_test.cpp'
        PassPattern = 'PASS_00434340'
    }
    [pscustomobject]@{
        Address = '00437020'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00437020.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00437020_test.cpp'
        PassPattern = 'PASS_00437020'
    }
    [pscustomobject]@{
        Address = '004371f0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004371f0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004371f0_test.cpp'
        PassPattern = 'PASS_004371f0'
    }
    [pscustomobject]@{
        Address = '0042fc66'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042fc66.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042fc66_test.cpp'
        PassPattern = 'PASS_0042fc66'
    }
    [pscustomobject]@{
        Address = '00433d19'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00433d19.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00433d19_test.cpp'
        PassPattern = 'PASS_00433d19'
    }
    [pscustomobject]@{
        Address = '00437040'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00437040.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00437040_test.cpp'
        PassPattern = 'PASS_00437040'
    }
    [pscustomobject]@{
        Address = '0042dab0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042dab0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042dab0_test.cpp'
        PassPattern = 'PASS_0042dab0'
    }
    [pscustomobject]@{
        Address = '004368a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004368a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004368a0_test.cpp'
        PassPattern = 'PASS_004368a0'
    }
    [pscustomobject]@{
        Address = '00437160'
        Module = 'CDefaultEngineMemoryInit'
        Source = 'CDefaultEngineMemoryInit_ApplyCustomValue_00437160.cpp'
        TestSource = 'CDefaultEngineMemoryInit_ApplyCustomValue_00437160_test.cpp'
        PassPattern = 'PASS_00437160'
    }
    [pscustomobject]@{
        Address = '0042dae0'
        Module = 'CEnginePrimitiveOcclusionManager'
        Source = 'CEnginePrimitiveOcclusionManager_GetOccludedFrames_0042dae0.cpp'
        TestSource = 'CEnginePrimitiveOcclusionManager_GetOccludedFrames_0042dae0_test.cpp'
        PassPattern = 'PASS_0042dae0'
    }
    [pscustomobject]@{
        Address = '004331c0'
        Module = 'CFrontEndDef'
        Source = 'CFrontEndDef_TransferCFrontEndDefUAEXAAVCPersistC_004331c0.cpp'
        TestSource = 'CFrontEndDef_TransferCFrontEndDefUAEXAAVCPersistC_004331c0_test.cpp'
        PassPattern = 'PASS_004331c0'
    }
    [pscustomobject]@{
        Address = '0042fc13'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_0042fc13.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_0042fc13_test.cpp'
        PassPattern = 'PASS_0042fc13'
    }
    [pscustomobject]@{
        Address = '00433d3a'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_00433d3a.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_00433d3a_test.cpp'
        PassPattern = 'PASS_00433d3a'
    }
    [pscustomobject]@{
        Address = '00434c90'
        Module = 'CDisplayEngine'
        Source = 'CDisplayEngine_EndScreenFadeOut_00434c90.cpp'
        TestSource = 'CDisplayEngine_EndScreenFadeOut_00434c90_test.cpp'
        PassPattern = 'PASS_00434c90'
    }
    [pscustomobject]@{
        Address = '0042f722'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_InitialiseDefs_0042f722.cpp'
        TestSource = 'CNewFrontendGameComponent_InitialiseDefs_0042f722_test.cpp'
        PassPattern = 'PASS_0042f722'
    }
    [pscustomobject]@{
        Address = '0043332c'
        Module = 'CArray<std::pair<long'
        Source = 'CArraystdpairlong_pushback_0043332c.cpp'
        TestSource = 'CArraystdpairlong_pushback_0043332c_test.cpp'
        PassPattern = 'PASS_0043332c'
    }
    [pscustomobject]@{
        Address = '00430300'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00430300.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00430300_test.cpp'
        PassPattern = 'PASS_00430300'
    }
    [pscustomobject]@{
        Address = '00431020'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00431020.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00431020_test.cpp'
        PassPattern = 'PASS_00431020'
    }
    [pscustomobject]@{
        Address = '00431102'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00431102.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00431102_test.cpp'
        PassPattern = 'PASS_00431102'
    }
    [pscustomobject]@{
        Address = '0043119e'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_0043119e.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_0043119e_test.cpp'
        PassPattern = 'PASS_0043119e'
    }
    [pscustomobject]@{
        Address = '0042db60'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042db60.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0042db60_test.cpp'
        PassPattern = 'PASS_0042db60'
    }
    [pscustomobject]@{
        Address = '0043314a'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_0043314a.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_0043314a_test.cpp'
        PassPattern = 'PASS_0043314a'
    }
    [pscustomobject]@{
        Address = '0043020a'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0043020a.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0043020a_test.cpp'
        PassPattern = 'PASS_0043020a'
    }
    [pscustomobject]@{
        Address = '00436560'
        Module = 'CTCHeroStats'
        Source = 'CTCHeroStats_SoberUp_00436560.cpp'
        TestSource = 'CTCHeroStats_SoberUp_00436560_test.cpp'
        PassPattern = 'PASS_00436560'
    }
    [pscustomobject]@{
        Address = '00431061'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00431061.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00431061_test.cpp'
        PassPattern = 'PASS_00431061'
    }
    [pscustomobject]@{
        Address = '00433227'
        Module = '_global'
        Source = 'global_GFSerialiseVectorBinaryOutCCharStrin_00433227.cpp'
        TestSource = 'global_GFSerialiseVectorBinaryOutCCharStrin_00433227_test.cpp'
        PassPattern = 'PASS_00433227'
    }
    [pscustomobject]@{
        Address = '00437210'
        Module = 'CFontBank'
        Source = 'CFontBank_GetVStringWidth_00437210.cpp'
        TestSource = 'CFontBank_GetVStringWidth_00437210_test.cpp'
        PassPattern = 'PASS_00437210'
    }
    [pscustomobject]@{
        Address = '00431f10'
        Module = 'CThingBuildingDef'
        Source = 'CThingBuildingDef_operator_00431f10.cpp'
        TestSource = 'CThingBuildingDef_operator_00431f10_test.cpp'
        PassPattern = 'PASS_00431f10'
    }
    [pscustomobject]@{
        Address = '004375f0'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_SetVWindowForDeprecatedDrawInterface_004375f0.cpp'
        TestSource = 'CPlayerGui_SetVWindowForDeprecatedDrawInterface_004375f0_test.cpp'
        PassPattern = 'PASS_004375f0'
    }
    [pscustomobject]@{
        Address = '00436a20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436a20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436a20_test.cpp'
        PassPattern = 'PASS_00436a20'
    }
    [pscustomobject]@{
        Address = '00437580'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_SetSpecialAbilityButtonsAsDisplaying_00437580.cpp'
        TestSource = 'CPlayerGui_SetSpecialAbilityButtonsAsDisplaying_00437580_test.cpp'
        PassPattern = 'PASS_00437580'
    }
    [pscustomobject]@{
        Address = '0042f9a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f9a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f9a0_test.cpp'
        PassPattern = 'PASS_0042f9a0'
    }
    [pscustomobject]@{
        Address = '0043027a'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0043027a.cpp'
        TestSource = 'CTCCarryable_OnKill_0043027a_test.cpp'
        PassPattern = 'PASS_0043027a'
    }
    [pscustomobject]@{
        Address = '00434ba0'
        Module = 'CDisplayEngine'
        Source = 'CDisplayEngine_StartLetterBoxMode_00434ba0.cpp'
        TestSource = 'CDisplayEngine_StartLetterBoxMode_00434ba0_test.cpp'
        PassPattern = 'PASS_00434ba0'
    }
    [pscustomobject]@{
        Address = '00434d60'
        Module = 'C3DAnimation2'
        Source = 'C3DAnimation2_CanRelocateData_00434d60.cpp'
        TestSource = 'C3DAnimation2_CanRelocateData_00434d60_test.cpp'
        PassPattern = 'PASS_00434d60'
    }
    [pscustomobject]@{
        Address = '004330f0'
        Module = 'CControlsDef'
        Source = 'CControlsDef_TransferCControlsDefUAEXAAVCPersistC_004330f0.cpp'
        TestSource = 'CControlsDef_TransferCControlsDefUAEXAAVCPersistC_004330f0_test.cpp'
        PassPattern = 'PASS_004330f0'
    }
    [pscustomobject]@{
        Address = '00435070'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_DisplayTutorial_00435070.cpp'
        TestSource = 'CGameScriptInterface_DisplayTutorial_00435070_test.cpp'
        PassPattern = 'PASS_00435070'
    }
    [pscustomobject]@{
        Address = '00434870'
        Module = 'CFadeInFadeOutBase'
        Source = 'CFadeInFadeOutBase_Update_00434870.cpp'
        TestSource = 'CFadeInFadeOutBase_Update_00434870_test.cpp'
        PassPattern = 'PASS_00434870'
    }
    [pscustomobject]@{
        Address = '00434a30'
        Module = 'CWorld'
        Source = 'CWorld_ProcessEvent_00434a30.cpp'
        TestSource = 'CWorld_ProcessEvent_00434a30_test.cpp'
        PassPattern = 'batch8_part3_00434a30_pass'
    }
    [pscustomobject]@{
        Address = '00433504'
        Module = '_global'
        Source = 'global_Fillnunsignedlongunsignedintunsigned_00433504.cpp'
        TestSource = 'global_Fillnunsignedlongunsignedintunsigned_00433504_test.cpp'
        PassPattern = 'batch8_part3_00433504_pass'
    }
    [pscustomobject]@{
        Address = '0042dbb0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042dbb0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3DPrimitive_CTriangleList_0042dbb0_test.cpp'
        PassPattern = 'batch8_part3_0042dbb0_pass'
    }
    [pscustomobject]@{
        Address = '00430046'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00430046.cpp'
        TestSource = 'CTCCarryable_OnKill_00430046_test.cpp'
        PassPattern = 'batch8_part3_00430046_pass'
    }
    [pscustomobject]@{
        Address = '00433d8a'
        Module = '_Uninit_copy<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMorph::CBodyPartMesh>*,std::allocator<CArray<CRandomAppearanceMorph'
        Source = 'UninitcopyCArrayCRandomAppearanceMorphCBodyPartMeshCArrayclassCRandomAppearanceMorphCBodyPartMeshstdallocatorCArrayCRandomAppearanceMorph_CBodyPartMesh_00433d8a.cpp'
        TestSource = 'UninitcopyCArrayCRandomAppearanceMorphCBodyPartMeshCArrayclassCRandomAppearanceMorphCBodyPartMeshstdallocatorCArrayCRandomAppearanceMorph_CBodyPartMesh_00433d8a_test.cpp'
        PassPattern = 'batch8_part3_00433d8a_pass'
    }
    [pscustomobject]@{
        Address = '0042f948'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f948.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f948_test.cpp'
        PassPattern = 'batch8_part3_0042f948_pass'
    }
    [pscustomobject]@{
        Address = '0042f974'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f974.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_0042f974_test.cpp'
        PassPattern = 'batch8_part3_0042f974_pass'
    }
    [pscustomobject]@{
        Address = '0043024e'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_0043024e.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_0043024e_test.cpp'
        PassPattern = 'batch8_part3_0043024e_pass'
    }
    [pscustomobject]@{
        Address = '00433cc0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00433cc0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00433cc0_test.cpp'
        PassPattern = 'batch8_part3_00433cc0_pass'
    }
    [pscustomobject]@{
        Address = '00434840'
        Module = 'CEngine'
        Source = 'CEngine_StartNewScene_00434840.cpp'
        TestSource = 'CEngine_StartNewScene_00434840_test.cpp'
        PassPattern = 'batch8_part3_00434840_pass'
    }
    [pscustomobject]@{
        Address = '0042fe07'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0042fe07.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0042fe07_test.cpp'
        PassPattern = 'batch8_part3_0042fe07_pass'
    }
    [pscustomobject]@{
        Address = '00431f60'
        Module = 'CTCPhysicsBase'
        Source = 'CTCPhysicsBase_ConstructFromParams_00431f60.cpp'
        TestSource = 'CTCPhysicsBase_ConstructFromParams_00431f60_test.cpp'
        PassPattern = 'batch8_part3_00431f60_pass'
    }
    [pscustomobject]@{
        Address = '00433cec'
        Module = '_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMorph'
        Source = 'CopyimplCArrayCRandomAppearanceMorphCBodyPartMeshCArrayclassCRandomAppearanceMorph_CBodyPartMesh_00433cec.cpp'
        TestSource = 'CopyimplCArrayCRandomAppearanceMorphCBodyPartMeshCArrayclassCRandomAppearanceMorph_CBodyPartMesh_00433cec_test.cpp'
        PassPattern = 'batch8_part3_00433cec_pass'
    }
    [pscustomobject]@{
        Address = '00431773'
        Module = '_global'
        Source = 'global_Alloc_00431773.cpp'
        TestSource = 'global_Alloc_00431773_test.cpp'
        PassPattern = 'batch8_part3_00431773_pass'
    }
    [pscustomobject]@{
        Address = '00431e40'
        Module = 'CThingTrackNode'
        Source = 'CThingTrackNode_vectordeletingdestructor_00431e40.cpp'
        TestSource = 'CThingTrackNode_vectordeletingdestructor_00431e40_test.cpp'
        PassPattern = 'batch8_part3_00431e40_pass'
    }
    [pscustomobject]@{
        Address = '0043318d'
        Module = 'CCreatureAction_FireMissileWeapon'
        Source = 'CCreatureActionFireMissileWeapon_FrameUpdate_0043318d.cpp'
        TestSource = 'CCreatureActionFireMissileWeapon_FrameUpdate_0043318d_test.cpp'
        PassPattern = 'batch8_part3_0043318d_pass'
    }
    [pscustomobject]@{
        Address = '004331f9'
        Module = 'CCreatureAction_FireMissileWeapon'
        Source = 'CCreatureActionFireMissileWeapon_FrameUpdate_004331f9.cpp'
        TestSource = 'CCreatureActionFireMissileWeapon_FrameUpdate_004331f9_test.cpp'
        PassPattern = 'batch8_part3_004331f9_pass'
    }
    [pscustomobject]@{
        Address = '0042fe4e'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042fe4e.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042fe4e_test.cpp'
        PassPattern = 'batch8_part3_0042fe4e_pass'
    }
    [pscustomobject]@{
        Address = '0042fe7d'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042fe7d.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0042fe7d_test.cpp'
        PassPattern = 'batch8_part3_0042fe7d_pass'
    }
    [pscustomobject]@{
        Address = '0042dc63'
        Module = 'CNewFrontendGameComponent'
        Source = 'CNewFrontendGameComponent_IsControllerDisconnected_0042dc63.cpp'
        TestSource = 'CNewFrontendGameComponent_IsControllerDisconnected_0042dc63_test.cpp'
        PassPattern = 'batch8_part3_0042dc63_pass'
    }
    [pscustomobject]@{
        Address = '004302cd'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004302cd.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004302cd_test.cpp'
        PassPattern = 'batch8_part3_004302cd_pass'
    }
    [pscustomobject]@{
        Address = '004304a0'
        Module = 'CCompressedMemoryDataInputStream'
        Source = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_004304a0.cpp'
        TestSource = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_004304a0_test.cpp'
        PassPattern = 'batch8_part3_004304a0_pass'
    }
    [pscustomobject]@{
        Address = '00431ed0'
        Module = 'CParentDefClassBase'
        Source = 'CParentDefClassBase_operator_00431ed0.cpp'
        TestSource = 'CParentDefClassBase_operator_00431ed0_test.cpp'
        PassPattern = 'batch8_part3_00431ed0_pass'
    }
    [pscustomobject]@{
        Address = '0042f9f2'
        Module = 'CCountedPointer<NUISystem::CAction>'
        Source = 'CCountedPointerNUISystemCAction_ShareData_0042f9f2.cpp'
        TestSource = 'CCountedPointerNUISystemCAction_ShareData_0042f9f2_test.cpp'
        PassPattern = 'batch8_part3_0042f9f2_pass'
    }
    [pscustomobject]@{
        Address = '00437670'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_DrawScreenFade_00437670.cpp'
        TestSource = 'CPlayerGui_DrawScreenFade_00437670_test.cpp'
        PassPattern = 'batch8_part3_00437670_pass'
    }
    [pscustomobject]@{
        Address = '0042fb40'
        Module = 'pair<CCharString'
        Source = 'pairCCharString_swap_0042fb40.cpp'
        TestSource = 'pairCCharString_swap_0042fb40_test.cpp'
        PassPattern = 'batch8_part3_0042fb40_pass'
    }
    [pscustomobject]@{
        Address = '004372c0'
        Module = 'vector<bool'
        Source = 'vectorbool_operator_004372c0.cpp'
        TestSource = 'vectorbool_operator_004372c0_test.cpp'
        PassPattern = 'batch8_part3_004372c0_pass'
    }
    [pscustomobject]@{
        Address = '004364c0'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_004364c0.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusicManager_CRegisteredMusicEntry_004364c0_test.cpp'
        PassPattern = 'batch8_part3_004364c0_pass'
    }
    [pscustomobject]@{
        Address = '00431242'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00431242.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00431242_test.cpp'
        PassPattern = 'batch8_part3_00431242_pass'
    }
    [pscustomobject]@{
        Address = '00431493'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00431493.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00431493_test.cpp'
        PassPattern = 'batch8_part3_00431493_pass'
    }
    [pscustomobject]@{
        Address = '004362c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004362c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_004362c0_test.cpp'
        PassPattern = 'batch8_part3_004362c0_pass'
    }
    [pscustomobject]@{
        Address = '00436300'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436300.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436300_test.cpp'
        PassPattern = 'batch8_part3_00436300_pass'
    }
    [pscustomobject]@{
        Address = '00436340'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436340.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCCountedPointerQuadricOptimiserInternals_COptimisedPrimitive_00436340_test.cpp'
        PassPattern = 'batch8_part3_00436340_pass'
    }
    [pscustomobject]@{
        Address = '00437cd0'
        Module = 'CWorld'
        Source = 'CWorld_DrawGetEnvironment_00437cd0.cpp'
        TestSource = 'CWorld_DrawGetEnvironment_00437cd0_test.cpp'
        PassPattern = 'PASS_00437cd0'
    }
    [pscustomobject]@{
        Address = '00437860'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437860.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437860_test.cpp'
        PassPattern = 'PASS_00437860'
    }
    [pscustomobject]@{
        Address = '004378b0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378b0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378b0_test.cpp'
        PassPattern = 'PASS_004378b0'
    }
    [pscustomobject]@{
        Address = '004378c0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378c0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378c0_test.cpp'
        PassPattern = 'PASS_004378c0'
    }
    [pscustomobject]@{
        Address = '004378d0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378d0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378d0_test.cpp'
        PassPattern = 'PASS_004378d0'
    }
    [pscustomobject]@{
        Address = '004378e0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378e0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378e0_test.cpp'
        PassPattern = 'PASS_004378e0'
    }
    [pscustomobject]@{
        Address = '004378f0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378f0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004378f0_test.cpp'
        PassPattern = 'PASS_004378f0'
    }
    [pscustomobject]@{
        Address = '00437900'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437900.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437900_test.cpp'
        PassPattern = 'PASS_00437900'
    }
    [pscustomobject]@{
        Address = '00437910'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437910.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437910_test.cpp'
        PassPattern = 'PASS_00437910'
    }
    [pscustomobject]@{
        Address = '00437940'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437940.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437940_test.cpp'
        PassPattern = 'PASS_00437940'
    }
    [pscustomobject]@{
        Address = '00437980'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437980.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437980_test.cpp'
        PassPattern = 'PASS_00437980'
    }
    [pscustomobject]@{
        Address = '00437990'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437990.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437990_test.cpp'
        PassPattern = 'PASS_00437990'
    }
    [pscustomobject]@{
        Address = '004379a0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379a0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379a0_test.cpp'
        PassPattern = 'PASS_004379a0'
    }
    [pscustomobject]@{
        Address = '004379b0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379b0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379b0_test.cpp'
        PassPattern = 'PASS_004379b0'
    }
    [pscustomobject]@{
        Address = '004379c0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379c0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379c0_test.cpp'
        PassPattern = 'PASS_004379c0'
    }
    [pscustomobject]@{
        Address = '004379d0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379d0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379d0_test.cpp'
        PassPattern = 'PASS_004379d0'
    }
    [pscustomobject]@{
        Address = '004379e0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379e0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_004379e0_test.cpp'
        PassPattern = 'PASS_004379e0'
    }
    [pscustomobject]@{
        Address = '00437a00'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437a00.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437a00_test.cpp'
        PassPattern = 'PASS_00437a00'
    }
    [pscustomobject]@{
        Address = '00437d00'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437d00.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437d00_test.cpp'
        PassPattern = 'PASS_00437d00'
    }
    [pscustomobject]@{
        Address = '00437ff0'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437ff0.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437ff0_test.cpp'
        PassPattern = 'PASS_00437ff0'
    }
    [pscustomobject]@{
        Address = '00438000'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00438000.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00438000_test.cpp'
        PassPattern = 'PASS_00438000'
    }
    [pscustomobject]@{
        Address = '00438010'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00438010.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00438010_test.cpp'
        PassPattern = 'PASS_00438010'
    }
    [pscustomobject]@{
        Address = '004379f0'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_HideScreenshotMessage_004379f0.cpp'
        TestSource = 'CPlayerGui_HideScreenshotMessage_004379f0_test.cpp'
        PassPattern = 'PASS_004379f0'
    }
    [pscustomobject]@{
        Address = '00437920'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_AddMoralityChange_00437920.cpp'
        TestSource = 'CPlayerGui_AddMoralityChange_00437920_test.cpp'
        PassPattern = 'PASS_00437920'
    }
    [pscustomobject]@{
        Address = '00437930'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_AddMoralityChange_00437930.cpp'
        TestSource = 'CPlayerGui_AddMoralityChange_00437930_test.cpp'
        PassPattern = 'PASS_00437930'
    }
    [pscustomobject]@{
        Address = '0043dbd0'
        Module = '_global'
        Source = 'global_ISTHINGALIVECThingBuilding_0043dbd0.cpp'
        TestSource = 'global_ISTHINGALIVECThingBuilding_0043dbd0_test.cpp'
        PassPattern = 'PASS_0043dbd0'
    }
    [pscustomobject]@{
        Address = '00437a10'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437a10.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437a10_test.cpp'
        PassPattern = 'PASS_00437a10'
    }
    [pscustomobject]@{
        Address = '00437ce0'
        Module = 'CVertexBufferWin32'
        Source = 'CVertexBufferWin32_DoSizeof_00437ce0.cpp'
        TestSource = 'CVertexBufferWin32_DoSizeof_00437ce0_test.cpp'
        PassPattern = 'PASS_00437ce0'
    }
    [pscustomobject]@{
        Address = '0043eea0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0043eea0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0043eea0_test.cpp'
        PassPattern = 'PASS_0043eea0'
    }
    [pscustomobject]@{
        Address = '00437840'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437840.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437840_test.cpp'
        PassPattern = 'PASS_00437840'
    }
    [pscustomobject]@{
        Address = '00438440'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_ForceClose_00438440.cpp'
        TestSource = 'CPlayerGui_ForceClose_00438440_test.cpp'
        PassPattern = 'PASS_00438440'
    }
    [pscustomobject]@{
        Address = '004382c0'
        Module = 'CWorldMap'
        Source = 'CWorldMap_PrepareForLoad_004382c0.cpp'
        TestSource = 'CWorldMap_PrepareForLoad_004382c0_test.cpp'
        PassPattern = 'PASS_004382c0'
    }
    [pscustomobject]@{
        Address = '0043e390'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e390.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e390_test.cpp'
        PassPattern = 'PASS_0043e390'
    }
    [pscustomobject]@{
        Address = '0043e3b0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e3b0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e3b0_test.cpp'
        PassPattern = 'PASS_0043e3b0'
    }
    [pscustomobject]@{
        Address = '0043f700'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f700.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f700_test.cpp'
        PassPattern = 'PASS_0043f700'
    }
    [pscustomobject]@{
        Address = '00437950'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_OnHeroHit_00437950.cpp'
        TestSource = 'CPlayerGui_OnHeroHit_00437950_test.cpp'
        PassPattern = 'PASS_00437950'
    }
    [pscustomobject]@{
        Address = '0043f720'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f720.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f720_test.cpp'
        PassPattern = 'PASS_0043f720'
    }
    [pscustomobject]@{
        Address = '00437880'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437880.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00437880_test.cpp'
        PassPattern = 'PASS_00437880'
    }
    [pscustomobject]@{
        Address = '0043e520'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e520.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043e520_test.cpp'
        PassPattern = 'PASS_0043e520'
    }
    [pscustomobject]@{
        Address = '0043c3c0'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_0043c3c0.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_0043c3c0_test.cpp'
        PassPattern = 'PASS_0043c3c0'
    }
    [pscustomobject]@{
        Address = '0043dd10'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_0043dd10.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_0043dd10_test.cpp'
        PassPattern = 'PASS_0043dd10'
    }
    [pscustomobject]@{
        Address = '004382e0'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_SetButtonAlpha_004382e0.cpp'
        TestSource = 'CPlayerGui_SetButtonAlpha_004382e0_test.cpp'
        PassPattern = 'PASS_004382e0'
    }
    [pscustomobject]@{
        Address = '0043eec0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0043eec0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0043eec0_test.cpp'
        PassPattern = 'PASS_0043eec0'
    }
    [pscustomobject]@{
        Address = '004383d0'
        Module = 'CGameScriptInterface'
        Source = 'CGameScriptInterface_GiveHeroExpression_004383d0.cpp'
        TestSource = 'CGameScriptInterface_GiveHeroExpression_004383d0_test.cpp'
        PassPattern = 'PASS_004383d0'
    }
    [pscustomobject]@{
        Address = '0043f270'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043f270.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043f270_test.cpp'
        PassPattern = 'PASS_0043f270'
    }
    [pscustomobject]@{
        Address = '0043f2a0'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043f2a0.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043f2a0_test.cpp'
        PassPattern = 'PASS_0043f2a0'
    }
    [pscustomobject]@{
        Address = '0043f6c0'
        Module = '_Uninit_fill_n<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>*,unsigned_int,std::pair<unsigned___int64,class_CCountedPointer<class_CActiveEntityScriptBase>_>,std::allocator<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>_>,std'
        Source = 'Uninitfillnstdpairunsignedint64CCountedPointerCActiveEntityScriptBaseunsignedint_pairunsignedint64classCCountedPointerclassCActiv_0043f6c0.cpp'
        TestSource = 'Uninitfillnstdpairunsignedint64CCountedPointerCActiveEntityScriptBaseunsignedint_pairunsignedint64classCCountedPointerclassCActiv_0043f6c0_test.cpp'
        PassPattern = 'PASS_0043f6c0'
    }
    [pscustomobject]@{
        Address = '0043d200'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d200.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d200_test.cpp'
        PassPattern = 'PASS_0043d200'
    }
    [pscustomobject]@{
        Address = '0043d240'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d240.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d240_test.cpp'
        PassPattern = 'PASS_0043d240'
    }
    [pscustomobject]@{
        Address = '0043d280'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d280.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d280_test.cpp'
        PassPattern = 'PASS_0043d280'
    }
    [pscustomobject]@{
        Address = '0043d2c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d2c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d2c0_test.cpp'
        PassPattern = 'PASS_0043d2c0'
    }
    [pscustomobject]@{
        Address = '0043d300'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d300.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d300_test.cpp'
        PassPattern = 'PASS_0043d300'
    }
    [pscustomobject]@{
        Address = '0043d340'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d340.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d340_test.cpp'
        PassPattern = 'PASS_0043d340'
    }
    [pscustomobject]@{
        Address = '0043d380'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d380.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d380_test.cpp'
        PassPattern = 'PASS_0043d380'
    }
    [pscustomobject]@{
        Address = '0043d3c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d3c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d3c0_test.cpp'
        PassPattern = 'PASS_0043d3c0'
    }
    [pscustomobject]@{
        Address = '0043d400'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d400.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d400_test.cpp'
        PassPattern = 'PASS_0043d400'
    }
    [pscustomobject]@{
        Address = '0043d440'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d440.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d440_test.cpp'
        PassPattern = 'PASS_0043d440'
    }
    [pscustomobject]@{
        Address = '0043d480'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d480.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d480_test.cpp'
        PassPattern = 'PASS_0043d480'
    }
    [pscustomobject]@{
        Address = '0043d4c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d4c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d4c0_test.cpp'
        PassPattern = 'PASS_0043d4c0'
    }
    [pscustomobject]@{
        Address = '0043d500'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d500.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d500_test.cpp'
        PassPattern = 'PASS_0043d500'
    }
    [pscustomobject]@{
        Address = '0043d540'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d540.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d540_test.cpp'
        PassPattern = 'PASS_0043d540'
    }
    [pscustomobject]@{
        Address = '0043d580'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d580.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d580_test.cpp'
        PassPattern = 'PASS_0043d580'
    }
    [pscustomobject]@{
        Address = '0043d5c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d5c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d5c0_test.cpp'
        PassPattern = 'PASS_0043d5c0'
    }
    [pscustomobject]@{
        Address = '0043d600'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d600.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d600_test.cpp'
        PassPattern = 'PASS_0043d600'
    }
    [pscustomobject]@{
        Address = '0043d640'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d640.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d640_test.cpp'
        PassPattern = 'PASS_0043d640'
    }
    [pscustomobject]@{
        Address = '0043d680'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d680.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d680_test.cpp'
        PassPattern = 'PASS_0043d680'
    }
    [pscustomobject]@{
        Address = '0043d6c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d6c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d6c0_test.cpp'
        PassPattern = 'PASS_0043d6c0'
    }
    [pscustomobject]@{
        Address = '0043d700'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d700.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d700_test.cpp'
        PassPattern = 'PASS_0043d700'
    }
    [pscustomobject]@{
        Address = '0043d740'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d740.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d740_test.cpp'
        PassPattern = 'PASS_0043d740'
    }
    [pscustomobject]@{
        Address = '0043d780'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d780.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d780_test.cpp'
        PassPattern = 'PASS_0043d780'
    }
    [pscustomobject]@{
        Address = '0043d7c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d7c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d7c0_test.cpp'
        PassPattern = 'PASS_0043d7c0'
    }
    [pscustomobject]@{
        Address = '0043d800'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d800.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d800_test.cpp'
        PassPattern = 'PASS_0043d800'
    }
    [pscustomobject]@{
        Address = '0043d840'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d840.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d840_test.cpp'
        PassPattern = 'PASS_0043d840'
    }
    [pscustomobject]@{
        Address = '0043d880'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d880.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d880_test.cpp'
        PassPattern = 'PASS_0043d880'
    }
    [pscustomobject]@{
        Address = '0043d8c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d8c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d8c0_test.cpp'
        PassPattern = 'PASS_0043d8c0'
    }
    [pscustomobject]@{
        Address = '0043d900'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d900.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d900_test.cpp'
        PassPattern = 'PASS_0043d900'
    }
    [pscustomobject]@{
        Address = '0043d940'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d940.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d940_test.cpp'
        PassPattern = 'PASS_0043d940'
    }
    [pscustomobject]@{
        Address = '0043d980'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d980.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d980_test.cpp'
        PassPattern = 'PASS_0043d980'
    }
    [pscustomobject]@{
        Address = '0043d9c0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d9c0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043d9c0_test.cpp'
        PassPattern = 'PASS_0043d9c0'
    }
    [pscustomobject]@{
        Address = '0043da00'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da00.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da00_test.cpp'
        PassPattern = 'PASS_0043da00'
    }
    [pscustomobject]@{
        Address = '0043da40'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da40.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da40_test.cpp'
        PassPattern = 'PASS_0043da40'
    }
    [pscustomobject]@{
        Address = '0043da80'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da80.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043da80_test.cpp'
        PassPattern = 'PASS_0043da80'
    }
    [pscustomobject]@{
        Address = '0043dac0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043dac0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043dac0_test.cpp'
        PassPattern = 'PASS_0043dac0'
    }
    [pscustomobject]@{
        Address = '0043db00'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043db00.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043db00_test.cpp'
        PassPattern = 'PASS_0043db00'
    }
    [pscustomobject]@{
        Address = '0043e5e0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e5e0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e5e0_test.cpp'
        PassPattern = 'PASS_0043e5e0'
    }
    [pscustomobject]@{
        Address = '0043e620'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e620.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e620_test.cpp'
        PassPattern = 'PASS_0043e620'
    }
    [pscustomobject]@{
        Address = '0043e660'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e660.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e660_test.cpp'
        PassPattern = 'PASS_0043e660'
    }
    [pscustomobject]@{
        Address = '0043e6a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e6a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e6a0_test.cpp'
        PassPattern = 'PASS_0043e6a0'
    }
    [pscustomobject]@{
        Address = '0043e6e0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e6e0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e6e0_test.cpp'
        PassPattern = 'PASS_0043e6e0'
    }
    [pscustomobject]@{
        Address = '0043e720'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e720.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e720_test.cpp'
        PassPattern = 'PASS_0043e720'
    }
    [pscustomobject]@{
        Address = '0043e760'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e760.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e760_test.cpp'
        PassPattern = 'PASS_0043e760'
    }
    [pscustomobject]@{
        Address = '0043e7a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e7a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e7a0_test.cpp'
        PassPattern = 'PASS_0043e7a0'
    }
    [pscustomobject]@{
        Address = '0043e7e0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e7e0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e7e0_test.cpp'
        PassPattern = 'PASS_0043e7e0'
    }
    [pscustomobject]@{
        Address = '0043e820'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e820.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e820_test.cpp'
        PassPattern = 'PASS_0043e820'
    }
    [pscustomobject]@{
        Address = '0043e860'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e860.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e860_test.cpp'
        PassPattern = 'PASS_0043e860'
    }
    [pscustomobject]@{
        Address = '0043e8a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e8a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e8a0_test.cpp'
        PassPattern = 'PASS_0043e8a0'
    }
    [pscustomobject]@{
        Address = '0043e8e0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e8e0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e8e0_test.cpp'
        PassPattern = 'PASS_0043e8e0'
    }
    [pscustomobject]@{
        Address = '0043e920'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e920.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e920_test.cpp'
        PassPattern = 'PASS_0043e920'
    }
    [pscustomobject]@{
        Address = '0043e960'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e960.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e960_test.cpp'
        PassPattern = 'PASS_0043e960'
    }
    [pscustomobject]@{
        Address = '0043e9a0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e9a0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e9a0_test.cpp'
        PassPattern = 'batch9_part4_0043e9a0_pass'
    }
    [pscustomobject]@{
        Address = '0043e9e0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e9e0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043e9e0_test.cpp'
        PassPattern = 'batch9_part4_0043e9e0_pass'
    }
    [pscustomobject]@{
        Address = '0043ea20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ea20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ea20_test.cpp'
        PassPattern = 'batch9_part4_0043ea20_pass'
    }
    [pscustomobject]@{
        Address = '0043ea60'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ea60.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ea60_test.cpp'
        PassPattern = 'batch9_part4_0043ea60_pass'
    }
    [pscustomobject]@{
        Address = '0043eaa0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eaa0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eaa0_test.cpp'
        PassPattern = 'batch9_part4_0043eaa0_pass'
    }
    [pscustomobject]@{
        Address = '0043eae0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eae0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eae0_test.cpp'
        PassPattern = 'batch9_part4_0043eae0_pass'
    }
    [pscustomobject]@{
        Address = '0043eb20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eb20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eb20_test.cpp'
        PassPattern = 'batch9_part4_0043eb20_pass'
    }
    [pscustomobject]@{
        Address = '0043eb60'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eb60.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eb60_test.cpp'
        PassPattern = 'batch9_part4_0043eb60_pass'
    }
    [pscustomobject]@{
        Address = '0043eba0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eba0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eba0_test.cpp'
        PassPattern = 'batch9_part4_0043eba0_pass'
    }
    [pscustomobject]@{
        Address = '0043ebe0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ebe0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ebe0_test.cpp'
        PassPattern = 'batch9_part4_0043ebe0_pass'
    }
    [pscustomobject]@{
        Address = '0043ec20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ec20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ec20_test.cpp'
        PassPattern = 'batch9_part4_0043ec20_pass'
    }
    [pscustomobject]@{
        Address = '0043ec60'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ec60.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ec60_test.cpp'
        PassPattern = 'batch9_part4_0043ec60_pass'
    }
    [pscustomobject]@{
        Address = '0043eca0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eca0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eca0_test.cpp'
        PassPattern = 'batch9_part4_0043eca0_pass'
    }
    [pscustomobject]@{
        Address = '0043ece0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ece0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ece0_test.cpp'
        PassPattern = 'batch9_part4_0043ece0_pass'
    }
    [pscustomobject]@{
        Address = '0043ed20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ed20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ed20_test.cpp'
        PassPattern = 'batch9_part4_0043ed20_pass'
    }
    [pscustomobject]@{
        Address = '0043ed60'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ed60.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ed60_test.cpp'
        PassPattern = 'batch9_part4_0043ed60_pass'
    }
    [pscustomobject]@{
        Address = '0043eda0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eda0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eda0_test.cpp'
        PassPattern = 'batch9_part4_0043eda0_pass'
    }
    [pscustomobject]@{
        Address = '0043ede0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ede0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ede0_test.cpp'
        PassPattern = 'batch9_part4_0043ede0_pass'
    }
    [pscustomobject]@{
        Address = '0043ee20'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ee20.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ee20_test.cpp'
        PassPattern = 'batch9_part4_0043ee20_pass'
    }
    [pscustomobject]@{
        Address = '0043ee60'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ee60.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ee60_test.cpp'
        PassPattern = 'batch9_part4_0043ee60_pass'
    }
    [pscustomobject]@{
        Address = '0043eef0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eef0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043eef0_test.cpp'
        PassPattern = 'batch9_part4_0043eef0_pass'
    }
    [pscustomobject]@{
        Address = '0043ef30'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ef30.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ef30_test.cpp'
        PassPattern = 'batch9_part4_0043ef30_pass'
    }
    [pscustomobject]@{
        Address = '0043ef70'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ef70.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0043ef70_test.cpp'
        PassPattern = 'batch9_part4_0043ef70_pass'
    }
    [pscustomobject]@{
        Address = '0043f2d0'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043f2d0.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043f2d0_test.cpp'
        PassPattern = 'batch9_part4_0043f2d0_pass'
    }
    [pscustomobject]@{
        Address = '00438400'
        Module = 'CPlayerGui'
        Source = 'CPlayerGui_Close_00438400.cpp'
        TestSource = 'CPlayerGui_Close_00438400_test.cpp'
        PassPattern = 'batch9_part4_00438400_pass'
    }
    [pscustomobject]@{
        Address = '0043df80'
        Module = '_global'
        Source = 'global_fillENavigatorTypeENavigatorType_0043df80.cpp'
        TestSource = 'global_fillENavigatorTypeENavigatorType_0043df80_test.cpp'
        PassPattern = 'batch9_part4_0043df80_pass'
    }
    [pscustomobject]@{
        Address = '0043f230'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043f230.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043f230_test.cpp'
        PassPattern = 'batch9_part4_0043f230_pass'
    }
    [pscustomobject]@{
        Address = '0043f400'
        Module = '_Cons_val<std::allocator<std::pair<CCharString,C3DVector>_>,std::pair<CCharString,C3DVector>,std'
        Source = 'ConsvalstdallocatorstdpairCCharStringC3DVectorstdpairCCharStringC3DVectorstd_pairclassCCharStringclassC3DVector_0043f400.cpp'
        TestSource = 'ConsvalstdallocatorstdpairCCharStringC3DVectorstdpairCCharStringC3DVectorstd_pairclassCCharStringclassC3DVector_0043f400_test.cpp'
        PassPattern = 'batch9_part4_0043f400_pass'
    }
    [pscustomobject]@{
        Address = '0043e4d0'
        Module = 'CFontBank'
        Source = 'CFontBank_GetVStringWidth_0043e4d0.cpp'
        TestSource = 'CFontBank_GetVStringWidth_0043e4d0_test.cpp'
        PassPattern = 'batch9_part4_0043e4d0_pass'
    }
    [pscustomobject]@{
        Address = '0043e3d0'
        Module = '_Dest_val<std'
        Source = 'Destvalstd_allocatorCMacroCMacro_0043e3d0.cpp'
        TestSource = 'Destvalstd_allocatorCMacroCMacro_0043e3d0_test.cpp'
        PassPattern = 'batch9_part4_0043e3d0_pass'
    }
    [pscustomobject]@{
        Address = '0043f3a0'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_0043f3a0.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_0043f3a0_test.cpp'
        PassPattern = 'batch9_part4_0043f3a0_pass'
    }
    [pscustomobject]@{
        Address = '00445680'
        Module = 'CInputTypeXboxPadLeftStickEvent'
        Source = 'CInputTypeXboxPadLeftStickEvent_IsInputTypeWithButton_00445680.cpp'
        TestSource = 'CInputTypeXboxPadLeftStickEvent_IsInputTypeWithButton_00445680_test.cpp'
        PassPattern = 'PASS_00445680'
    }
    [pscustomobject]@{
        Address = '004456c0'
        Module = 'CInputTypeXboxPadRightStickEvent'
        Source = 'CInputTypeXboxPadRightStickEvent_IsInputTypeWithButton_004456c0.cpp'
        TestSource = 'CInputTypeXboxPadRightStickEvent_IsInputTypeWithButton_004456c0_test.cpp'
        PassPattern = 'PASS_004456c0'
    }
    [pscustomobject]@{
        Address = '00445700'
        Module = 'CInputTypeMouseMovementEvent'
        Source = 'CInputTypeMouseMovementEvent_IsInputTypeWithButton_00445700.cpp'
        TestSource = 'CInputTypeMouseMovementEvent_IsInputTypeWithButton_00445700_test.cpp'
        PassPattern = 'PASS_00445700'
    }
    [pscustomobject]@{
        Address = '00445740'
        Module = 'CInputTypeMouseWheelMovementEvent'
        Source = 'CInputTypeMouseWheelMovementEvent_IsInputTypeWithButton_00445740.cpp'
        TestSource = 'CInputTypeMouseWheelMovementEvent_IsInputTypeWithButton_00445740_test.cpp'
        PassPattern = 'PASS_00445740'
    }
    [pscustomobject]@{
        Address = '00445780'
        Module = 'CInputTypeMouseWheelMovementUpEvent'
        Source = 'CInputTypeMouseWheelMovementUpEvent_IsInputTypeWithButton_00445780.cpp'
        TestSource = 'CInputTypeMouseWheelMovementUpEvent_IsInputTypeWithButton_00445780_test.cpp'
        PassPattern = 'PASS_00445780'
    }
    [pscustomobject]@{
        Address = '004457c0'
        Module = 'CInputTypeMouseWheelMovementDownEvent'
        Source = 'CInputTypeMouseWheelMovementDownEvent_IsInputTypeWithButton_004457c0.cpp'
        TestSource = 'CInputTypeMouseWheelMovementDownEvent_IsInputTypeWithButton_004457c0_test.cpp'
        PassPattern = 'PASS_004457c0'
    }
    [pscustomobject]@{
        Address = '00447a00'
        Module = 'CInputTypeKeyboardKeyEvent'
        Source = 'CInputTypeKeyboardKeyEvent_IsInputTypeWithButton_00447a00.cpp'
        TestSource = 'CInputTypeKeyboardKeyEvent_IsInputTypeWithButton_00447a00_test.cpp'
        PassPattern = 'PASS_00447a00'
    }
    [pscustomobject]@{
        Address = '00445ba0'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445ba0.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445ba0_test.cpp'
        PassPattern = 'PASS_00445ba0'
    }
    [pscustomobject]@{
        Address = '00445bb0'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445bb0.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445bb0_test.cpp'
        PassPattern = 'PASS_00445bb0'
    }
    [pscustomobject]@{
        Address = '00445bc0'
        Module = 'CTCShotDefinitionBase'
        Source = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445bc0.cpp'
        TestSource = 'CTCShotDefinitionBase_CanBeHitByMeleeStrike_00445bc0_test.cpp'
        PassPattern = 'PASS_00445bc0'
    }
    [pscustomobject]@{
        Address = '0044b110'
        Module = '_Cons_val<std::allocator<std::pair<CWideString_const_,float>_>,std::pair<CWideString_const_,float>,std'
        Source = 'ConsvalstdallocatorstdpairCWideStringconstfloatstdpairCWideStringconstfloatstd_Consval_0044b110.cpp'
        TestSource = 'ConsvalstdallocatorstdpairCWideStringconstfloatstdpairCWideStringconstfloatstd_Consval_0044b110_test.cpp'
        PassPattern = 'PASS_0044b110'
    }
    [pscustomobject]@{
        Address = '00440070'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440070.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440070_test.cpp'
        PassPattern = 'PASS_00440070'
    }
    [pscustomobject]@{
        Address = '00440d60'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440d60.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440d60_test.cpp'
        PassPattern = 'PASS_00440d60'
    }
    [pscustomobject]@{
        Address = '00445940'
        Module = 'CShaderPreParser'
        Source = 'CShaderPreParser_GetConstantCount_00445940.cpp'
        TestSource = 'CShaderPreParser_GetConstantCount_00445940_test.cpp'
        PassPattern = 'PASS_00445940'
    }
    [pscustomobject]@{
        Address = '00445950'
        Module = 'CShaderPreParser'
        Source = 'CShaderPreParser_GetConstantCount_00445950.cpp'
        TestSource = 'CShaderPreParser_GetConstantCount_00445950_test.cpp'
        PassPattern = 'PASS_00445950'
    }
    [pscustomobject]@{
        Address = '00445630'
        Module = 'CInputTypeXboxPadButtonEvent'
        Source = 'CInputTypeXboxPadButtonEvent_IsInputTypeWithButton_00445630.cpp'
        TestSource = 'CInputTypeXboxPadButtonEvent_IsInputTypeWithButton_00445630_test.cpp'
        PassPattern = 'PASS_00445630'
    }
    [pscustomobject]@{
        Address = '00449a30'
        Module = 'CEngine'
        Source = 'CEngine_GetEditorAnimation_00449a30.cpp'
        TestSource = 'CEngine_GetEditorAnimation_00449a30_test.cpp'
        PassPattern = 'PASS_00449a30'
    }
    [pscustomobject]@{
        Address = '00449b40'
        Module = 'CEngine'
        Source = 'CEngine_GetEditorAnimation_00449b40.cpp'
        TestSource = 'CEngine_GetEditorAnimation_00449b40_test.cpp'
        PassPattern = 'PASS_00449b40'
    }
    [pscustomobject]@{
        Address = '00443500'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443500.cpp'
        TestSource = 'CTCMapwho_OnDie_00443500_test.cpp'
        PassPattern = 'PASS_00443500'
    }
    [pscustomobject]@{
        Address = '00443520'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443520.cpp'
        TestSource = 'CTCMapwho_OnDie_00443520_test.cpp'
        PassPattern = 'PASS_00443520'
    }
    [pscustomobject]@{
        Address = '004437c0'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_004437c0.cpp'
        TestSource = 'CTCMapwho_OnDie_004437c0_test.cpp'
        PassPattern = 'PASS_004437c0'
    }
    [pscustomobject]@{
        Address = '00443820'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443820.cpp'
        TestSource = 'CTCMapwho_OnDie_00443820_test.cpp'
        PassPattern = 'PASS_00443820'
    }
    [pscustomobject]@{
        Address = '00443890'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443890.cpp'
        TestSource = 'CTCMapwho_OnDie_00443890_test.cpp'
        PassPattern = 'PASS_00443890'
    }
    [pscustomobject]@{
        Address = '004439c0'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_004439c0.cpp'
        TestSource = 'CTCMapwho_OnDie_004439c0_test.cpp'
        PassPattern = 'PASS_004439c0'
    }
    [pscustomobject]@{
        Address = '00443aa0'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443aa0.cpp'
        TestSource = 'CTCMapwho_OnDie_00443aa0_test.cpp'
        PassPattern = 'PASS_00443aa0'
    }
    [pscustomobject]@{
        Address = '00443b00'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00443b00.cpp'
        TestSource = 'CTCMapwho_OnDie_00443b00_test.cpp'
        PassPattern = 'PASS_00443b00'
    }
    [pscustomobject]@{
        Address = '00445350'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00445350.cpp'
        TestSource = 'CTCMapwho_OnDie_00445350_test.cpp'
        PassPattern = 'PASS_00445350'
    }
    [pscustomobject]@{
        Address = '00445370'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00445370.cpp'
        TestSource = 'CTCMapwho_OnDie_00445370_test.cpp'
        PassPattern = 'PASS_00445370'
    }
    [pscustomobject]@{
        Address = '004457d0'
        Module = 'CInputTypeMouseWheelMovementDownEvent'
        Source = 'CInputTypeMouseWheelMovementDownEvent_IsInputTypeWithMouseButton_004457d0.cpp'
        TestSource = 'CInputTypeMouseWheelMovementDownEvent_IsInputTypeWithMouseButton_004457d0_test.cpp'
        PassPattern = 'PASS_004457d0'
    }
    [pscustomobject]@{
        Address = '0043fcd0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0043fcd0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0043fcd0_test.cpp'
        PassPattern = 'PASS_0043fcd0'
    }
    [pscustomobject]@{
        Address = '00440cb0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440cb0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440cb0_test.cpp'
        PassPattern = 'PASS_00440cb0'
    }
    [pscustomobject]@{
        Address = '00440d40'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440d40.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440d40_test.cpp'
        PassPattern = 'PASS_00440d40'
    }
    [pscustomobject]@{
        Address = '00440e40'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440e40.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440e40_test.cpp'
        PassPattern = 'PASS_00440e40'
    }
    [pscustomobject]@{
        Address = '00440ec0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440ec0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440ec0_test.cpp'
        PassPattern = 'PASS_00440ec0'
    }
    [pscustomobject]@{
        Address = '00440ee0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440ee0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440ee0_test.cpp'
        PassPattern = 'PASS_00440ee0'
    }
    [pscustomobject]@{
        Address = '004422f0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004422f0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004422f0_test.cpp'
        PassPattern = 'PASS_004422f0'
    }
    [pscustomobject]@{
        Address = '00442310'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00442310.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00442310_test.cpp'
        PassPattern = 'PASS_00442310'
    }
    [pscustomobject]@{
        Address = '00442330'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00442330.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00442330_test.cpp'
        PassPattern = 'PASS_00442330'
    }
    [pscustomobject]@{
        Address = '00443290'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00443290.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00443290_test.cpp'
        PassPattern = 'PASS_00443290'
    }
    [pscustomobject]@{
        Address = '00445650'
        Module = 'CInputTypeXboxPadButtonEvent'
        Source = 'CInputTypeXboxPadButtonEvent_IsInputTypeWithMouseButton_00445650.cpp'
        TestSource = 'CInputTypeXboxPadButtonEvent_IsInputTypeWithMouseButton_00445650_test.cpp'
        PassPattern = 'PASS_00445650'
    }
    [pscustomobject]@{
        Address = '00445690'
        Module = 'CInputTypeXboxPadLeftStickEvent'
        Source = 'CInputTypeXboxPadLeftStickEvent_IsInputTypeWithMouseButton_00445690.cpp'
        TestSource = 'CInputTypeXboxPadLeftStickEvent_IsInputTypeWithMouseButton_00445690_test.cpp'
        PassPattern = 'PASS_00445690'
    }
    [pscustomobject]@{
        Address = '004456d0'
        Module = 'CInputTypeXboxPadRightStickEvent'
        Source = 'CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton_004456d0.cpp'
        TestSource = 'CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton_004456d0_test.cpp'
        PassPattern = 'PASS_004456d0'
    }
    [pscustomobject]@{
        Address = '00445710'
        Module = 'CInputTypeMouseMovementEvent'
        Source = 'CInputTypeMouseMovementEvent_IsInputTypeWithMouseButton_00445710.cpp'
        TestSource = 'CInputTypeMouseMovementEvent_IsInputTypeWithMouseButton_00445710_test.cpp'
        PassPattern = 'PASS_00445710'
    }
    [pscustomobject]@{
        Address = '00445750'
        Module = 'CInputTypeMouseWheelMovementEvent'
        Source = 'CInputTypeMouseWheelMovementEvent_IsInputTypeWithMouseButton_00445750.cpp'
        TestSource = 'CInputTypeMouseWheelMovementEvent_IsInputTypeWithMouseButton_00445750_test.cpp'
        PassPattern = 'PASS_00445750'
    }
    [pscustomobject]@{
        Address = '00445790'
        Module = 'CInputTypeMouseWheelMovementUpEvent'
        Source = 'CInputTypeMouseWheelMovementUpEvent_IsInputTypeWithMouseButton_00445790.cpp'
        TestSource = 'CInputTypeMouseWheelMovementUpEvent_IsInputTypeWithMouseButton_00445790_test.cpp'
        PassPattern = 'PASS_00445790'
    }
    [pscustomobject]@{
        Address = '00449630'
        Module = 'CGuiVarTransferBool'
        Source = 'CGuiVarTransferBool_TransferToGui_00449630.cpp'
        TestSource = 'CGuiVarTransferBool_TransferToGui_00449630_test.cpp'
        PassPattern = 'PASS_00449630'
    }
    [pscustomobject]@{
        Address = '00449b20'
        Module = 'CSpellContainerList'
        Source = 'CSpellContainerList_AddChild_00449b20.cpp'
        TestSource = 'CSpellContainerList_AddChild_00449b20_test.cpp'
        PassPattern = 'PASS_00449b20'
    }
    [pscustomobject]@{
        Address = '00445920'
        Module = '_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs'
        Source = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00445920.cpp'
        TestSource = 'ConsvalstdallocatorstdpairEHeroMorphTypeCParticleMorphsCEntrystdpairEHeroMorphTy_CEntryconst_00445920_test.cpp'
        PassPattern = 'PASS_00445920'
    }
    [pscustomobject]@{
        Address = '0044b930'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044b930.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044b930_test.cpp'
        PassPattern = 'PASS_0044b930'
    }
    [pscustomobject]@{
        Address = '0043f960'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f960.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f960_test.cpp'
        PassPattern = 'PASS_0043f960'
    }
    [pscustomobject]@{
        Address = '00442960'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442960.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442960_test.cpp'
        PassPattern = 'PASS_00442960'
    }
    [pscustomobject]@{
        Address = '00442d30'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442d30.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442d30_test.cpp'
        PassPattern = 'PASS_00442d30'
    }
    [pscustomobject]@{
        Address = '00443bb0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443bb0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443bb0_test.cpp'
        PassPattern = 'PASS_00443bb0'
    }
    [pscustomobject]@{
        Address = '00444e90'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00444e90.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00444e90_test.cpp'
        PassPattern = 'PASS_00444e90'
    }
    [pscustomobject]@{
        Address = '00444eb0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00444eb0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00444eb0_test.cpp'
        PassPattern = 'PASS_00444eb0'
    }
    [pscustomobject]@{
        Address = '00449420'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00449420.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00449420_test.cpp'
        PassPattern = 'PASS_00449420'
    }
    [pscustomobject]@{
        Address = '004497c0'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_IsThingMainPlayer_004497c0.cpp'
        TestSource = 'CPlayerManager_IsThingMainPlayer_004497c0_test.cpp'
        PassPattern = 'PASS_004497c0'
    }
    [pscustomobject]@{
        Address = '0043f750'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f750.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f750_test.cpp'
        PassPattern = 'PASS_0043f750'
    }
    [pscustomobject]@{
        Address = '0043f980'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f980.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f980_test.cpp'
        PassPattern = 'PASS_0043f980'
    }
    [pscustomobject]@{
        Address = '00440ba0'
        Module = '_Destroy_range<std'
        Source = 'Destroyrangestd_allocatorCSpeechFrame_00440ba0.cpp'
        TestSource = 'Destroyrangestd_allocatorCSpeechFrame_00440ba0_test.cpp'
        PassPattern = 'PASS_00440ba0'
    }
    [pscustomobject]@{
        Address = '00440bd0'
        Module = '_Destroy_range<std'
        Source = 'Destroyrangestd_allocatorCSpeechFrame_00440bd0.cpp'
        TestSource = 'Destroyrangestd_allocatorCSpeechFrame_00440bd0_test.cpp'
        PassPattern = 'PASS_00440bd0'
    }
    [pscustomobject]@{
        Address = '00449700'
        Module = 'CWorld'
        Source = 'CWorld_DrawGetEnvironment_00449700.cpp'
        TestSource = 'CWorld_DrawGetEnvironment_00449700_test.cpp'
        PassPattern = 'PASS_00449700'
    }
    [pscustomobject]@{
        Address = '004406a0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004406a0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004406a0_test.cpp'
        PassPattern = 'PASS_004406a0'
    }
    [pscustomobject]@{
        Address = '00446b10'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsGameActionEventInQueue_00446b10.cpp'
        TestSource = 'CGamePlayerInterface_IsGameActionEventInQueue_00446b10_test.cpp'
        PassPattern = 'PASS_00446b10'
    }
    [pscustomobject]@{
        Address = '00443190'
        Module = 'CTCAssassinRush'
        Source = 'CTCAssassinRush_FinishMove_00443190.cpp'
        TestSource = 'CTCAssassinRush_FinishMove_00443190_test.cpp'
        PassPattern = 'PASS_00443190'
    }
    [pscustomobject]@{
        Address = '004431c0'
        Module = 'CTCAssassinRush'
        Source = 'CTCAssassinRush_FinishMove_004431c0.cpp'
        TestSource = 'CTCAssassinRush_FinishMove_004431c0_test.cpp'
        PassPattern = 'PASS_004431c0'
    }
    [pscustomobject]@{
        Address = '0043fdc0'
        Module = 'CBankFileAsyncData'
        Source = 'CBankFileAsyncData_FreeBuffer_0043fdc0.cpp'
        TestSource = 'CBankFileAsyncData_FreeBuffer_0043fdc0_test.cpp'
        PassPattern = 'PASS_0043fdc0'
    }
    [pscustomobject]@{
        Address = '0043fe80'
        Module = 'CBankFileAsyncData'
        Source = 'CBankFileAsyncData_FreeBuffer_0043fe80.cpp'
        TestSource = 'CBankFileAsyncData_FreeBuffer_0043fe80_test.cpp'
        PassPattern = 'PASS_0043fe80'
    }
    [pscustomobject]@{
        Address = '004431f0'
        Module = 'CTCAssassinRush'
        Source = 'CTCAssassinRush_FinishMove_004431f0.cpp'
        TestSource = 'CTCAssassinRush_FinishMove_004431f0_test.cpp'
        PassPattern = 'PASS_004431f0'
    }
    [pscustomobject]@{
        Address = '0044b120'
        Module = '_Uninit_copy<CFeatDef*,CFeatDef*,std'
        Source = 'UninitcopyCFeatDefCFeatDefstd_allocatorCFeatDef_0044b120.cpp'
        TestSource = 'UninitcopyCFeatDefCFeatDefstd_allocatorCFeatDef_0044b120_test.cpp'
        PassPattern = 'PASS_0044b120'
    }
    [pscustomobject]@{
        Address = '00447a10'
        Module = 'CInputTypeKeyboardKeyEvent'
        Source = 'CInputTypeKeyboardKeyEvent_IsInputTypeWithMouseButton_00447a10.cpp'
        TestSource = 'CInputTypeKeyboardKeyEvent_IsInputTypeWithMouseButton_00447a10_test.cpp'
        PassPattern = 'PASS_00447a10'
    }
    [pscustomobject]@{
        Address = '00442870'
        Module = 'CCombatWheel'
        Source = 'CCombatWheel_ResetRings_00442870.cpp'
        TestSource = 'CCombatWheel_ResetRings_00442870_test.cpp'
        PassPattern = 'PASS_00442870'
    }
    [pscustomobject]@{
        Address = '00448410'
        Module = '_Dest_val<std::allocator<std::pair<unsigned_long,CCountedPointer<CFaction>_>_>,std'
        Source = 'DestvalstdallocatorstdpairunsignedlongCCountedPointerCFactionstd_pairunsignedlongCCountedPointerCFaction_00448410.cpp'
        TestSource = 'DestvalstdallocatorstdpairunsignedlongCCountedPointerCFactionstd_pairunsignedlongCCountedPointerCFaction_00448410_test.cpp'
        PassPattern = 'PASS_00448410'
    }
    [pscustomobject]@{
        Address = '0044a530'
        Module = 'CTCHeroExperience'
        Source = 'CTCHeroExperience_UpdateAbilitiesForAllStats_0044a530.cpp'
        TestSource = 'CTCHeroExperience_UpdateAbilitiesForAllStats_0044a530_test.cpp'
        PassPattern = 'PASS_0044a530'
    }
    [pscustomobject]@{
        Address = '004477c0'
        Module = '_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>,std::allocator<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>_>_>_>,std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>*,std::allocator<std'
        Source = 'UninitcopystdVectorconstiteratorstdVectorvalstdpairunsignedint64CCountedPointerC_pairunsignedint64CCountedPointerCActiveEntityScr_004477c0.cpp'
        TestSource = 'UninitcopystdVectorconstiteratorstdVectorvalstdpairunsignedint64CCountedPointerC_pairunsignedint64CCountedPointerCActiveEntityScr_004477c0_test.cpp'
        PassPattern = 'PASS_004477c0'
    }
    [pscustomobject]@{
        Address = '004482c0'
        Module = '_Uninit_fill_n<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>*,unsigned_int,std::pair<unsigned___int64,class_CCountedPointer<class_CActiveEntityScriptBase>_>,std::allocator<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>_>,std'
        Source = 'Uninitfillnstdpairunsignedint64CCountedPointerCActiveEntityScriptBaseunsignedint_pairunsignedint64classCCountedPointerclassCActiv_004482c0.cpp'
        TestSource = 'Uninitfillnstdpairunsignedint64CCountedPointerCActiveEntityScriptBaseunsignedint_pairunsignedint64classCCountedPointerclassCActiv_004482c0_test.cpp'
        PassPattern = 'PASS_004482c0'
    }
    [pscustomobject]@{
        Address = '004400c0'
        Module = '_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<CScriptThing,std::allocator<CScriptThing>_>_>,CScriptThing*,std'
        Source = 'UninitcopystdVectorconstiteratorstdVectorvalCScriptThingstdallocatorCScriptThing_allocatorCScriptThing_004400c0.cpp'
        TestSource = 'UninitcopystdVectorconstiteratorstdVectorvalCScriptThingstdallocatorCScriptThing_allocatorCScriptThing_004400c0_test.cpp'
        PassPattern = 'PASS_004400c0'
    }
    [pscustomobject]@{
        Address = '00441380'
        Module = 'CEngineScreenEffectOutlineGlow'
        Source = 'CEngineScreenEffectOutlineGlow_FinishWithDynamicTextures_00441380.cpp'
        TestSource = 'CEngineScreenEffectOutlineGlow_FinishWithDynamicTextures_00441380_test.cpp'
        PassPattern = 'PASS_00441380'
    }
    [pscustomobject]@{
        Address = '004454e0'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_004454e0.cpp'
        TestSource = 'CTCMapwho_OnDie_004454e0_test.cpp'
        PassPattern = 'PASS_004454e0'
    }
    [pscustomobject]@{
        Address = '00447d50'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00447d50.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00447d50_test.cpp'
        PassPattern = 'PASS_00447d50'
    }
    [pscustomobject]@{
        Address = '004483d0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_004483d0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_004483d0_test.cpp'
        PassPattern = 'PASS_004483d0'
    }
    [pscustomobject]@{
        Address = '00445b40'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_LoadInitialInfo_00445b40.cpp'
        TestSource = 'CGamePlayerInterface_LoadInitialInfo_00445b40_test.cpp'
        PassPattern = 'PASS_00445b40'
    }
    [pscustomobject]@{
        Address = '0044b150'
        Module = '_global'
        Source = 'global_Fillnunsignedlongunsignedintunsignedlong_0044b150.cpp'
        TestSource = 'global_Fillnunsignedlongunsignedintunsignedlong_0044b150_test.cpp'
        PassPattern = 'PASS_0044b150'
    }
    [pscustomobject]@{
        Address = '0043f920'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f920.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043f920_test.cpp'
        PassPattern = 'PASS_0043f920'
    }
    [pscustomobject]@{
        Address = '0043fe40'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043fe40.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043fe40_test.cpp'
        PassPattern = 'PASS_0043fe40'
    }
    [pscustomobject]@{
        Address = '00440730'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_00440730.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_00440730_test.cpp'
        PassPattern = 'PASS_00440730'
    }
    [pscustomobject]@{
        Address = '00440770'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_00440770.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_00440770_test.cpp'
        PassPattern = 'PASS_00440770'
    }
    [pscustomobject]@{
        Address = '00448280'
        Module = '_Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>,std::allocator<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>_>_>_>,std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>*,std::allocator<std'
        Source = 'UninitcopystdVectorconstiteratorstdVectorvalstdpairunsignedint64CCountedPointerC_pairunsignedint64CCountedPointerCActiveEntityScr_00448280.cpp'
        TestSource = 'UninitcopystdVectorconstiteratorstdVectorvalstdpairunsignedint64CCountedPointerC_pairunsignedint64CCountedPointerCActiveEntityScr_00448280_test.cpp'
        PassPattern = 'PASS_00448280'
    }
    [pscustomobject]@{
        Address = '00449880'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_IsPlayer_00449880.cpp'
        TestSource = 'CPlayerManager_IsPlayer_00449880_test.cpp'
        PassPattern = 'PASS_00449880'
    }
    [pscustomobject]@{
        Address = '0043fd40'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043fd40.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043fd40_test.cpp'
        PassPattern = 'PASS_0043fd40'
    }
    [pscustomobject]@{
        Address = '0043fd80'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043fd80.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043fd80_test.cpp'
        PassPattern = 'PASS_0043fd80'
    }
    [pscustomobject]@{
        Address = '00440080'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440080.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440080_test.cpp'
        PassPattern = 'PASS_00440080'
    }
    [pscustomobject]@{
        Address = '00448910'
        Module = '_global'
        Source = 'global_CopyimplCNavigationPositionCNavigationPosition_00448910.cpp'
        TestSource = 'global_CopyimplCNavigationPositionCNavigationPosition_00448910_test.cpp'
        PassPattern = 'PASS_00448910'
    }
    [pscustomobject]@{
        Address = '00449990'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_GetPlayerNumberFromJoystickDeviceNumber_00449990.cpp'
        TestSource = 'CPlayerManager_GetPlayerNumberFromJoystickDeviceNumber_00449990_test.cpp'
        PassPattern = 'PASS_00449990'
    }
    [pscustomobject]@{
        Address = '00443c50'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00443c50.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00443c50_test.cpp'
        PassPattern = 'PASS_00443c50'
    }
    [pscustomobject]@{
        Address = '00443ca0'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00443ca0.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00443ca0_test.cpp'
        PassPattern = 'PASS_00443ca0'
    }
    [pscustomobject]@{
        Address = '00440650'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440650.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440650_test.cpp'
        PassPattern = 'PASS_00440650'
    }
    [pscustomobject]@{
        Address = '0043fdf0'
        Module = 'CNavigatorQueueManager'
        Source = 'CNavigatorQueueManager_FrameUpdate_0043fdf0.cpp'
        TestSource = 'CNavigatorQueueManager_FrameUpdate_0043fdf0_test.cpp'
        PassPattern = 'PASS_0043fdf0'
    }
    [pscustomobject]@{
        Address = '004496b0'
        Module = 'CAIStateGroup_IdleAtHome'
        Source = 'CAIStateGroupIdleAtHome_InitCreaturePosition_004496b0.cpp'
        TestSource = 'CAIStateGroupIdleAtHome_InitCreaturePosition_004496b0_test.cpp'
        PassPattern = 'PASS_004496b0'
    }
    [pscustomobject]@{
        Address = '004499e0'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_IsPlayerAssociatedWithJoystickDeviceNumber_004499e0.cpp'
        TestSource = 'CPlayerManager_IsPlayerAssociatedWithJoystickDeviceNumber_004499e0_test.cpp'
        PassPattern = 'batch10_part4_004499e0_pass'
    }
    [pscustomobject]@{
        Address = '00445e90'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_00445e90.cpp'
        TestSource = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_00445e90_test.cpp'
        PassPattern = 'batch10_part4_00445e90_pass'
    }
    [pscustomobject]@{
        Address = '004460c0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_004460c0.cpp'
        TestSource = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_004460c0_test.cpp'
        PassPattern = 'batch10_part4_004460c0_pass'
    }
    [pscustomobject]@{
        Address = '004461d0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_004461d0.cpp'
        TestSource = 'CGamePlayerInterface_IsAssignableSpecialMoveButtonInQueue_004461d0_test.cpp'
        PassPattern = 'batch10_part4_004461d0_pass'
    }
    [pscustomobject]@{
        Address = '00440340'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00440340.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00440340_test.cpp'
        PassPattern = 'batch10_part4_00440340_pass'
    }
    [pscustomobject]@{
        Address = '004425c0'
        Module = 'CWound'
        Source = 'CWound_Kill_004425c0.cpp'
        TestSource = 'CWound_Kill_004425c0_test.cpp'
        PassPattern = 'batch10_part4_004425c0_pass'
    }
    [pscustomobject]@{
        Address = '00448080'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00448080.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00448080_test.cpp'
        PassPattern = 'batch10_part4_00448080_pass'
    }
    [pscustomobject]@{
        Address = '004498c0'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_GetPlayer_004498c0.cpp'
        TestSource = 'CPlayerManager_GetPlayer_004498c0_test.cpp'
        PassPattern = 'batch10_part4_004498c0_pass'
    }
    [pscustomobject]@{
        Address = '00449910'
        Module = 'CPlayerManager'
        Source = 'CPlayerManager_GetPlayer_00449910.cpp'
        TestSource = 'CPlayerManager_GetPlayer_00449910_test.cpp'
        PassPattern = 'batch10_part4_00449910_pass'
    }
    [pscustomobject]@{
        Address = '00443140'
        Module = 'CEngineScreenEffectDisplacementRenderer'
        Source = 'CEngineScreenEffectDisplacementRenderer_FinishWithDynamicTextures_00443140.cpp'
        TestSource = 'CEngineScreenEffectDisplacementRenderer_FinishWithDynamicTextures_00443140_test.cpp'
        PassPattern = 'batch10_part4_00443140_pass'
    }
    [pscustomobject]@{
        Address = '00448860'
        Module = '_Destroy_range<std::allocator<std'
        Source = 'Destroyrangestdallocatorstd_pairCCharStringCCountedPointerCEntityScriptBindi_00448860.cpp'
        TestSource = 'Destroyrangestdallocatorstd_pairCCharStringCCountedPointerCEntityScriptBindi_00448860_test.cpp'
        PassPattern = 'batch10_part4_00448860_pass'
    }
    [pscustomobject]@{
        Address = '0043fcf0'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_0043fcf0.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_0043fcf0_test.cpp'
        PassPattern = 'batch10_part4_0043fcf0_pass'
    }
    [pscustomobject]@{
        Address = '00440c40'
        Module = '~vector<CConversation,std'
        Source = 'vectorCConversationstd_allocatorCConversation_00440c40.cpp'
        TestSource = 'vectorCConversationstd_allocatorCConversation_00440c40_test.cpp'
        PassPattern = 'batch10_part4_00440c40_pass'
    }
    [pscustomobject]@{
        Address = '00448380'
        Module = 'CItem'
        Source = 'CItem_operator_00448380.cpp'
        TestSource = 'CItem_operator_00448380_test.cpp'
        PassPattern = 'batch10_part4_00448380_pass'
    }
    [pscustomobject]@{
        Address = '00447c80'
        Module = 'CVectorMap<long'
        Source = 'CVectorMaplong_LowerBound_00447c80.cpp'
        TestSource = 'CVectorMaplong_LowerBound_00447c80_test.cpp'
        PassPattern = 'batch10_part4_00447c80_pass'
    }
    [pscustomobject]@{
        Address = '00440a70'
        Module = '~vector<CConversation,std'
        Source = 'vectorCConversationstd_allocatorCConversation_00440a70.cpp'
        TestSource = 'vectorCConversationstd_allocatorCConversation_00440a70_test.cpp'
        PassPattern = 'batch10_part4_00440a70_pass'
    }
    [pscustomobject]@{
        Address = '00445bd0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsEventGameAction_00445bd0.cpp'
        TestSource = 'CGamePlayerInterface_IsEventGameAction_00445bd0_test.cpp'
        PassPattern = 'batch10_part4_00445bd0_pass'
    }
    [pscustomobject]@{
        Address = '00449650'
        Module = 'CTCVillage'
        Source = 'CTCVillage_OnInitialActivate_00449650.cpp'
        TestSource = 'CTCVillage_OnInitialActivate_00449650_test.cpp'
        PassPattern = 'batch10_part4_00449650_pass'
    }
    [pscustomobject]@{
        Address = '004406d0'
        Module = '~vector<CConversation,std'
        Source = 'vectorCConversationstd_allocatorCConversation_004406d0.cpp'
        TestSource = 'vectorCConversationstd_allocatorCConversation_004406d0_test.cpp'
        PassPattern = 'batch10_part4_004406d0_pass'
    }
    [pscustomobject]@{
        Address = '00445ae0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_SaveInitialInfo_00445ae0.cpp'
        TestSource = 'CGamePlayerInterface_SaveInitialInfo_00445ae0_test.cpp'
        PassPattern = 'batch10_part4_00445ae0_pass'
    }
    [pscustomobject]@{
        Address = '00447760'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00447760.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00447760_test.cpp'
        PassPattern = 'batch10_part4_00447760_pass'
    }
    [pscustomobject]@{
        Address = '004488b0'
        Module = '_Destroy_range<std::allocator<std'
        Source = 'Destroyrangestdallocatorstd_pairlongCCountedPointerCRegionConnectionNode_004488b0.cpp'
        TestSource = 'Destroyrangestdallocatorstd_pairlongCCountedPointerCRegionConnectionNode_004488b0_test.cpp'
        PassPattern = 'batch10_part4_004488b0_pass'
    }
    [pscustomobject]@{
        Address = '00442770'
        Module = 'vector<unsigned_int'
        Source = 'vectorunsignedint_resize_00442770.cpp'
        TestSource = 'vectorunsignedint_resize_00442770_test.cpp'
        PassPattern = 'batch10_part4_00442770_pass'
    }
    [pscustomobject]@{
        Address = '00445e30'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsEventAssignableSpecialMoveButton_00445e30.cpp'
        TestSource = 'CGamePlayerInterface_IsEventAssignableSpecialMoveButton_00445e30_test.cpp'
        PassPattern = 'batch10_part4_00445e30_pass'
    }
    [pscustomobject]@{
        Address = '00445ee0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445ee0.cpp'
        TestSource = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445ee0_test.cpp'
        PassPattern = 'batch10_part4_00445ee0_pass'
    }
    [pscustomobject]@{
        Address = '00445f40'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445f40.cpp'
        TestSource = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445f40_test.cpp'
        PassPattern = 'batch10_part4_00445f40_pass'
    }
    [pscustomobject]@{
        Address = '00445fa0'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445fa0.cpp'
        TestSource = 'CGamePlayerInterface_GetAssignableSpecialMoveIndexForButton_00445fa0_test.cpp'
        PassPattern = 'batch10_part4_00445fa0_pass'
    }
    [pscustomobject]@{
        Address = '0044a140'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_0044a140.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_0044a140_test.cpp'
        PassPattern = 'batch10_part4_0044a140_pass'
    }
    [pscustomobject]@{
        Address = '00446000'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsEventContextSensitiveItemButton_00446000.cpp'
        TestSource = 'CGamePlayerInterface_IsEventContextSensitiveItemButton_00446000_test.cpp'
        PassPattern = 'batch10_part4_00446000_pass'
    }
    [pscustomobject]@{
        Address = '00440e60'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00440e60.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00440e60_test.cpp'
        PassPattern = 'batch10_part4_00440e60_pass'
    }
    [pscustomobject]@{
        Address = '00446060'
        Module = 'CGamePlayerInterface'
        Source = 'CGamePlayerInterface_IsOtherQuickAccessItemButtonHeld_00446060.cpp'
        TestSource = 'CGamePlayerInterface_IsOtherQuickAccessItemButtonHeld_00446060_test.cpp'
        PassPattern = 'batch10_part4_00446060_pass'
    }
    [pscustomobject]@{
        Address = '0044c158'
        Module = 'CPhysicsDef'
        Source = 'CPhysicsDef_GetSizeofClass_0044c158.cpp'
        TestSource = 'CPhysicsDef_GetSizeofClass_0044c158_test.cpp'
        PassPattern = 'CPhysicsDef_0044c158_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c269'
        Module = 'CActionUseDef'
        Source = 'CActionUseDef_GetSizeofClass_0044c269.cpp'
        TestSource = 'CActionUseDef_GetSizeofClass_0044c269_test.cpp'
        PassPattern = 'CActionUseDef_0044c269_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c313'
        Module = 'CSpecialAbilitiesDrainLifeDataDef'
        Source = 'CSpecialAbilitiesDrainLifeDataDef_GetSizeofClass_0044c313.cpp'
        TestSource = 'CSpecialAbilitiesDrainLifeDataDef_GetSizeofClass_0044c313_test.cpp'
        PassPattern = 'CSpecialAbilitiesDrainLifeDataDef_0044c313_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c329'
        Module = 'CSpecialAbilitiesForcePushDataDef'
        Source = 'CSpecialAbilitiesForcePushDataDef_GetSizeofClass_0044c329.cpp'
        TestSource = 'CSpecialAbilitiesForcePushDataDef_GetSizeofClass_0044c329_test.cpp'
        PassPattern = 'CSpecialAbilitiesForcePushDataDef_0044c329_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c33f'
        Module = 'CCreatureStatsDef'
        Source = 'CCreatureStatsDef_GetSizeofClass_0044c33f.cpp'
        TestSource = 'CCreatureStatsDef_GetSizeofClass_0044c33f_test.cpp'
        PassPattern = 'CCreatureStatsDef_0044c33f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c364'
        Module = 'CCoopSpiritDef'
        Source = 'CCoopSpiritDef_GetSizeofClass_0044c364.cpp'
        TestSource = 'CCoopSpiritDef_GetSizeofClass_0044c364_test.cpp'
        PassPattern = 'CCoopSpiritDef_0044c364_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c37a'
        Module = 'CContextSensitiveItemDef'
        Source = 'CContextSensitiveItemDef_GetSizeofClass_0044c37a.cpp'
        TestSource = 'CContextSensitiveItemDef_GetSizeofClass_0044c37a_test.cpp'
        PassPattern = 'CContextSensitiveItemDef_0044c37a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c390'
        Module = 'CBonusItemDef'
        Source = 'CBonusItemDef_GetSizeofClass_0044c390.cpp'
        TestSource = 'CBonusItemDef_GetSizeofClass_0044c390_test.cpp'
        PassPattern = 'CBonusItemDef_0044c390_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c3a6'
        Module = 'CHeroTitleDef'
        Source = 'CHeroTitleDef_GetSizeofClass_0044c3a6.cpp'
        TestSource = 'CHeroTitleDef_GetSizeofClass_0044c3a6_test.cpp'
        PassPattern = 'CHeroTitleDef_0044c3a6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c3d4'
        Module = 'CTattooDef'
        Source = 'CTattooDef_GetSizeofClass_0044c3d4.cpp'
        TestSource = 'CTattooDef_GetSizeofClass_0044c3d4_test.cpp'
        PassPattern = 'CTattooDef_0044c3d4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c3ea'
        Module = 'CHairCardDef'
        Source = 'CHairCardDef_GetSizeofClass_0044c3ea.cpp'
        TestSource = 'CHairCardDef_GetSizeofClass_0044c3ea_test.cpp'
        PassPattern = 'CHairCardDef_0044c3ea_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c549'
        Module = 'CCombatAbilityBlockCounterAttackDef'
        Source = 'CCombatAbilityBlockCounterAttackDef_GetSizeofClass_0044c549.cpp'
        TestSource = 'CCombatAbilityBlockCounterAttackDef_GetSizeofClass_0044c549_test.cpp'
        PassPattern = 'CCombatAbilityBlockCounterAttackDef_0044c549_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c561'
        Module = 'CCombatAbilityFlourishCounterAttackDef'
        Source = 'CCombatAbilityFlourishCounterAttackDef_GetSizeofClass_0044c561.cpp'
        TestSource = 'CCombatAbilityFlourishCounterAttackDef_GetSizeofClass_0044c561_test.cpp'
        PassPattern = 'CCombatAbilityFlourishCounterAttackDef_0044c561_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c579'
        Module = 'CCombatAbilityGetHitCounterAttackDef'
        Source = 'CCombatAbilityGetHitCounterAttackDef_GetSizeofClass_0044c579.cpp'
        TestSource = 'CCombatAbilityGetHitCounterAttackDef_GetSizeofClass_0044c579_test.cpp'
        PassPattern = 'CCombatAbilityGetHitCounterAttackDef_0044c579_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c599'
        Module = 'CCombatAbilityStrafeDef'
        Source = 'CCombatAbilityStrafeDef_GetSizeofClass_0044c599.cpp'
        TestSource = 'CCombatAbilityStrafeDef_GetSizeofClass_0044c599_test.cpp'
        PassPattern = 'CCombatAbilityStrafeDef_0044c599_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c5b9'
        Module = 'CCombatAbilityUseProjectileWeaponDef'
        Source = 'CCombatAbilityUseProjectileWeaponDef_GetSizeofClass_0044c5b9.cpp'
        TestSource = 'CCombatAbilityUseProjectileWeaponDef_GetSizeofClass_0044c5b9_test.cpp'
        PassPattern = 'CCombatAbilityUseProjectileWeaponDef_0044c5b9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c5cf'
        Module = 'CDragonActionSwoopDef'
        Source = 'CDragonActionSwoopDef_GetSizeofClass_0044c5cf.cpp'
        TestSource = 'CDragonActionSwoopDef_GetSizeofClass_0044c5cf_test.cpp'
        PassPattern = 'CDragonActionSwoopDef_0044c5cf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c5e5'
        Module = 'CDragonActionNapalmDef'
        Source = 'CDragonActionNapalmDef_GetSizeofClass_0044c5e5.cpp'
        TestSource = 'CDragonActionNapalmDef_GetSizeofClass_0044c5e5_test.cpp'
        PassPattern = 'CDragonActionNapalmDef_0044c5e5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c5fb'
        Module = 'CDragonActionHoverDef'
        Source = 'CDragonActionHoverDef_GetSizeofClass_0044c5fb.cpp'
        TestSource = 'CDragonActionHoverDef_GetSizeofClass_0044c5fb_test.cpp'
        PassPattern = 'CDragonActionHoverDef_0044c5fb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c611'
        Module = 'CAugmentationDef'
        Source = 'CAugmentationDef_GetSizeofClass_0044c611.cpp'
        TestSource = 'CAugmentationDef_GetSizeofClass_0044c611_test.cpp'
        PassPattern = 'CAugmentationDef_0044c611_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c627'
        Module = 'CAbilityDef'
        Source = 'CAbilityDef_GetSizeofClass_0044c627.cpp'
        TestSource = 'CAbilityDef_GetSizeofClass_0044c627_test.cpp'
        PassPattern = 'CAbilityDef_0044c627_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c63d'
        Module = 'CJackDragonDef'
        Source = 'CJackDragonDef_GetSizeofClass_0044c63d.cpp'
        TestSource = 'CJackDragonDef_GetSizeofClass_0044c63d_test.cpp'
        PassPattern = 'CJackDragonDef_0044c63d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c653'
        Module = 'CBriarRoseDef'
        Source = 'CBriarRoseDef_GetSizeofClass_0044c653.cpp'
        TestSource = 'CBriarRoseDef_GetSizeofClass_0044c653_test.cpp'
        PassPattern = 'CBriarRoseDef_0044c653_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c669'
        Module = 'CSnowTrollDef'
        Source = 'CSnowTrollDef_GetSizeofClass_0044c669.cpp'
        TestSource = 'CSnowTrollDef_GetSizeofClass_0044c669_test.cpp'
        PassPattern = 'CSnowTrollDef_0044c669_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c67f'
        Module = 'CGuildMasterDef'
        Source = 'CGuildMasterDef_GetSizeofClass_0044c67f.cpp'
        TestSource = 'CGuildMasterDef_GetSizeofClass_0044c67f_test.cpp'
        PassPattern = 'CGuildMasterDef_0044c67f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c695'
        Module = 'CSummonerDef'
        Source = 'CSummonerDef_GetSizeofClass_0044c695.cpp'
        TestSource = 'CSummonerDef_GetSizeofClass_0044c695_test.cpp'
        PassPattern = 'CSummonerDef_0044c695_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c6ab'
        Module = 'CExplosiveTrailDef'
        Source = 'CExplosiveTrailDef_GetSizeofClass_0044c6ab.cpp'
        TestSource = 'CExplosiveTrailDef_GetSizeofClass_0044c6ab_test.cpp'
        PassPattern = 'CExplosiveTrailDef_0044c6ab_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f619'
        Module = 'CThingMultiArrowShotDef'
        Source = 'CThingMultiArrowShotDef_GetSizeofClass_0044f619.cpp'
        TestSource = 'CThingMultiArrowShotDef_GetSizeofClass_0044f619_test.cpp'
        PassPattern = 'CThingMultiArrowShotDef_0044f619_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c6b0'
        Module = '_global'
        Source = 'global_GFGetPreMainMemoryUsed_0044c6b0.cpp'
        TestSource = 'global_GFGetPreMainMemoryUsed_0044c6b0_test.cpp'
        PassPattern = '_global_0044c6b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c71f'
        Module = '_global'
        Source = 'global_operatornew_0044c71f.cpp'
        TestSource = 'global_operatornew_0044c71f_test.cpp'
        PassPattern = '_global_0044c71f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451763'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451763.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451763_test.cpp'
        PassPattern = 'CActiveFile_00451763_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004517a4'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004517a4.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004517a4_test.cpp'
        PassPattern = 'CActiveFile_004517a4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c160'
        Module = 'CThingComponentSet'
        Source = 'CThingComponentSet_CThingComponentSet_0044c160.cpp'
        TestSource = 'CThingComponentSet_CThingComponentSet_0044c160_test.cpp'
        PassPattern = 'CThingComponentSet_0044c160_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f6a0'
        Module = 'CThingComponentSet'
        Source = 'CThingComponentSet_CThingComponentSet_0044f6a0.cpp'
        TestSource = 'CThingComponentSet_CThingComponentSet_0044f6a0_test.cpp'
        PassPattern = 'CThingComponentSet_0044f6a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450ba0'
        Module = 'CThingComponentSet'
        Source = 'CThingComponentSet_CThingComponentSet_00450ba0.cpp'
        TestSource = 'CThingComponentSet_CThingComponentSet_00450ba0_test.cpp'
        PassPattern = 'CThingComponentSet_00450ba0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c180'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044c180.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044c180_test.cpp'
        PassPattern = 'Dest_val_0044c180_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c1a0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044c1a0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044c1a0_test.cpp'
        PassPattern = '_Dest_val_0044c1a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f6e0'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044f6e0.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044f6e0_test.cpp'
        PassPattern = 'DestVal_0044f6e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f7fe'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f7fe.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f7fe_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f7fe_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fa51'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fa51.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fa51_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fa51_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450a3c'
        Module = 'vector<CIntelligentPointer<CThing_const_> '
        Source = 'vectorCIntelligentPointerCThingconst_Destroy_00450a3c.cpp'
        TestSource = 'vectorCIntelligentPointerCThingconst_Destroy_00450a3c_test.cpp'
        PassPattern = 'vecdestroy_00450a3c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045266b'
        Module = 'CHitLocationsDef'
        Source = 'CHitLocationsDef_GetSizeofClass_0045266b.cpp'
        TestSource = 'CHitLocationsDef_GetSizeofClass_0045266b_test.cpp'
        PassPattern = 'CHitLocationsDef_0045266b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453884'
        Module = 'CDegradableDef'
        Source = 'CDegradableDef_GetSizeofClass_00453884.cpp'
        TestSource = 'CDegradableDef_GetSizeofClass_00453884_test.cpp'
        PassPattern = 'CDegradableDef_00453884_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453e80'
        Module = 'CCombatAbilityBlockUnarmedAttackDef'
        Source = 'CCombatAbilityBlockUnarmedAttackDef_GetSizeofClass_00453e80.cpp'
        TestSource = 'CCombatAbilityBlockUnarmedAttackDef_GetSizeofClass_00453e80_test.cpp'
        PassPattern = 'CCombatAbilityBlockUnarmedAttackDef_00453e80_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453e9a'
        Module = 'CCombatAbilityBlockLightWeaponAttackDef'
        Source = 'CCombatAbilityBlockLightWeaponAttackDef_GetSizeofClass_00453e9a.cpp'
        TestSource = 'CCombatAbilityBlockLightWeaponAttackDef_GetSizeofClass_00453e9a_test.cpp'
        PassPattern = 'CCombatAbilityBlockLightWeaponAttackDef_00453e9a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453eb4'
        Module = 'CCombatAbilityBlockHeavyWeaponAttackDef'
        Source = 'CCombatAbilityBlockHeavyWeaponAttackDef_GetSizeofClass_00453eb4.cpp'
        TestSource = 'CCombatAbilityBlockHeavyWeaponAttackDef_GetSizeofClass_00453eb4_test.cpp'
        PassPattern = 'CCombatAbilityBlockHeavyWeaponAttackDef_00453eb4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453ece'
        Module = 'CCombatAbilityBlockProjectileWeaponAttackDef'
        Source = 'CCombatAbilityBlockProjectileWeaponAttackDef_GetSizeofClass_00453ece.cpp'
        TestSource = 'CCombatAbilityBlockProjectileWeaponAttackDef_GetSizeofClass_00453ece_test.cpp'
        PassPattern = 'CCombatAbilityBlockProjectileWeaponAttackDef_00453ece_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454643'
        Module = 'CSpecialEffectsDef'
        Source = 'CSpecialEffectsDef_GetSizeofClass_00454643.cpp'
        TestSource = 'CSpecialEffectsDef_GetSizeofClass_00454643_test.cpp'
        PassPattern = 'CSpecialEffectsDef_00454643_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004536a9'
        Module = 'CThingMultiArrowShotDef'
        Source = 'CThingMultiArrowShotDef_Copy_004536a9.cpp'
        TestSource = 'CThingMultiArrowShotDef_Copy_004536a9_test.cpp'
        PassPattern = 'CThingMultiArrowShotDef_004536a9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453705'
        Module = 'CInventoryItemDef'
        Source = 'CInventoryItemDef_Copy_00453705.cpp'
        TestSource = 'CInventoryItemDef_Copy_00453705_test.cpp'
        PassPattern = 'CInventoryItemDef_00453705_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004537a0'
        Module = 'CPhysicsDef'
        Source = 'CPhysicsDef_Copy_004537a0.cpp'
        TestSource = 'CPhysicsDef_Copy_004537a0_test.cpp'
        PassPattern = 'CPhysicsDef_004537a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004537e4'
        Module = 'CTCNoiseDef'
        Source = 'CTCNoiseDef_CopyCTCNoiseDefUAEXPBVCDefClassBaseZ_004537e4.cpp'
        TestSource = 'CTCNoiseDef_CopyCTCNoiseDefUAEXPBVCDefClassBaseZ_004537e4_test.cpp'
        PassPattern = 'CTCNoiseDef_004537e4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453810'
        Module = 'CCarrySlotDef'
        Source = 'CCarrySlotDef_Copy_00453810.cpp'
        TestSource = 'CCarrySlotDef_Copy_00453810_test.cpp'
        PassPattern = 'CCarrySlotDef_00453810_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453899'
        Module = 'CActionUseDef'
        Source = 'CActionUseDef_Copy_00453899.cpp'
        TestSource = 'CActionUseDef_Copy_00453899_test.cpp'
        PassPattern = 'CActionUseDef_00453899_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045390c'
        Module = 'CHeroMarriageDef'
        Source = 'CHeroMarriageDef_Copy_0045390c.cpp'
        TestSource = 'CHeroMarriageDef_Copy_0045390c_test.cpp'
        PassPattern = 'CHeroMarriageDef_0045390c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453b48'
        Module = 'CCreatureStatsDef'
        Source = 'CCreatureStatsDef_Copy_00453b48.cpp'
        TestSource = 'CCreatureStatsDef_Copy_00453b48_test.cpp'
        PassPattern = 'CCreatureStatsDef_00453b48_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453be3'
        Module = 'CCoopSpiritDef'
        Source = 'CCoopSpiritDef_Copy_00453be3.cpp'
        TestSource = 'CCoopSpiritDef_Copy_00453be3_test.cpp'
        PassPattern = 'CCoopSpiritDef_00453be3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453c1b'
        Module = 'CContextSensitiveItemDef'
        Source = 'CContextSensitiveItemDef_Copy_00453c1b.cpp'
        TestSource = 'CContextSensitiveItemDef_Copy_00453c1b_test.cpp'
        PassPattern = 'CContextSensitiveItemDef_00453c1b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453c53'
        Module = 'CBonusItemDef'
        Source = 'CBonusItemDef_Copy_00453c53.cpp'
        TestSource = 'CBonusItemDef_Copy_00453c53_test.cpp'
        PassPattern = 'CBonusItemDef_00453c53_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453ce5'
        Module = 'CHeroTitleDef'
        Source = 'CHeroTitleDef_Copy_00453ce5.cpp'
        TestSource = 'CHeroTitleDef_Copy_00453ce5_test.cpp'
        PassPattern = 'CHeroTitleDef_00453ce5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453d41'
        Module = 'CTattooDef'
        Source = 'CTattooDef_Copy_00453d41.cpp'
        TestSource = 'CTattooDef_Copy_00453d41_test.cpp'
        PassPattern = 'CTattooDef_00453d41_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453f5e'
        Module = 'CCombatAbilityStrafeDef'
        Source = 'CCombatAbilityStrafeDef_Copy_00453f5e.cpp'
        TestSource = 'CCombatAbilityStrafeDef_Copy_00453f5e_test.cpp'
        PassPattern = 'CCombatAbilityStrafeDef_00453f5e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453f8e'
        Module = 'CCombatAbilityUseProjectileWeaponDef'
        Source = 'CCombatAbilityUseProjectileWeaponDef_Copy_00453f8e.cpp'
        TestSource = 'CCombatAbilityUseProjectileWeaponDef_Copy_00453f8e_test.cpp'
        PassPattern = 'CCombatAbilityUseProjectileWeaponDef_00453f8e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453fc4'
        Module = 'CDragonActionSwoopDef'
        Source = 'CDragonActionSwoopDef_Copy_00453fc4.cpp'
        TestSource = 'CDragonActionSwoopDef_Copy_00453fc4_test.cpp'
        PassPattern = 'CDragonActionSwoopDef_00453fc4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454038'
        Module = 'CDragonActionNapalmDef'
        Source = 'CDragonActionNapalmDef_Copy_00454038.cpp'
        TestSource = 'CDragonActionNapalmDef_Copy_00454038_test.cpp'
        PassPattern = 'CDragonActionNapalmDef_00454038_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454083'
        Module = 'CAugmentationDef'
        Source = 'CAugmentationDef_Copy_00454083.cpp'
        TestSource = 'CAugmentationDef_Copy_00454083_test.cpp'
        PassPattern = 'CAugmentationDef_00454083_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004540ec'
        Module = 'CJackDragonDef'
        Source = 'CJackDragonDef_Copy_004540ec.cpp'
        TestSource = 'CJackDragonDef_Copy_004540ec_test.cpp'
        PassPattern = 'CJackDragonDef_004540ec_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045417e'
        Module = 'CBriarRoseDef'
        Source = 'CBriarRoseDef_Copy_0045417e.cpp'
        TestSource = 'CBriarRoseDef_Copy_0045417e_test.cpp'
        PassPattern = 'CBriarRoseDef_0045417e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004541da'
        Module = 'CSnowTrollDef'
        Source = 'CSnowTrollDef_Copy_004541da.cpp'
        TestSource = 'CSnowTrollDef_Copy_004541da_test.cpp'
        PassPattern = 'CSnowTrollDef_004541da_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454267'
        Module = 'CSummonerDef'
        Source = 'CSummonerDef_Copy_00454267.cpp'
        TestSource = 'CSummonerDef_Copy_00454267_test.cpp'
        PassPattern = 'CSummonerDef_00454267_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004542cf'
        Module = 'CExplosiveTrailDef'
        Source = 'CExplosiveTrailDef_Copy_004542cf.cpp'
        TestSource = 'CExplosiveTrailDef_Copy_004542cf_test.cpp'
        PassPattern = 'CExplosiveTrailDef_004542cf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045442f'
        Module = 'CDegradableDef'
        Source = 'CDegradableDef_Copy_0045442f.cpp'
        TestSource = 'CDegradableDef_Copy_0045442f_test.cpp'
        PassPattern = 'CDegradableDef_0045442f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045473a'
        Module = 'CVillagePeopleDef'
        Source = 'CVillagePeopleDef_Copy_0045473a.cpp'
        TestSource = 'CVillagePeopleDef_Copy_0045473a_test.cpp'
        PassPattern = 'CVillagePeopleDef_0045473a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004525e3'
        Module = 'CVillagePeopleDef'
        Source = 'CVillagePeopleDef_GetSizeofClass_004525e3.cpp'
        TestSource = 'CVillagePeopleDef_GetSizeofClass_004525e3_test.cpp'
        PassPattern = 'CVillagePeopleDef_004525e3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004538e5'
        Module = 'CTargetingDef'
        Source = 'CTargetingDef_GetSizeofClass_004538e5.cpp'
        TestSource = 'CTargetingDef_GetSizeofClass_004538e5_test.cpp'
        PassPattern = 'CTargetingDef_004538e5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452593'
        Module = 'CSpecialAbilitiesDrainLifeDataDef'
        Source = 'CSpecialAbilitiesDrainLifeDataDef_Transfer_00452593.cpp'
        TestSource = 'CSpecialAbilitiesDrainLifeDataDef_Transfer_00452593_test.cpp'
        PassPattern = 'CSpecialAbilitiesDrainLifeDataDef_00452593_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452b04'
        Module = 'CHairCardDef'
        Source = 'CHairCardDef_Transfer_00452b04.cpp'
        TestSource = 'CHairCardDef_Transfer_00452b04_test.cpp'
        PassPattern = 'CHairCardDef_00452b04_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452ccd'
        Module = 'CDragonActionHoverDef'
        Source = 'CDragonActionHoverDef_Transfer_00452ccd.cpp'
        TestSource = 'CDragonActionHoverDef_Transfer_00452ccd_test.cpp'
        PassPattern = 'CDragonActionHoverDef_00452ccd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452df1'
        Module = 'CAbilityDef'
        Source = 'CAbilityDef_Transfer_00452df1.cpp'
        TestSource = 'CAbilityDef_Transfer_00452df1_test.cpp'
        PassPattern = 'CAbilityDef_00452df1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045185f'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0045185f.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0045185f_test.cpp'
        PassPattern = 'CActiveFile_0045185f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451940'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451940.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451940_test.cpp'
        PassPattern = 'CActiveFile_00451940_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451981'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451981.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451981_test.cpp'
        PassPattern = 'CActiveFile_00451981_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004519c2'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004519c2.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004519c2_test.cpp'
        PassPattern = 'CActiveFile_004519c2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451a03'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451a03.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451a03_test.cpp'
        PassPattern = 'CActiveFile_00451a03_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451a85'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451a85.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451a85_test.cpp'
        PassPattern = 'CActiveFile_00451a85_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451b4b'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00451b4b.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00451b4b_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00451b4b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452d4d'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00452d4d.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00452d4d_test.cpp'
        PassPattern = 'CEngineLightingManager_00452d4d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f700'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044f700.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044f700_test.cpp'
        PassPattern = 'DestVal_0044f700_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004509f8'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004509f8.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004509f8_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004509f8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450a58'
        Module = 'vector<CIntelligentPointer<CThing_const_> '
        Source = 'vectorCIntelligentPointerCThingconst_Destroy_00450a58.cpp'
        TestSource = 'vectorCIntelligentPointerCThingconst_Destroy_00450a58_test.cpp'
        PassPattern = 'vecCIP_00450a58_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450a74'
        Module = 'vector<CIntelligentPointer<CThing_const_>'
        Source = 'vectorCIntelligentPointerCThingconst_Destroy_00450a74.cpp'
        TestSource = 'vectorCIntelligentPointerCThingconst_Destroy_00450a74_test.cpp'
        PassPattern = 'vector_00450a74_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450ab2'
        Module = 'vector<CIntelligentPointer<CThing_const_> '
        Source = 'vectorCIntelligentPointerCThingconst_Destroy_00450ab2.cpp'
        TestSource = 'vectorCIntelligentPointerCThingconst_Destroy_00450ab2_test.cpp'
        PassPattern = 'vecdestroy_00450ab2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450ace'
        Module = 'vector<CIntelligentPointer<CThing_const_> '
        Source = 'vectorCIntelligentPointerCThingconst_Destroy_00450ace.cpp'
        TestSource = 'vectorCIntelligentPointerCThingconst_Destroy_00450ace_test.cpp'
        PassPattern = 'vector_CIntelligentPointer_00450ace_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450be3'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00450be3.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00450be3_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00450be3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c1f6'
        Module = 'CCarrySlotDef'
        Source = 'CCarrySlotDef_GetSizeofClass_0044c1f6.cpp'
        TestSource = 'CCarrySlotDef_GetSizeofClass_0044c1f6_test.cpp'
        PassPattern = 'CCarrySlotDef_0044c1f6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c2e2'
        Module = 'CHeroMarriageDef'
        Source = 'CHeroMarriageDef_GetSizeofClass_0044c2e2.cpp'
        TestSource = 'CHeroMarriageDef_GetSizeofClass_0044c2e2_test.cpp'
        PassPattern = 'CHeroMarriageDef_0044c2e2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004509d9'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004509d9.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004509d9_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004509d9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f6ba'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f6ba.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f6ba_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f6ba_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044faba'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044faba.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044faba_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044faba_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450b79'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00450b79.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00450b79_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00450b79_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450855'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_00450855.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_00450855_test.cpp'
        PassPattern = 'CGuiControlTreePane_00450855_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004516e7'
        Module = 'CTCEnemy'
        Source = 'CTCEnemy_SetFaction_004516e7.cpp'
        TestSource = 'CTCEnemy_SetFaction_004516e7_test.cpp'
        PassPattern = 'CTCEnemy_004516e7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450bba'
        Module = '_Dest_val<std::allocator<std::pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>_>_>,std'
        Source = 'DestvalstdallocatorstdpairEAnimComponentCIVCountedPointerCAnimComponentBasestd_pairEAnimComponentCIVCountedPointerCAnimComponen_00450bba.cpp'
        TestSource = 'DestvalstdallocatorstdpairEAnimComponentCIVCountedPointerCAnimComponentBasestd_pairEAnimComponentCIVCountedPointerCAnimComponen_00450bba_test.cpp'
        PassPattern = 'Destval_00450bba_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450e92'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00450e92.cpp'
        TestSource = 'CTCCarryable_OnKill_00450e92_test.cpp'
        PassPattern = 'CTCCarryable_00450e92_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450edf'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00450edf.cpp'
        TestSource = 'CTCCarryable_OnKill_00450edf_test.cpp'
        PassPattern = 'CTCCarryable_00450edf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450f50'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00450f50.cpp'
        TestSource = 'CTCCarryable_OnKill_00450f50_test.cpp'
        PassPattern = 'CTCCarryable_00450f50_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450fac'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00450fac.cpp'
        TestSource = 'CTCCarryable_OnKill_00450fac_test.cpp'
        PassPattern = 'CTCCarryable_00450fac_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450ff9'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00450ff9.cpp'
        TestSource = 'CTCCarryable_OnKill_00450ff9_test.cpp'
        PassPattern = 'CTCCarryable_00450ff9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451046'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00451046.cpp'
        TestSource = 'CTCCarryable_OnKill_00451046_test.cpp'
        PassPattern = 'CTCCarryable_00451046_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004510bf'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_004510bf.cpp'
        TestSource = 'CTCCarryable_OnKill_004510bf_test.cpp'
        PassPattern = 'CTCCarryable_004510bf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045110c'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0045110c.cpp'
        TestSource = 'CTCCarryable_OnKill_0045110c_test.cpp'
        PassPattern = 'CTCCarryable_0045110c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451159'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00451159.cpp'
        TestSource = 'CTCCarryable_OnKill_00451159_test.cpp'
        PassPattern = 'CTCCarryable_00451159_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004511a6'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_004511a6.cpp'
        TestSource = 'CTCCarryable_OnKill_004511a6_test.cpp'
        PassPattern = 'CTCCarryable_004511a6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004511f3'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_004511f3.cpp'
        TestSource = 'CTCCarryable_OnKill_004511f3_test.cpp'
        PassPattern = 'CTCCarryable_004511f3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451240'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00451240.cpp'
        TestSource = 'CTCCarryable_OnKill_00451240_test.cpp'
        PassPattern = 'CTCCarryable_00451240_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045128d'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_0045128d.cpp'
        TestSource = 'CTCCarryable_OnKill_0045128d_test.cpp'
        PassPattern = 'CTCCarryable_0045128d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004512da'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_004512da.cpp'
        TestSource = 'CTCCarryable_OnKill_004512da_test.cpp'
        PassPattern = 'CTCCarryable_004512da_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f3b2'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0044f3b2.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0044f3b2_test.cpp'
        PassPattern = 'DestVal_0044f3b2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004506c0'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_004506c0.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_004506c0_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_004506c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004506ed'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_004506ed.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_004506ed_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_004506ed_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045071a'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0045071a.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0045071a_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_0045071a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450747'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00450747.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00450747_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00450747_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450774'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00450774.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00450774_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00450774_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004507a1'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_004507a1.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_004507a1_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_004507a1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004507ce'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_004507ce.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_004507ce_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_004507ce_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004507fb'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_004507fb.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_004507fb_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_004507fb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450828'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00450828.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00450828_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00450828_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045087c'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_0045087c.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_0045087c_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_0045087c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450903'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00450903.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00450903_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00450903_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450930'
        Module = 'LTextTreeWalkThrough'
        Source = 'LTextTreeWalkThrough_BuildTreeArray_00450930.cpp'
        TestSource = 'LTextTreeWalkThrough_BuildTreeArray_00450930_test.cpp'
        PassPattern = 'LTextTreeWalkThrough_00450930_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045095d'
        Module = 'CEngineInternalPrimitiveMeshBase'
        Source = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0045095d.cpp'
        TestSource = 'CEngineInternalPrimitiveMeshBase_GetMeshEffect_0045095d_test.cpp'
        PassPattern = 'CEngineInternalPrimitiveMeshBase_0045095d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f71b'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f71b.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f71b_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f71b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f753'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f753.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f753_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f753_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f90f'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f90f.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f90f_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f90f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fa19'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fa19.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fa19_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fa19_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f7c3'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f7c3.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f7c3_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f7c3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450142'
        Module = 'CCountedPointer<std::vector<CCharString'
        Source = 'CCountedPointerstdvectorCCharString_Reset_00450142.cpp'
        TestSource = 'CCountedPointerstdvectorCCharString_Reset_00450142_test.cpp'
        PassPattern = 'CCountedPointer_00450142_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c41e'
        Module = 'CCombatAbilityData'
        Source = 'CCombatAbilityData_TransferBinaryOut_0044c41e.cpp'
        TestSource = 'CCombatAbilityData_TransferBinaryOut_0044c41e_test.cpp'
        PassPattern = 'CCombatAbilityData_0044c41e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fd16'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd16.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd16_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fd16_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fd55'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd55.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd55_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fd55_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fd94'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd94.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fd94_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fd94_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f65a'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f65a.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044f65a_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044f65a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fe5f'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fe5f.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fe5f_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fe5f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044fea5'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fea5.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fea5_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fea5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044feeb'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044feeb.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044feeb_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044feeb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044ff31'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ff31.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ff31_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044ff31_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044ff77'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ff77.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ff77_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044ff77_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044ffbd'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ffbd.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044ffbd_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044ffbd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044be30'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044be30.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044be30_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044be30_TEST_PASS'
    }
    [pscustomobject]@{
        Address = '0044fe12'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fe12.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044fe12_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044fe12_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004525a3'
        Module = 'CSpecialAbilitiesForcePushDataDef'
        Source = 'CSpecialAbilitiesForcePushDataDef_Transfer_004525a3.cpp'
        TestSource = 'CSpecialAbilitiesForcePushDataDef_Transfer_004525a3_test.cpp'
        PassPattern = 'CSpecialAbilitiesForcePushDataDef_004525a3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004518a0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004518a0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004518a0_test.cpp'
        PassPattern = 'CActiveFile_004518a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451a44'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451a44.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451a44_test.cpp'
        PassPattern = 'CActiveFile_00451a44_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451ac6'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451ac6.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451ac6_test.cpp'
        PassPattern = 'CActiveFile_00451ac6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004543af'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004543af.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004543af_test.cpp'
        PassPattern = 'CActiveFile_004543af_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453aae'
        Module = 'CSpecialAbilitiesDrainLifeDataDef'
        Source = 'CSpecialAbilitiesDrainLifeDataDef_Copy_00453aae.cpp'
        TestSource = 'CSpecialAbilitiesDrainLifeDataDef_Copy_00453aae_test.cpp'
        PassPattern = 'CSpecialAbilitiesDrainLifeDataDef_00453aae_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453dc7'
        Module = 'CHairCardDef'
        Source = 'CHairCardDef_Copy_00453dc7.cpp'
        TestSource = 'CHairCardDef_Copy_00453dc7_test.cpp'
        PassPattern = 'CHairCardDef_00453dc7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453e51'
        Module = 'CAIStateGroup_HoverWanderAroundThing'
        Source = 'CAIStateGroupHoverWanderAroundThing_CAIStateGroupHoverWanderAroundThing_00453e51.cpp'
        TestSource = 'CAIStateGroupHoverWanderAroundThing_CAIStateGroupHoverWanderAroundThing_00453e51_test.cpp'
        PassPattern = 'CAIStateGroup_HoverWanderAroundThing_00453e51_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045406a'
        Module = 'CDragonActionHoverDef'
        Source = 'CDragonActionHoverDef_Copy_0045406a.cpp'
        TestSource = 'CDragonActionHoverDef_Copy_0045406a_test.cpp'
        PassPattern = 'CDragonActionHoverDef_0045406a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004540d3'
        Module = 'CAbilityDef'
        Source = 'CAbilityDef_Copy_004540d3.cpp'
        TestSource = 'CAbilityDef_Copy_004540d3_test.cpp'
        PassPattern = 'CAbilityDef_004540d3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453af9'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00453af9.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00453af9_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00453af9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453ba8'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00453ba8.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00453ba8_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00453ba8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004545ee'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004545ee.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004545ee_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004545ee_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004534a6'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_004534a6.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_004534a6_test.cpp'
        PassPattern = 'CDisplayManager_004534a6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453536'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_00453536.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_00453536_test.cpp'
        PassPattern = 'CDisplayManager_00453536_CopyBackBufferToTexture_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451c83'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00451c83.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00451c83_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00451c83_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453bc4'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00453bc4.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00453bc4_test.cpp'
        PassPattern = 'CTCInventoryItem_00453bc4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453ed2'
        Module = 'CCombatAbilityFlourishCounterAttackDef'
        Source = 'CCombatAbilityFlourishCounterAttackDef_Copy_00453ed2.cpp'
        TestSource = 'CCombatAbilityFlourishCounterAttackDef_Copy_00453ed2_test.cpp'
        PassPattern = 'CCombatAbilityFlourishCounterAttackDef_00453ed2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453f3f'
        Module = 'CCombatAbilityGetHitCounterAttackDef'
        Source = 'CCombatAbilityGetHitCounterAttackDef_Copy_00453f3f.cpp'
        TestSource = 'CCombatAbilityGetHitCounterAttackDef_Copy_00453f3f_test.cpp'
        PassPattern = 'CCombatAbilityGetHitCounterAttackDef_00453f3f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454248'
        Module = 'CGuildMasterDef'
        Source = 'CGuildMasterDef_Copy_00454248.cpp'
        TestSource = 'CGuildMasterDef_Copy_00454248_test.cpp'
        PassPattern = 'CGuildMasterDef_00454248_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045460a'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_0045460a.cpp'
        TestSource = 'CTCInventoryItem_OnDie_0045460a_test.cpp'
        PassPattern = 'CTCInventoryItem_0045460a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454690'
        Module = 'CTextureReplacementDef'
        Source = 'CTextureReplacementDef_GetSizeofClass_00454690.cpp'
        TestSource = 'CTextureReplacementDef_GetSizeofClass_00454690_test.cpp'
        PassPattern = 'CTextureReplacementDef_00454690_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452b93'
        Module = 'CCombatAbilityStrafeDef'
        Source = 'CCombatAbilityStrafeDef_TransferCCombatAbilityStrafeDefUAEXAAVCPersistCo_00452b93.cpp'
        TestSource = 'CCombatAbilityStrafeDef_TransferCCombatAbilityStrafeDefUAEXAAVCPersistCo_00452b93_test.cpp'
        PassPattern = 'CCombatAbilityStrafeDef_00452b93_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004530bb'
        Module = 'CGuildMasterDef'
        Source = 'CGuildMasterDef_TransferCGuildMasterDefUAEXAAVCPersistContextZ_004530bb.cpp'
        TestSource = 'CGuildMasterDef_TransferCGuildMasterDefUAEXAAVCPersistContextZ_004530bb_test.cpp'
        PassPattern = 'CGuildMasterDef_004530bb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004538eb'
        Module = 'CApprenticeSpeedTest'
        Source = 'CApprenticeSpeedTest_OnPersist_004538eb.cpp'
        TestSource = 'CApprenticeSpeedTest_OnPersist_004538eb_test.cpp'
        PassPattern = 'CApprenticeSpeedTest_004538eb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045322c'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0045322c.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0045322c_test.cpp'
        PassPattern = 'CActiveFile_0045322c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004532c3'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004532c3.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004532c3_test.cpp'
        PassPattern = 'CActiveFile_004532c3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004532e6'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004532e6.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004532e6_test.cpp'
        PassPattern = 'CActiveFile_004532e6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453327'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00453327.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00453327_test.cpp'
        PassPattern = 'CActiveFile_00453327_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045334a'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0045334a.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0045334a_test.cpp'
        PassPattern = 'CActiveFile_0045334a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004533e3'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004533e3.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004533e3_test.cpp'
        PassPattern = 'CActiveFile_004533e3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453406'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00453406.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00453406_test.cpp'
        PassPattern = 'CActiveFile_00453406_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453483'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00453483.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00453483_test.cpp'
        PassPattern = 'CActiveFile_00453483_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004545ca'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004545ca.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004545ca_test.cpp'
        PassPattern = 'CActiveFile_004545ca_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453842'
        Module = 'CSmashableDef'
        Source = 'CSmashableDef_Copy_00453842.cpp'
        TestSource = 'CSmashableDef_Copy_00453842_test.cpp'
        PassPattern = 'CSmashableDef_00453842_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004537e9'
        Module = 'CBalverineBattleDef'
        Source = 'CBalverineBattleDef_operator_004537e9.cpp'
        TestSource = 'CBalverineBattleDef_operator_004537e9_test.cpp'
        PassPattern = 'CBalverineBattleDef_004537e9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453ef1'
        Module = 'CCombatAbilityMeleeAttackData'
        Source = 'CCombatAbilityMeleeAttackData_operator_00453ef1.cpp'
        TestSource = 'CCombatAbilityMeleeAttackData_operator_00453ef1_test.cpp'
        PassPattern = 'CCombatAbilityMeleeAttackData_00453ef1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453f18'
        Module = 'CCombatAbilityData'
        Source = 'CCombatAbilityData_operator_00453f18.cpp'
        TestSource = 'CCombatAbilityData_operator_00453f18_test.cpp'
        PassPattern = 'CCombatAbilityData_00453f18_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453607'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00453607.cpp'
        TestSource = 'CTCCarryable_OnKill_00453607_test.cpp'
        PassPattern = 'CTCCarryable_00453607_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453630'
        Module = 'CTCCarryable'
        Source = 'CTCCarryable_OnKill_00453630.cpp'
        TestSource = 'CTCCarryable_OnKill_00453630_test.cpp'
        PassPattern = 'CTCCarryable_00453630_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454647'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454647.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454647_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00454647_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045470e'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045470e.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045470e_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045470e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452184'
        Module = 'CTCNoiseDef'
        Source = 'CTCNoiseDef_TransferCTCNoiseDefUAEXAAVCPersistContextZ_00452184.cpp'
        TestSource = 'CTCNoiseDef_TransferCTCNoiseDefUAEXAAVCPersistContextZ_00452184_test.cpp'
        PassPattern = 'CTCNoiseDef_00452184_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004522c6'
        Module = 'CSmashableDef'
        Source = 'CSmashableDef_TransferCSmashableDefUAEXAAVCPersistContextZ_004522c6.cpp'
        TestSource = 'CSmashableDef_TransferCSmashableDefUAEXAAVCPersistContextZ_004522c6_test.cpp'
        PassPattern = 'CSmashableDef_004522c6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452bb4'
        Module = 'CCombatAbilityUseProjectileWeaponDef'
        Source = 'CCombatAbilityUseProjectileWeaponDef_TransferCCombatAbilityUseProjectileWeaponDefUAEX_00452bb4.cpp'
        TestSource = 'CCombatAbilityUseProjectileWeaponDef_TransferCCombatAbilityUseProjectileWeaponDefUAEX_00452bb4_test.cpp'
        PassPattern = 'CCombatAbilityUseProjectileWeaponDef_00452bb4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00453b15'
        Module = 'CCompressedMemoryDataInputStream'
        Source = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_00453b15.cpp'
        TestSource = 'CCompressedMemoryDataInputStream_CCompressedMemoryDataInputStream_00453b15_test.cpp'
        PassPattern = 'CCompressedMemoryDataInputStream_00453b15_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004518e1'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004518e1.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004518e1_test.cpp'
        PassPattern = 'CActiveFile_004518e1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045228f'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_0045228f.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_0045228f_test.cpp'
        PassPattern = 'CEngineLightingManager_0045228f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452256'
        Module = 'CCarrySlotDef'
        Source = 'CCarrySlotDef_TransferCCarrySlotDefUAEXAAVCPersistContextZ_00452256.cpp'
        TestSource = 'CCarrySlotDef_TransferCCarrySlotDefUAEXAAVCPersistContextZ_00452256_test.cpp'
        PassPattern = 'CCarrySlotDef_00452256_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452b14'
        Module = 'CCombatAbilityAttackBase'
        Source = 'CCombatAbilityAttackBase_TransferCCombatAbilityAttackBaseUAEXAAVCPersistC_00452b14.cpp'
        TestSource = 'CCombatAbilityAttackBase_TransferCCombatAbilityAttackBaseUAEXAAVCPersistC_00452b14_test.cpp'
        PassPattern = 'CCombatAbilityAttackBase_00452b14_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452c94'
        Module = 'CDragonActionNapalmDef'
        Source = 'CDragonActionNapalmDef_TransferCDragonActionNapalmDefUAEXAAVCPersistCon_00452c94.cpp'
        TestSource = 'CDragonActionNapalmDef_TransferCDragonActionNapalmDefUAEXAAVCPersistCon_00452c94_test.cpp'
        PassPattern = 'CDragonActionNapalmDef_00452c94_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004543c6'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004543c6.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004543c6_test.cpp'
        PassPattern = 'CActiveFile_004543c6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451e59'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00451e59.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00451e59_test.cpp'
        PassPattern = 'CEngineLightingManager_00451e59_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451efd'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00451efd.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00451efd_test.cpp'
        PassPattern = 'CEngineLightingManager_00451efd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451fa1'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00451fa1.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00451fa1_test.cpp'
        PassPattern = 'CEngineLightingManager_00451fa1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452045'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00452045.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00452045_test.cpp'
        PassPattern = 'CEngineLightingManager_00452045_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452086'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00452086.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00452086_test.cpp'
        PassPattern = 'CEngineLightingManager_00452086_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004521b2'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004521b2.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004521b2_test.cpp'
        PassPattern = 'CEngineLightingManager_004521b2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004522f4'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004522f4.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004522f4_test.cpp'
        PassPattern = 'CEngineLightingManager_004522f4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004526f7'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004526f7.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004526f7_test.cpp'
        PassPattern = 'CEngineLightingManager_004526f7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045290a'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_0045290a.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_0045290a_test.cpp'
        PassPattern = 'CEngineLightingManager_0045290a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452a60'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00452a60.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00452a60_test.cpp'
        PassPattern = 'CEngineLightingManager_00452a60_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451b07'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00451b07.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00451b07_test.cpp'
        PassPattern = 'CActiveFile_00451b07_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045266f'
        Module = 'CCoopSpiritDef'
        Source = 'CCoopSpiritDef_TransferCCoopSpiritDefUAEXAAVCPersistContextZ_0045266f.cpp'
        TestSource = 'CCoopSpiritDef_TransferCCoopSpiritDefUAEXAAVCPersistContextZ_0045266f_test.cpp'
        PassPattern = 'CCoopSpiritDef_0045266f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004526b3'
        Module = 'CContextSensitiveItemDef'
        Source = 'CContextSensitiveItemDef_TransferCContextSensitiveItemDefUAEXAAVCPersistC_004526b3.cpp'
        TestSource = 'CContextSensitiveItemDef_TransferCContextSensitiveItemDefUAEXAAVCPersistC_004526b3_test.cpp'
        PassPattern = 'CContextSensitiveItemDef_004526b3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00452b4d'
        Module = 'CScrollingComponent'
        Source = 'CScrollingComponent_ChangeState_00452b4d.cpp'
        TestSource = 'CScrollingComponent_ChangeState_00452b4d_test.cpp'
        PassPattern = 'CScrollingComponent_00452b4d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00451e11'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00451e11.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00451e11_test.cpp'
        PassPattern = 'CEngineLightingManager_00451e11_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045212a'
        Module = 'CPhysicsDef'
        Source = 'CPhysicsDef_TransferCPhysicsDefUAEXAAVCPersistContextZ_0045212a.cpp'
        TestSource = 'CPhysicsDef_TransferCPhysicsDefUAEXAAVCPersistContextZ_0045212a_test.cpp'
        PassPattern = 'CPhysicsDef_0045212a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454f11'
        Module = 'CThingSwitchDef'
        Source = 'CThingSwitchDef_GetSizeofClass_00454f11.cpp'
        TestSource = 'CThingSwitchDef_GetSizeofClass_00454f11_test.cpp'
        PassPattern = 'CThingSwitchDef_00454f11_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455584'
        Module = 'CInventoryCategoryDef'
        Source = 'CInventoryCategoryDef_GetSizeofClass_00455584.cpp'
        TestSource = 'CInventoryCategoryDef_GetSizeofClass_00455584_test.cpp'
        PassPattern = 'CInventoryCategoryDef_00455584_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004556ca'
        Module = 'CMessageEventDef'
        Source = 'CMessageEventDef_GetSizeofClass_004556ca.cpp'
        TestSource = 'CMessageEventDef_GetSizeofClass_004556ca_test.cpp'
        PassPattern = 'CMessageEventDef_004556ca_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455c60'
        Module = 'CEnvironmentThemeDaySetDef'
        Source = 'CEnvironmentThemeDaySetDef_GetSizeofClass_00455c60.cpp'
        TestSource = 'CEnvironmentThemeDaySetDef_GetSizeofClass_00455c60_test.cpp'
        PassPattern = 'CEnvironmentThemeDaySetDef_00455c60_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455ca7'
        Module = 'CEngineLocalDetailGeneratorDef'
        Source = 'CEngineLocalDetailGeneratorDef_GetSizeofClass_00455ca7.cpp'
        TestSource = 'CEngineLocalDetailGeneratorDef_GetSizeofClass_00455ca7_test.cpp'
        PassPattern = 'CEngineLocalDetailGeneratorDef_00455ca7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455e8c'
        Module = 'CSimVoicesDef'
        Source = 'CSimVoicesDef_GetSizeofClass_00455e8c.cpp'
        TestSource = 'CSimVoicesDef_GetSizeofClass_00455e8c_test.cpp'
        PassPattern = 'CSimVoicesDef_00455e8c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455ec6'
        Module = 'CSoundThemeDef'
        Source = 'CSoundThemeDef_GetSizeofClass_00455ec6.cpp'
        TestSource = 'CSoundThemeDef_GetSizeofClass_00455ec6_test.cpp'
        PassPattern = 'CSoundThemeDef_00455ec6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455ff0'
        Module = 'CCreatureGenerationFamilyDef'
        Source = 'CCreatureGenerationFamilyDef_GetSizeofClass_00455ff0.cpp'
        TestSource = 'CCreatureGenerationFamilyDef_GetSizeofClass_00455ff0_test.cpp'
        PassPattern = 'CCreatureGenerationFamilyDef_00455ff0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004564b1'
        Module = 'CEntitySoundDef'
        Source = 'CEntitySoundDef_GetSizeofClass_004564b1.cpp'
        TestSource = 'CEntitySoundDef_GetSizeofClass_004564b1_test.cpp'
        PassPattern = 'CEntitySoundDef_004564b1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456511'
        Module = 'CFactionDef'
        Source = 'CFactionDef_GetSizeofClass_00456511.cpp'
        TestSource = 'CFactionDef_GetSizeofClass_00456511_test.cpp'
        PassPattern = 'CFactionDef_00456511_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004567bb'
        Module = 'CCameraManagerDef'
        Source = 'CCameraManagerDef_GetSizeofClass_004567bb.cpp'
        TestSource = 'CCameraManagerDef_GetSizeofClass_004567bb_test.cpp'
        PassPattern = 'CCameraManagerDef_004567bb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456814'
        Module = 'CCameraManagerSetDef'
        Source = 'CCameraManagerSetDef_GetSizeofClass_00456814.cpp'
        TestSource = 'CCameraManagerSetDef_GetSizeofClass_00456814_test.cpp'
        PassPattern = 'CCameraManagerSetDef_00456814_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045687f'
        Module = 'CHeroPostcardGeneratorDef'
        Source = 'CHeroPostcardGeneratorDef_GetSizeofClass_0045687f.cpp'
        TestSource = 'CHeroPostcardGeneratorDef_GetSizeofClass_0045687f_test.cpp'
        PassPattern = 'CHeroPostcardGeneratorDef_0045687f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456cfb'
        Module = 'COpinionReactionMaskDef'
        Source = 'COpinionReactionMaskDef_GetSizeofClass_00456cfb.cpp'
        TestSource = 'COpinionReactionMaskDef_GetSizeofClass_00456cfb_test.cpp'
        PassPattern = 'COpinionReactionMaskDef_00456cfb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456d3f'
        Module = 'COpinionDeedMaskDef'
        Source = 'COpinionDeedMaskDef_GetSizeofClass_00456d3f.cpp'
        TestSource = 'COpinionDeedMaskDef_GetSizeofClass_00456d3f_test.cpp'
        PassPattern = 'COpinionDeedMaskDef_00456d3f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456f23'
        Module = 'CVillagerInteractionsDef'
        Source = 'CVillagerInteractionsDef_GetSizeofClass_00456f23.cpp'
        TestSource = 'CVillagerInteractionsDef_GetSizeofClass_00456f23_test.cpp'
        PassPattern = 'CVillagerInteractionsDef_00456f23_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456fb8'
        Module = 'COpinionPersonalityDef'
        Source = 'COpinionPersonalityDef_GetSizeofClass_00456fb8.cpp'
        TestSource = 'COpinionPersonalityDef_GetSizeofClass_00456fb8_test.cpp'
        PassPattern = 'COpinionPersonalityDef_00456fb8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454b88'
        Module = 'CThingBuildingDef'
        Source = 'CThingBuildingDef_GetSizeofClass_00454b88.cpp'
        TestSource = 'CThingBuildingDef_GetSizeofClass_00454b88_test.cpp'
        PassPattern = 'CThingBuildingDef_00454b88_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454dc5'
        Module = 'CThingHolySiteDef'
        Source = 'CThingHolySiteDef_GetSizeofClass_00454dc5.cpp'
        TestSource = 'CThingHolySiteDef_GetSizeofClass_00454dc5_test.cpp'
        PassPattern = 'CThingHolySiteDef_00454dc5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454e1b'
        Module = 'CThingMarkerDef'
        Source = 'CThingMarkerDef_GetSizeofClass_00454e1b.cpp'
        TestSource = 'CThingMarkerDef_GetSizeofClass_00454e1b_test.cpp'
        PassPattern = 'CThingMarkerDef_00454e1b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454f59'
        Module = 'CThingVillageDef'
        Source = 'CThingVillageDef_GetSizeofClass_00454f59.cpp'
        TestSource = 'CThingVillageDef_GetSizeofClass_00454f59_test.cpp'
        PassPattern = 'CThingVillageDef_00454f59_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454fbd'
        Module = 'CThingPhysicalSwitchDef'
        Source = 'CThingPhysicalSwitchDef_GetSizeofClass_00454fbd.cpp'
        TestSource = 'CThingPhysicalSwitchDef_GetSizeofClass_00454fbd_test.cpp'
        PassPattern = 'CThingPhysicalSwitchDef_00454fbd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004554a4'
        Module = 'CInventoryDef'
        Source = 'CInventoryDef_GetSizeofClass_004554a4.cpp'
        TestSource = 'CInventoryDef_GetSizeofClass_004554a4_test.cpp'
        PassPattern = 'CInventoryDef_004554a4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455893'
        Module = 'CSkyDef'
        Source = 'CSkyDef_GetSizeofClass_00455893.cpp'
        TestSource = 'CSkyDef_GetSizeofClass_00455893_test.cpp'
        PassPattern = 'CSkyDef_00455893_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004558d9'
        Module = 'CEnvironmentDef'
        Source = 'CEnvironmentDef_GetSizeofClass_004558d9.cpp'
        TestSource = 'CEnvironmentDef_GetSizeofClass_004558d9_test.cpp'
        PassPattern = 'CEnvironmentDef_004558d9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455d22'
        Module = 'CMaterialDef'
        Source = 'CMaterialDef_GetSizeofClass_00455d22.cpp'
        TestSource = 'CMaterialDef_GetSizeofClass_00455d22_test.cpp'
        PassPattern = 'CMaterialDef_00455d22_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455ddd'
        Module = 'CCombatTypeDef'
        Source = 'CCombatTypeDef_GetSizeofClass_00455ddd.cpp'
        TestSource = 'CCombatTypeDef_GetSizeofClass_00455ddd_test.cpp'
        PassPattern = 'CCombatTypeDef_00455ddd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455f14'
        Module = 'CThingNoiseDef'
        Source = 'CThingNoiseDef_GetSizeofClass_00455f14.cpp'
        TestSource = 'CThingNoiseDef_GetSizeofClass_00455f14_test.cpp'
        PassPattern = 'CThingNoiseDef_00455f14_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456087'
        Module = 'CHeroAbilityDef'
        Source = 'CHeroAbilityDef_GetSizeofClass_00456087.cpp'
        TestSource = 'CHeroAbilityDef_GetSizeofClass_00456087_test.cpp'
        PassPattern = 'CHeroAbilityDef_00456087_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045617c'
        Module = 'CHeroStatsDef'
        Source = 'CHeroStatsDef_GetSizeofClass_0045617c.cpp'
        TestSource = 'CHeroStatsDef_GetSizeofClass_0045617c_test.cpp'
        PassPattern = 'CHeroStatsDef_0045617c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004564e5'
        Module = 'CRegionDef'
        Source = 'CRegionDef_GetSizeofClass_004564e5.cpp'
        TestSource = 'CRegionDef_GetSizeofClass_004564e5_test.cpp'
        PassPattern = 'CRegionDef_004564e5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045655e'
        Module = 'CCameraModeDef'
        Source = 'CCameraModeDef_GetSizeofClass_0045655e.cpp'
        TestSource = 'CCameraModeDef_GetSizeofClass_0045655e_test.cpp'
        PassPattern = 'CCameraModeDef_0045655e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004569a1'
        Module = 'CExpressionDef'
        Source = 'CExpressionDef_GetSizeofClass_004569a1.cpp'
        TestSource = 'CExpressionDef_GetSizeofClass_004569a1_test.cpp'
        PassPattern = 'CExpressionDef_004569a1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456d9e'
        Module = 'COpinionDeedEffectsDef'
        Source = 'COpinionDeedEffectsDef_GetSizeofClass_00456d9e.cpp'
        TestSource = 'COpinionDeedEffectsDef_GetSizeofClass_00456d9e_test.cpp'
        PassPattern = 'COpinionDeedEffectsDef_00456d9e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456df5'
        Module = 'CCombatDialogueDef'
        Source = 'CCombatDialogueDef_GetSizeofClass_00456df5.cpp'
        TestSource = 'CCombatDialogueDef_GetSizeofClass_00456df5_test.cpp'
        PassPattern = 'CCombatDialogueDef_00456df5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457028'
        Module = 'COpinionSourceDef'
        Source = 'COpinionSourceDef_GetSizeofClass_00457028.cpp'
        TestSource = 'COpinionSourceDef_GetSizeofClass_00457028_test.cpp'
        PassPattern = 'COpinionSourceDef_00457028_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457095'
        Module = 'CSpecialAbilitiesAssassinRushDef'
        Source = 'CSpecialAbilitiesAssassinRushDef_GetSizeofClass_00457095.cpp'
        TestSource = 'CSpecialAbilitiesAssassinRushDef_GetSizeofClass_00457095_test.cpp'
        PassPattern = 'CSpecialAbilitiesAssassinRushDef_00457095_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457110'
        Module = 'CSpecialAbilitiesBattleChargeDef'
        Source = 'CSpecialAbilitiesBattleChargeDef_GetSizeofClass_00457110.cpp'
        TestSource = 'CSpecialAbilitiesBattleChargeDef_GetSizeofClass_00457110_test.cpp'
        PassPattern = 'CSpecialAbilitiesBattleChargeDef_00457110_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004571e8'
        Module = 'CSpecialAbilitiesMultiStrikeDef'
        Source = 'CSpecialAbilitiesMultiStrikeDef_GetSizeofClass_004571e8.cpp'
        TestSource = 'CSpecialAbilitiesMultiStrikeDef_GetSizeofClass_004571e8_test.cpp'
        PassPattern = 'CSpecialAbilitiesMultiStrikeDef_004571e8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457285'
        Module = 'CSpecialAbilitiesMultiArrowDef'
        Source = 'CSpecialAbilitiesMultiArrowDef_GetSizeofClass_00457285.cpp'
        TestSource = 'CSpecialAbilitiesMultiArrowDef_GetSizeofClass_00457285_test.cpp'
        PassPattern = 'CSpecialAbilitiesMultiArrowDef_00457285_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004573db'
        Module = 'CSpecialAbilitiesBerserkDef'
        Source = 'CSpecialAbilitiesBerserkDef_GetSizeofClass_004573db.cpp'
        TestSource = 'CSpecialAbilitiesBerserkDef_GetSizeofClass_004573db_test.cpp'
        PassPattern = 'CSpecialAbilitiesBerserkDef_004573db_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004575da'
        Module = 'CSpecialAbilitiesBulletTimeDef'
        Source = 'CSpecialAbilitiesBulletTimeDef_GetSizeofClass_004575da.cpp'
        TestSource = 'CSpecialAbilitiesBulletTimeDef_GetSizeofClass_004575da_test.cpp'
        PassPattern = 'CSpecialAbilitiesBulletTimeDef_004575da_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004576b9'
        Module = 'CSpecialAbilitiesForcePushDef'
        Source = 'CSpecialAbilitiesForcePushDef_GetSizeofClass_004576b9.cpp'
        TestSource = 'CSpecialAbilitiesForcePushDef_GetSizeofClass_004576b9_test.cpp'
        PassPattern = 'CSpecialAbilitiesForcePushDef_004576b9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454b4c'
        Module = '_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager'
        Source = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_00454b4c.cpp'
        TestSource = 'DestvalstdallocatorstdpairlongCMusicManagerCRegisteredMusicEntrystdpairlongCMusi_CRegisteredMusicEntry_00454b4c_test.cpp'
        PassPattern = '_Dest_val_00454b4c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455e59'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00455e59.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00455e59_test.cpp'
        PassPattern = 'CTCInventoryItem_00455e59_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456531'
        Module = '_Dest_val<std::allocator<std::pair<CCharString,CSymbolMap::CSymbolInfo>_>,std::pair<CCharString,CSymbolMap'
        Source = 'DestvalstdallocatorstdpairCCharStringCSymbolMapCSymbolInfostdpairCCharStringCSym_CSymbolInfo_00456531.cpp'
        TestSource = 'DestvalstdallocatorstdpairCCharStringCSymbolMapCSymbolInfostdpairCCharStringCSym_CSymbolInfo_00456531_test.cpp'
        PassPattern = '_Dest_val_00456531_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004549b2'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004549b2.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004549b2_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004549b2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454a96'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a96.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a96_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00454a96_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004554aa'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004554aa.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004554aa_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004554aa_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045569c'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045569c.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045569c_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045569c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045585a'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045585a.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045585a_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045585a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455d28'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455d28.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455d28_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00455d28_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455de3'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455de3.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455de3_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00455de3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455e90'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455e90.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455e90_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00455e90_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455ff4'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455ff4.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00455ff4_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00455ff4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045608d'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045608d.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045608d_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045608d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456182'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456182.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456182_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456182_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004563ad'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004563ad.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004563ad_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004563ad_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004563fe'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004563fe.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004563fe_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004563fe_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456515'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456515.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456515_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456515_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456779'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456779.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456779_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456779_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004567bf'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004567bf.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004567bf_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004567bf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456851'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456851.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456851_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456851_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456948'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456948.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456948_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456948_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456cc5'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456cc5.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456cc5_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456cc5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456da4'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456da4.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456da4_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456da4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456eed'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456eed.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456eed_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456eed_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456f55'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456f55.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456f55_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456f55_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456fbc'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456fbc.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00456fbc_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00456fbc_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045702e'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045702e.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045702e_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045702e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045709b'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045709b.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045709b_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045709b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004570b7'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_004570b7.cpp'
        TestSource = 'CTCInventoryItem_OnDie_004570b7_test.cpp'
        PassPattern = 'CTCInventoryItem_004570b7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457116'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457116.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457116_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457116_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004571ee'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004571ee.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004571ee_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004571ee_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045728b'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045728b.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045728b_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045728b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004573e1'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004573e1.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004573e1_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004573e1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004575e0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004575e0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004575e0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004575e0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004576bf'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004576bf.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004576bf_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004576bf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454f15'
        Module = 'CThingSwitchDef'
        Source = 'CThingSwitchDef_Transfer_00454f15.cpp'
        TestSource = 'CThingSwitchDef_Transfer_00454f15_test.cpp'
        PassPattern = 'CThingSwitchDef_00454f15_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454f5f'
        Module = 'CThingVillageDef'
        Source = 'CThingVillageDef_Transfer_00454f5f.cpp'
        TestSource = 'CThingVillageDef_Transfer_00454f5f_test.cpp'
        PassPattern = 'CThingVillageDef_00454f5f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454ad1'
        Module = 'CDisplayManager'
        Source = 'CDisplayManager_CopyBackBufferToTexture_00454ad1.cpp'
        TestSource = 'CDisplayManager_CopyBackBufferToTexture_00454ad1_test.cpp'
        PassPattern = 'CDisplayManager_00454ad1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454993'
        Module = 'CHitLocationsDef'
        Source = 'CHitLocationsDef_Copy_00454993.cpp'
        TestSource = 'CHitLocationsDef_Copy_00454993_test.cpp'
        PassPattern = 'CHitLocationsDef_00454993_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454ab2'
        Module = 'CCombatAbilityBlockCounterAttackDef'
        Source = 'CCombatAbilityBlockCounterAttackDef_Copy_00454ab2.cpp'
        TestSource = 'CCombatAbilityBlockCounterAttackDef_Copy_00454ab2_test.cpp'
        PassPattern = 'CCombatAbilityBlockCounterAttackDef_00454ab2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456244'
        Module = 'CTextureReplacementDef'
        Source = 'CTextureReplacementDef_Copy_00456244.cpp'
        TestSource = 'CTextureReplacementDef_Copy_00456244_test.cpp'
        PassPattern = 'CTextureReplacementDef_00456244_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045645d'
        Module = 'CSystemRegistry'
        Source = 'CSystemRegistry_CSystemRegistry_0045645d.cpp'
        TestSource = 'CSystemRegistry_CSystemRegistry_0045645d_test.cpp'
        PassPattern = 'CSystemRegistry_0045645d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455f1a'
        Module = 'CThingNoiseDef'
        Source = 'CThingNoiseDef_Transfer_00455f1a.cpp'
        TestSource = 'CThingNoiseDef_Transfer_00455f1a_test.cpp'
        PassPattern = 'CThingNoiseDef_00455f1a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456dc0'
        Module = '_Dest_val<std'
        Source = 'Destvalstd_allocatorCMacroCMacro_00456dc0.cpp'
        TestSource = 'Destvalstd_allocatorCMacroCMacro_00456dc0_test.cpp'
        PassPattern = '_Dest_val_std_00456dc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455899'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455899.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455899_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00455899_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455c1d'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455c1d.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455c1d_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00455c1d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455c64'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455c64.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455c64_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00455c64_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455cab'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455cab.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455cab_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00455cab_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455eca'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455eca.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00455eca_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00455eca_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456cff'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00456cff.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00456cff_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00456cff_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456d43'
        Module = 'CEngineSubPrimitiveText'
        Source = 'CEngineSubPrimitiveText_scalardeletingdestructor_00456d43.cpp'
        TestSource = 'CEngineSubPrimitiveText_scalardeletingdestructor_00456d43_test.cpp'
        PassPattern = 'CEngineSubPrimitiveText_00456d43_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456fd8'
        Module = 'COpinionPersonalityDef'
        Source = 'COpinionPersonalityDef_COpinionPersonalityDef_00456fd8.cpp'
        TestSource = 'COpinionPersonalityDef_COpinionPersonalityDef_00456fd8_test.cpp'
        PassPattern = 'COpinionPersonalityDef_00456fd8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004567db'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004567db.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004567db_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_004567db_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004572a7'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004572a7.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004572a7_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_004572a7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454dcb'
        Module = 'CThingHolySiteDef'
        Source = 'CThingHolySiteDef_Transfer_00454dcb.cpp'
        TestSource = 'CThingHolySiteDef_Transfer_00454dcb_test.cpp'
        PassPattern = 'CThingHolySiteDef_00454dcb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045704a'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_0045704a.cpp'
        TestSource = 'CTCInventoryItem_OnDie_0045704a_test.cpp'
        PassPattern = 'CTCInventoryItem_0045704a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456f27'
        Module = 'CVillagerInteractionsDef'
        Source = 'CVillagerInteractionsDef_TransferCVillagerInteractionsDefUAEXAAVCPersistC_00456f27.cpp'
        TestSource = 'CVillagerInteractionsDef_TransferCVillagerInteractionsDefUAEXAAVCPersistC_00456f27_test.cpp'
        PassPattern = 'CVillagerInteractionsDef_00456f27_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454a00'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a00.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a00_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00454a00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454a32'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a32.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a32_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00454a32_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454a64'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a64.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00454a64_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00454a64_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004563c9'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_004563c9.cpp'
        TestSource = 'CTCInventoryItem_OnDie_004563c9_test.cpp'
        PassPattern = 'CTCInventoryItem_004563c9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004560a9'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004560a9.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004560a9_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_004560a9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456818'
        Module = 'CCameraManagerSetDef'
        Source = 'CCameraManagerSetDef_TransferCCameraManagerSetDefUAEXAAVCPersistConte_00456818.cpp'
        TestSource = 'CCameraManagerSetDef_TransferCCameraManagerSetDefUAEXAAVCPersistConte_00456818_test.cpp'
        PassPattern = 'CCameraManagerSetDef_00456818_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456883'
        Module = 'CHeroPostcardGeneratorDef'
        Source = 'CHeroPostcardGeneratorDef_TransferCHeroPostcardGeneratorDefUAEXAAVCPersist_00456883.cpp'
        TestSource = 'CHeroPostcardGeneratorDef_TransferCHeroPostcardGeneratorDefUAEXAAVCPersist_00456883_test.cpp'
        PassPattern = 'CHeroPostcardGeneratorDef_00456883_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454e21'
        Module = 'CThingMarkerDef'
        Source = 'CThingMarkerDef_Transfer_00454e21.cpp'
        TestSource = 'CThingMarkerDef_Transfer_00454e21_test.cpp'
        PassPattern = 'CThingMarkerDef_00454e21_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455dff'
        Module = 'CTCAssassinRush'
        Source = 'CTCAssassinRush_FinishMove_00455dff.cpp'
        TestSource = 'CTCAssassinRush_FinishMove_00455dff_test.cpp'
        PassPattern = 'CTCAssassinRush_00455dff_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454cfa'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00454cfa.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00454cfa_test.cpp'
        PassPattern = 'CEngineLightingManager_00454cfa_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454e5b'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00454e5b.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00454e5b_test.cpp'
        PassPattern = 'CEngineLightingManager_00454e5b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004555f8'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004555f8.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004555f8_test.cpp'
        PassPattern = 'CEngineLightingManager_004555f8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455712'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00455712.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00455712_test.cpp'
        PassPattern = 'CEngineLightingManager_00455712_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004557b6'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004557b6.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004557b6_test.cpp'
        PassPattern = 'CEngineLightingManager_004557b6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456ad9'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00456ad9.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00456ad9_test.cpp'
        PassPattern = 'CEngineLightingManager_00456ad9_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456b7d'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00456b7d.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00456b7d_test.cpp'
        PassPattern = 'CEngineLightingManager_00456b7d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456c21'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00456c21.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00456c21_test.cpp'
        PassPattern = 'CEngineLightingManager_00456c21_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004556ce'
        Module = 'CMessageEventDef'
        Source = 'CMessageEventDef_TransferCMessageEventDefUAEXAAVCPersistContextZ_004556ce.cpp'
        TestSource = 'CMessageEventDef_TransferCMessageEventDefUAEXAAVCPersistContextZ_004556ce_test.cpp'
        PassPattern = 'CMessageEventDef_004556ce_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00454fc3'
        Module = 'CThingPhysicalSwitchDef'
        Source = 'CThingPhysicalSwitchDef_Transfer_00454fc3.cpp'
        TestSource = 'CThingPhysicalSwitchDef_Transfer_00454fc3_test.cpp'
        PassPattern = 'CThingPhysicalSwitchDef_00454fc3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00456903'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_00456903.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_00456903_test.cpp'
        PassPattern = 'CEngineLightingManager_00456903_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457132'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457132.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457132_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_00457132_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004568bc'
        Module = 'CEngineLightingManager'
        Source = 'CEngineLightingManager_UpdateShadowScene_004568bc.cpp'
        TestSource = 'CEngineLightingManager_UpdateShadowScene_004568bc_test.cpp'
        PassPattern = 'CEngineLightingManager_004568bc_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045720a'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_0045720a.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_0045720a_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_0045720a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00455d44'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00455d44.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00455d44_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_00455d44_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457916'
        Module = 'CSpecialAbilitiesDrainLifeDef'
        Source = 'CSpecialAbilitiesDrainLifeDef_GetSizeofClass_00457916.cpp'
        TestSource = 'CSpecialAbilitiesDrainLifeDef_GetSizeofClass_00457916_test.cpp'
        PassPattern = 'CSpecialAbilitiesDrainLifeDef_00457916_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004579a2'
        Module = 'CSpecialAbilitiesHealLifeDef'
        Source = 'CSpecialAbilitiesHealLifeDef_GetSizeofClass_004579a2.cpp'
        TestSource = 'CSpecialAbilitiesHealLifeDef_GetSizeofClass_004579a2_test.cpp'
        PassPattern = 'CSpecialAbilitiesHealLifeDef_004579a2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457a09'
        Module = 'CSpecialAbilitiesGhostSwordDef'
        Source = 'CSpecialAbilitiesGhostSwordDef_GetSizeofClass_00457a09.cpp'
        TestSource = 'CSpecialAbilitiesGhostSwordDef_GetSizeofClass_00457a09_test.cpp'
        PassPattern = 'CSpecialAbilitiesGhostSwordDef_00457a09_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457ac4'
        Module = 'CSpecialAbilitiesTurncoatSpellDef'
        Source = 'CSpecialAbilitiesTurncoatSpellDef_GetSizeofClass_00457ac4.cpp'
        TestSource = 'CSpecialAbilitiesTurncoatSpellDef_GetSizeofClass_00457ac4_test.cpp'
        PassPattern = 'CSpecialAbilitiesTurncoatSpellDef_00457ac4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457b52'
        Module = 'CSpecialAbilitiesFireballSpellDef'
        Source = 'CSpecialAbilitiesFireballSpellDef_GetSizeofClass_00457b52.cpp'
        TestSource = 'CSpecialAbilitiesFireballSpellDef_GetSizeofClass_00457b52_test.cpp'
        PassPattern = 'CSpecialAbilitiesFireballSpellDef_00457b52_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457de3'
        Module = 'CSpecialAbilitiesSummonSpellDef'
        Source = 'CSpecialAbilitiesSummonSpellDef_GetSizeofClass_00457de3.cpp'
        TestSource = 'CSpecialAbilitiesSummonSpellDef_GetSizeofClass_00457de3_test.cpp'
        PassPattern = 'CSpecialAbilitiesSummonSpellDef_00457de3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457e57'
        Module = 'CSpecialAbilitiesBurntEffectDef'
        Source = 'CSpecialAbilitiesBurntEffectDef_GetSizeofClass_00457e57.cpp'
        TestSource = 'CSpecialAbilitiesBurntEffectDef_GetSizeofClass_00457e57_test.cpp'
        PassPattern = 'CSpecialAbilitiesBurntEffectDef_00457e57_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457eb6'
        Module = 'CSpecialAbilitiesElectrocutedEffectDef'
        Source = 'CSpecialAbilitiesElectrocutedEffectDef_GetSizeofClass_00457eb6.cpp'
        TestSource = 'CSpecialAbilitiesElectrocutedEffectDef_GetSizeofClass_00457eb6_test.cpp'
        PassPattern = 'CSpecialAbilitiesElectrocutedEffectDef_00457eb6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457f0f'
        Module = 'CSpecialAbilitiesDrunkennessDef'
        Source = 'CSpecialAbilitiesDrunkennessDef_GetSizeofClass_00457f0f.cpp'
        TestSource = 'CSpecialAbilitiesDrunkennessDef_GetSizeofClass_00457f0f_test.cpp'
        PassPattern = 'CSpecialAbilitiesDrunkennessDef_00457f0f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457f8d'
        Module = 'CSpecialAbilitiesThunderLightningStormDef'
        Source = 'CSpecialAbilitiesThunderLightningStormDef_GetSizeofClass_00457f8d.cpp'
        TestSource = 'CSpecialAbilitiesThunderLightningStormDef_GetSizeofClass_00457f8d_test.cpp'
        PassPattern = 'CSpecialAbilitiesThunderLightningStormDef_00457f8d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457738'
        Module = 'CSpecialAbilitiesEnflameDef'
        Source = 'CSpecialAbilitiesEnflameDef_GetSizeofClass_00457738.cpp'
        TestSource = 'CSpecialAbilitiesEnflameDef_GetSizeofClass_00457738_test.cpp'
        PassPattern = 'CSpecialAbilitiesEnflameDef_00457738_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457829'
        Module = 'CSpecialAbilitiesPhysicalShieldDef'
        Source = 'CSpecialAbilitiesPhysicalShieldDef_GetSizeofClass_00457829.cpp'
        TestSource = 'CSpecialAbilitiesPhysicalShieldDef_GetSizeofClass_00457829_test.cpp'
        PassPattern = 'CSpecialAbilitiesPhysicalShieldDef_00457829_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457c7e'
        Module = 'CSpecialAbilitiesLightningSpellDef'
        Source = 'CSpecialAbilitiesLightningSpellDef_GetSizeofClass_00457c7e.cpp'
        TestSource = 'CSpecialAbilitiesLightningSpellDef_GetSizeofClass_00457c7e_test.cpp'
        PassPattern = 'CSpecialAbilitiesLightningSpellDef_00457c7e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004580ef'
        Module = 'CSpecialAbilitiesDivineWrathDef'
        Source = 'CSpecialAbilitiesDivineWrathDef_GetSizeofClass_004580ef.cpp'
        TestSource = 'CSpecialAbilitiesDivineWrathDef_GetSizeofClass_004580ef_test.cpp'
        PassPattern = 'CSpecialAbilitiesDivineWrathDef_004580ef_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004582b4'
        Module = 'CSpecialAbilitiesUnholyPowerDef'
        Source = 'CSpecialAbilitiesUnholyPowerDef_GetSizeofClass_004582b4.cpp'
        TestSource = 'CSpecialAbilitiesUnholyPowerDef_GetSizeofClass_004582b4_test.cpp'
        PassPattern = 'CSpecialAbilitiesUnholyPowerDef_004582b4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045151c'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0045151c.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0045151c_test.cpp'
        PassPattern = 'CActiveFile_0045151c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00445960'
        Module = 'CShaderPreParser'
        Source = 'CShaderPreParser_GetConstantCount_00445960.cpp'
        TestSource = 'CShaderPreParser_GetConstantCount_00445960_test.cpp'
        PassPattern = 'CShaderPreParser_00445960_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00436380'
        Module = '_global'
        Source = 'global_ISTHINGALIVECThingBuilding_00436380.cpp'
        TestSource = 'global_ISTHINGALIVECThingBuilding_00436380_test.cpp'
        PassPattern = '_global_00436380_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00444d70'
        Module = 'CTCMapwho'
        Source = 'CTCMapwho_OnDie_00444d70.cpp'
        TestSource = 'CTCMapwho_OnDie_00444d70_test.cpp'
        PassPattern = 'CTCMapwho_00444d70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00450dab'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00450dab.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00450dab_test.cpp'
        PassPattern = 'CActiveFile_00450dab_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00428cbf'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_00428cbf.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_00428cbf_test.cpp'
        PassPattern = '_Dest_val_00428cbf_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eb34'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044eb34.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0044eb34_test.cpp'
        PassPattern = '_Dest_val_0044eb34_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004405d0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_004405d0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_004405d0_test.cpp'
        PassPattern = 'CActiveFile_004405d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00441670'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_00441670.cpp'
        TestSource = 'CActiveFile_OnReadFinished_00441670_test.cpp'
        PassPattern = 'CActiveFile_00441670_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457b72'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00457b72.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00457b72_test.cpp'
        PassPattern = 'CTCInventoryItem_00457b72_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457e77'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_00457e77.cpp'
        TestSource = 'CTCInventoryItem_OnDie_00457e77_test.cpp'
        PassPattern = 'CTCInventoryItem_00457e77_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042a0ce'
        Module = 'CRegion'
        Source = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042a0ce.cpp'
        TestSource = 'CRegion_SetMiniMapRegionExitTextOffsetX_0042a0ce_test.cpp'
        PassPattern = 'CRegion_0042a0ce_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ca52'
        Module = 'CObservable'
        Source = 'CObservable_AddExclusiveObserver_0041ca52.cpp'
        TestSource = 'CObservable_AddExclusiveObserver_0041ca52_test.cpp'
        PassPattern = 'CObservable_0041ca52_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cc70'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0041cc70.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_0041cc70_test.cpp'
        PassPattern = '_Dest_val_0041cc70_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045773e'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045773e.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045773e_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045773e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045775a'
        Module = 'CTCInventoryItem'
        Source = 'CTCInventoryItem_OnDie_0045775a.cpp'
        TestSource = 'CTCInventoryItem_OnDie_0045775a_test.cpp'
        PassPattern = 'CTCInventoryItem_0045775a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045782f'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045782f.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045782f_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045782f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045791a'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045791a.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0045791a_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0045791a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004579a6'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004579a6.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004579a6_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004579a6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457a67'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457a67.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457a67_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457a67_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457ac8'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457ac8.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457ac8_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457ac8_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457b56'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457b56.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457b56_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457b56_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457c84'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457c84.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457c84_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457c84_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457de7'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457de7.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457de7_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457de7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457e5b'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457e5b.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457e5b_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457e5b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457eba'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457eba.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457eba_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457eba_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457f57'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457f57.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00457f57_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00457f57_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458017'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00458017.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00458017_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00458017_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004580f5'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004580f5.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004580f5_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004580f5_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004582ba'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004582ba.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004582ba_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004582ba_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004133a0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004133a0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004133a0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004133a0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00428c1a'
        Module = '_Dest_val<std::allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>_>,CIVCountedPointer<N3DPrimitive'
        Source = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_00428c1a.cpp'
        TestSource = 'DestvalstdallocatorCIVCountedPointerN3DPrimitiveCTriangleListCIVCountedPointerN3_CTriangleList_00428c1a_test.cpp'
        PassPattern = '_Dest_val_00428c1a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0043dd40'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043dd40.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0043dd40_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0043dd40_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00442980'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442980.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442980_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00442980_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00442cc0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442cc0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442cc0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00442cc0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00442da0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442da0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442da0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00442da0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00442f00'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442f00.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00442f00_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00442f00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00443010'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443010.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443010_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00443010_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00443080'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443080.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00443080_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00443080_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044c030'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044c030.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0044c030_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0044c030_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00404f40'
        Module = 'CIME'
        Source = 'CIME_Enable_00404f40.cpp'
        TestSource = 'CIME_Enable_00404f40_test.cpp'
        PassPattern = 'CIME_00404f40_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041c9c0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041c9c0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_0041c9c0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_0041c9c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00411980'
        Module = 'CComponent'
        Source = 'CComponent_SetParentColour_00411980.cpp'
        TestSource = 'CComponent_SetParentColour_00411980_test.cpp'
        PassPattern = 'CComponent_00411980_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0040f2c0'
        Module = 'CActiveFile'
        Source = 'CActiveFile_OnReadFinished_0040f2c0.cpp'
        TestSource = 'CActiveFile_OnReadFinished_0040f2c0_test.cpp'
        PassPattern = 'CActiveFile_0040f2c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00413480'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00413480.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_00413480_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_00413480_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457ed6'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457ed6.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457ed6_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_00457ed6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00436830'
        Module = 'CTopologySizeGrid'
        Source = 'CTopologySizeGrid_Hora_00436830.cpp'
        TestSource = 'CTopologySizeGrid_Hora_00436830_test.cpp'
        PassPattern = 'CTopologySizeGrid_00436830_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004292c0'
        Module = 'CMemoryAllocatorVariableSize'
        Source = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_004292c0.cpp'
        TestSource = 'CMemoryAllocatorVariableSize_GetNoAllocatedAreas_004292c0_test.cpp'
        PassPattern = 'CMemoryAllocatorVariableSize_004292c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004038c0'
        Module = '_global'
        Source = 'global_vectorconstructoriterator_004038c0.cpp'
        TestSource = 'global_vectorconstructoriterator_004038c0_test.cpp'
        PassPattern = '_global_004038c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00447e00'
        Module = '_Dest_val<std::allocator<std::pair<unsigned_long,CCountedPointer<CFaction>_>_>,std'
        Source = 'DestvalstdallocatorstdpairunsignedlongCCountedPointerCFactionstd_pairunsignedlongCCountedPointerCFaction_00447e00.cpp'
        TestSource = 'DestvalstdallocatorstdpairunsignedlongCCountedPointerCFactionstd_pairunsignedlongCCountedPointerCFaction_00447e00_test.cpp'
        PassPattern = 'DestVal_00447e00_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041bb30'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0041bb30.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_0041bb30_test.cpp'
        PassPattern = 'DestVal_0041bb30_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00436740'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00436740.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00436740_test.cpp'
        PassPattern = 'DestVal_00436740_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00440ad0'
        Module = '_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals'
        Source = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00440ad0.cpp'
        TestSource = 'DestvalstdallocatorCCountedPointerQuadricOptimiserInternalsCOptimisedPrimitiveCC_COptimisedPrimitive_00440ad0_test.cpp'
        PassPattern = 'destval_00440ad0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004579c2'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004579c2.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004579c2_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_004579c2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004197b3'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004197b3.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004197b3_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004197b3_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457e03'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457e03.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457e03_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_00457e03_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004047d0'
        Module = 'CLandscapeBackgroundPatch'
        Source = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004047d0.cpp'
        TestSource = 'CLandscapeBackgroundPatch_vectordeletingdestructor_004047d0_test.cpp'
        PassPattern = 'CLandscapeBackgroundPatch_004047d0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004407b0'
        Module = 'CGuiControlTreePane'
        Source = 'CGuiControlTreePane_SortTreeRecursively_004407b0.cpp'
        TestSource = 'CGuiControlTreePane_SortTreeRecursively_004407b0_test.cpp'
        PassPattern = 'CGuiControlTreePane_004407b0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004576db'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004576db.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_004576db_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_004576db_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00457936'
        Module = 'CPatchTesselationEdgeStrip'
        Source = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457936.cpp'
        TestSource = 'CPatchTesselationEdgeStrip_CPatchTesselationEdgeStrip_00457936_test.cpp'
        PassPattern = 'CPatchTesselationEdgeStrip_00457936_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004120c0'
        Module = 'CThingTrackNode'
        Source = 'CThingTrackNode_vectordeletingdestructor_004120c0.cpp'
        TestSource = 'CThingTrackNode_vectordeletingdestructor_004120c0_test.cpp'
        PassPattern = 'CThingTrackNode_004120c0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004583da'
        Module = 'CSpecialAbilitiesCreatureTintDef'
        Source = 'CSpecialAbilitiesCreatureTintDef_GetSizeofClass_004583da.cpp'
        TestSource = 'CSpecialAbilitiesCreatureTintDef_GetSizeofClass_004583da_test.cpp'
        PassPattern = 'AUTO_TINY_004583da_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004584e4'
        Module = 'CAttackPatternDef'
        Source = 'CAttackPatternDef_GetSizeofClass_004584e4.cpp'
        TestSource = 'CAttackPatternDef_GetSizeofClass_004584e4_test.cpp'
        PassPattern = 'AUTO_TINY_004584e4_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045853a'
        Module = 'CObjectFamilyDef'
        Source = 'CObjectFamilyDef_GetSizeofClass_0045853a.cpp'
        TestSource = 'CObjectFamilyDef_GetSizeofClass_0045853a_test.cpp'
        PassPattern = 'AUTO_TINY_0045853a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045859d'
        Module = 'CArmourDef'
        Source = 'CArmourDef_GetSizeofClass_0045859d.cpp'
        TestSource = 'CArmourDef_GetSizeofClass_0045859d_test.cpp'
        PassPattern = 'AUTO_TINY_0045859d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004585fa'
        Module = 'CHitLocationDef'
        Source = 'CHitLocationDef_GetSizeofClass_004585fa.cpp'
        TestSource = 'CHitLocationDef_GetSizeofClass_004585fa_test.cpp'
        PassPattern = 'AUTO_TINY_004585fa_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004588c2'
        Module = 'CCombatSequenceDef'
        Source = 'CCombatSequenceDef_GetSizeofClass_004588c2.cpp'
        TestSource = 'CCombatSequenceDef_GetSizeofClass_004588c2_test.cpp'
        PassPattern = 'AUTO_TINY_004588c2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045890d'
        Module = 'CLightningDef'
        Source = 'CLightningDef_GetSizeofClass_0045890d.cpp'
        TestSource = 'CLightningDef_GetSizeofClass_0045890d_test.cpp'
        PassPattern = 'AUTO_TINY_0045890d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458c46'
        Module = 'CMeleeCombatAbilityDef'
        Source = 'CMeleeCombatAbilityDef_GetSizeofClass_00458c46.cpp'
        TestSource = 'CMeleeCombatAbilityDef_GetSizeofClass_00458c46_test.cpp'
        PassPattern = 'AUTO_TINY_00458c46_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458c99'
        Module = 'CMeleeCombatKnockdownEffects'
        Source = 'CMeleeCombatKnockdownEffects_GetSizeofClass_00458c99.cpp'
        TestSource = 'CMeleeCombatKnockdownEffects_GetSizeofClass_00458c99_test.cpp'
        PassPattern = 'AUTO_TINY_00458c99_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045934f'
        Module = 'CCreatureAbilityDef'
        Source = 'CCreatureAbilityDef_GetSizeofClass_0045934f.cpp'
        TestSource = 'CCreatureAbilityDef_GetSizeofClass_0045934f_test.cpp'
        PassPattern = 'AUTO_TINY_0045934f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459549'
        Module = 'CThingGroupDef'
        Source = 'CThingGroupDef_GetSizeofClass_00459549.cpp'
        TestSource = 'CThingGroupDef_GetSizeofClass_00459549_test.cpp'
        PassPattern = 'AUTO_TINY_00459549_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045957e'
        Module = 'CPlayerDef'
        Source = 'CPlayerDef_GetSizeofClass_0045957e.cpp'
        TestSource = 'CPlayerDef_GetSizeofClass_0045957e_test.cpp'
        PassPattern = 'AUTO_TINY_0045957e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459611'
        Module = 'CPlayerMovementDef'
        Source = 'CPlayerMovementDef_GetSizeofClass_00459611.cpp'
        TestSource = 'CPlayerMovementDef_GetSizeofClass_00459611_test.cpp'
        PassPattern = 'AUTO_TINY_00459611_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459700'
        Module = 'CSimBuildingDef'
        Source = 'CSimBuildingDef_GetSizeofClass_00459700.cpp'
        TestSource = 'CSimBuildingDef_GetSizeofClass_00459700_test.cpp'
        PassPattern = 'AUTO_TINY_00459700_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459760'
        Module = 'CEngineThemeGroupDef'
        Source = 'CEngineThemeGroupDef_GetSizeofClass_00459760.cpp'
        TestSource = 'CEngineThemeGroupDef_GetSizeofClass_00459760_test.cpp'
        PassPattern = 'AUTO_TINY_00459760_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459a9e'
        Module = 'CBrainDef'
        Source = 'CBrainDef_GetSizeofClass_00459a9e.cpp'
        TestSource = 'CBrainDef_GetSizeofClass_00459a9e_test.cpp'
        PassPattern = 'AUTO_TINY_00459a9e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459b53'
        Module = 'CPlayerInventoryDef'
        Source = 'CPlayerInventoryDef_GetSizeofClass_00459b53.cpp'
        TestSource = 'CPlayerInventoryDef_GetSizeofClass_00459b53_test.cpp'
        PassPattern = 'AUTO_TINY_00459b53_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458454'
        Module = 'CSoundDef'
        Source = 'CSoundDef_GetSizeofClass_00458454.cpp'
        TestSource = 'CSoundDef_GetSizeofClass_00458454_test.cpp'
        PassPattern = 'AUTO_TINY_00458454_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458a2d'
        Module = 'CGlobalDef'
        Source = 'CGlobalDef_GetSizeofClass_00458a2d.cpp'
        TestSource = 'CGlobalDef_GetSizeofClass_00458a2d_test.cpp'
        PassPattern = 'AUTO_TINY_00458a2d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458b7b'
        Module = 'CHeroCombatDef'
        Source = 'CHeroCombatDef_GetSizeofClass_00458b7b.cpp'
        TestSource = 'CHeroCombatDef_GetSizeofClass_00458b7b_test.cpp'
        PassPattern = 'AUTO_TINY_00458b7b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00458d69'
        Module = 'CThingShotDef'
        Source = 'CThingShotDef_GetSizeofClass_00458d69.cpp'
        TestSource = 'CThingShotDef_GetSizeofClass_00458d69_test.cpp'
        PassPattern = 'AUTO_TINY_00458d69_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459129'
        Module = 'CThingObjectDef'
        Source = 'CThingObjectDef_GetSizeofClass_00459129.cpp'
        TestSource = 'CThingObjectDef_GetSizeofClass_00459129_test.cpp'
        PassPattern = 'AUTO_TINY_00459129_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459795'
        Module = 'CEngineThemeDef'
        Source = 'CEngineThemeDef_GetSizeofClass_00459795.cpp'
        TestSource = 'CEngineThemeDef_GetSizeofClass_00459795_test.cpp'
        PassPattern = 'AUTO_TINY_00459795_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459e24'
        Module = 'CPlayerGuiDef'
        Source = 'CPlayerGuiDef_GetSizeofClass_00459e24.cpp'
        TestSource = 'CPlayerGuiDef_GetSizeofClass_00459e24_test.cpp'
        PassPattern = 'AUTO_TINY_00459e24_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00401480'
        Module = '_global'
        Source = 'global_sub00401480_00401480.cpp'
        TestSource = 'global_sub00401480_00401480_test.cpp'
        PassPattern = 'AUTO_TINY_00401480_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ca1c'
        Module = '_global'
        Source = 'global_sub0041ca1c_0041ca1c.cpp'
        TestSource = 'global_sub0041ca1c_0041ca1c_test.cpp'
        PassPattern = 'AUTO_TINY_0041ca1c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ca22'
        Module = '_global'
        Source = 'global_sub0041ca22_0041ca22.cpp'
        TestSource = 'global_sub0041ca22_0041ca22_test.cpp'
        PassPattern = 'AUTO_TINY_0041ca22_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ca6d'
        Module = '_global'
        Source = 'global_sub0041ca6d_0041ca6d.cpp'
        TestSource = 'global_sub0041ca6d_0041ca6d_test.cpp'
        PassPattern = 'AUTO_TINY_0041ca6d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041ccd7'
        Module = '_global'
        Source = 'global_sub0041ccd7_0041ccd7.cpp'
        TestSource = 'global_sub0041ccd7_0041ccd7_test.cpp'
        PassPattern = 'AUTO_TINY_0041ccd7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cd6f'
        Module = '_global'
        Source = 'global_sub0041cd6f_0041cd6f.cpp'
        TestSource = 'global_sub0041cd6f_0041cd6f_test.cpp'
        PassPattern = 'AUTO_TINY_0041cd6f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cd83'
        Module = '_global'
        Source = 'global_sub0041cd83_0041cd83.cpp'
        TestSource = 'global_sub0041cd83_0041cd83_test.cpp'
        PassPattern = 'AUTO_TINY_0041cd83_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0045954d'
        Module = 'CThingGroupDef'
        Source = 'CThingGroupDef_Transfer_0045954d.cpp'
        TestSource = 'CThingGroupDef_Transfer_0045954d_test.cpp'
        PassPattern = 'AUTO_TINY_0045954d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00459764'
        Module = 'CEngineThemeGroupDef'
        Source = 'CEngineThemeGroupDef_Transfer_00459764.cpp'
        TestSource = 'CEngineThemeGroupDef_Transfer_00459764_test.cpp'
        PassPattern = 'AUTO_TINY_00459764_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004039f0'
        Module = '_global'
        Source = 'global_sub004039f0_004039f0.cpp'
        TestSource = 'global_sub004039f0_004039f0_test.cpp'
        PassPattern = 'AUTO_TINY_004039f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004197f0'
        Module = '_global'
        Source = 'global_sub004197f0_004197f0.cpp'
        TestSource = 'global_sub004197f0_004197f0_test.cpp'
        PassPattern = 'AUTO_TINY_004197f0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0041cd23'
        Module = '_global'
        Source = 'global_sub0041cd23_0041cd23.cpp'
        TestSource = 'global_sub0041cd23_0041cd23_test.cpp'
        PassPattern = 'AUTO_TINY_0041cd23_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004292ec'
        Module = '_global'
        Source = 'global_sub004292ec_004292ec.cpp'
        TestSource = 'global_sub004292ec_004292ec_test.cpp'
        PassPattern = 'AUTO_TINY_004292ec_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004292f1'
        Module = '_global'
        Source = 'global_sub004292f1_004292f1.cpp'
        TestSource = 'global_sub004292f1_004292f1_test.cpp'
        PassPattern = 'AUTO_TINY_004292f1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004292f6'
        Module = '_global'
        Source = 'global_sub004292f6_004292f6.cpp'
        TestSource = 'global_sub004292f6_004292f6_test.cpp'
        PassPattern = 'AUTO_TINY_004292f6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '004292fb'
        Module = '_global'
        Source = 'global_sub004292fb_004292fb.cpp'
        TestSource = 'global_sub004292fb_004292fb_test.cpp'
        PassPattern = 'AUTO_TINY_004292fb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00429317'
        Module = '_global'
        Source = 'global_sub00429317_00429317.cpp'
        TestSource = 'global_sub00429317_00429317_test.cpp'
        PassPattern = 'AUTO_TINY_00429317_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0042931c'
        Module = '_global'
        Source = 'global_sub0042931c_0042931c.cpp'
        TestSource = 'global_sub0042931c_0042931c_test.cpp'
        PassPattern = 'AUTO_TINY_0042931c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '00429321'
        Module = '_global'
        Source = 'global_sub00429321_00429321.cpp'
        TestSource = 'global_sub00429321_00429321_test.cpp'
        PassPattern = 'AUTO_TINY_00429321_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eebd'
        Module = '_global'
        Source = 'global_sub0044eebd_0044eebd.cpp'
        TestSource = 'global_sub0044eebd_0044eebd_test.cpp'
        PassPattern = 'AUTO_TINY_0044eebd_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eec2'
        Module = '_global'
        Source = 'global_sub0044eec2_0044eec2.cpp'
        TestSource = 'global_sub0044eec2_0044eec2_test.cpp'
        PassPattern = 'AUTO_TINY_0044eec2_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eec7'
        Module = '_global'
        Source = 'global_sub0044eec7_0044eec7.cpp'
        TestSource = 'global_sub0044eec7_0044eec7_test.cpp'
        PassPattern = 'AUTO_TINY_0044eec7_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eecc'
        Module = '_global'
        Source = 'global_sub0044eecc_0044eecc.cpp'
        TestSource = 'global_sub0044eecc_0044eecc_test.cpp'
        PassPattern = 'AUTO_TINY_0044eecc_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eed1'
        Module = '_global'
        Source = 'global_sub0044eed1_0044eed1.cpp'
        TestSource = 'global_sub0044eed1_0044eed1_test.cpp'
        PassPattern = 'AUTO_TINY_0044eed1_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eed6'
        Module = '_global'
        Source = 'global_sub0044eed6_0044eed6.cpp'
        TestSource = 'global_sub0044eed6_0044eed6_test.cpp'
        PassPattern = 'AUTO_TINY_0044eed6_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eedb'
        Module = '_global'
        Source = 'global_sub0044eedb_0044eedb.cpp'
        TestSource = 'global_sub0044eedb_0044eedb_test.cpp'
        PassPattern = 'AUTO_TINY_0044eedb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044eee0'
        Module = '_global'
        Source = 'global_sub0044eee0_0044eee0.cpp'
        TestSource = 'global_sub0044eee0_0044eee0_test.cpp'
        PassPattern = 'AUTO_TINY_0044eee0_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f03c'
        Module = '_global'
        Source = 'global_sub0044f03c_0044f03c.cpp'
        TestSource = 'global_sub0044f03c_0044f03c_test.cpp'
        PassPattern = 'AUTO_TINY_0044f03c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f08e'
        Module = '_global'
        Source = 'global_sub0044f08e_0044f08e.cpp'
        TestSource = 'global_sub0044f08e_0044f08e_test.cpp'
        PassPattern = 'AUTO_TINY_0044f08e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f1bb'
        Module = '_global'
        Source = 'global_sub0044f1bb_0044f1bb.cpp'
        TestSource = 'global_sub0044f1bb_0044f1bb_test.cpp'
        PassPattern = 'AUTO_TINY_0044f1bb_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f24f'
        Module = '_global'
        Source = 'global_sub0044f24f_0044f24f.cpp'
        TestSource = 'global_sub0044f24f_0044f24f_test.cpp'
        PassPattern = 'AUTO_TINY_0044f24f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f254'
        Module = '_global'
        Source = 'global_sub0044f254_0044f254.cpp'
        TestSource = 'global_sub0044f254_0044f254_test.cpp'
        PassPattern = 'AUTO_TINY_0044f254_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f40b'
        Module = '_global'
        Source = 'global_sub0044f40b_0044f40b.cpp'
        TestSource = 'global_sub0044f40b_0044f40b_test.cpp'
        PassPattern = 'AUTO_TINY_0044f40b_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f410'
        Module = '_global'
        Source = 'global_sub0044f410_0044f410.cpp'
        TestSource = 'global_sub0044f410_0044f410_test.cpp'
        PassPattern = 'AUTO_TINY_0044f410_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f415'
        Module = '_global'
        Source = 'global_sub0044f415_0044f415.cpp'
        TestSource = 'global_sub0044f415_0044f415_test.cpp'
        PassPattern = 'AUTO_TINY_0044f415_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f41a'
        Module = '_global'
        Source = 'global_sub0044f41a_0044f41a.cpp'
        TestSource = 'global_sub0044f41a_0044f41a_test.cpp'
        PassPattern = 'AUTO_TINY_0044f41a_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f41f'
        Module = '_global'
        Source = 'global_sub0044f41f_0044f41f.cpp'
        TestSource = 'global_sub0044f41f_0044f41f_test.cpp'
        PassPattern = 'AUTO_TINY_0044f41f_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f424'
        Module = '_global'
        Source = 'global_sub0044f424_0044f424.cpp'
        TestSource = 'global_sub0044f424_0044f424_test.cpp'
        PassPattern = 'AUTO_TINY_0044f424_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f429'
        Module = '_global'
        Source = 'global_sub0044f429_0044f429.cpp'
        TestSource = 'global_sub0044f429_0044f429_test.cpp'
        PassPattern = 'AUTO_TINY_0044f429_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f42e'
        Module = '_global'
        Source = 'global_sub0044f42e_0044f42e.cpp'
        TestSource = 'global_sub0044f42e_0044f42e_test.cpp'
        PassPattern = 'AUTO_TINY_0044f42e_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f433'
        Module = '_global'
        Source = 'global_sub0044f433_0044f433.cpp'
        TestSource = 'global_sub0044f433_0044f433_test.cpp'
        PassPattern = 'AUTO_TINY_0044f433_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f438'
        Module = '_global'
        Source = 'global_sub0044f438_0044f438.cpp'
        TestSource = 'global_sub0044f438_0044f438_test.cpp'
        PassPattern = 'AUTO_TINY_0044f438_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f43d'
        Module = '_global'
        Source = 'global_sub0044f43d_0044f43d.cpp'
        TestSource = 'global_sub0044f43d_0044f43d_test.cpp'
        PassPattern = 'AUTO_TINY_0044f43d_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f442'
        Module = '_global'
        Source = 'global_sub0044f442_0044f442.cpp'
        TestSource = 'global_sub0044f442_0044f442_test.cpp'
        PassPattern = 'AUTO_TINY_0044f442_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f447'
        Module = '_global'
        Source = 'global_sub0044f447_0044f447.cpp'
        TestSource = 'global_sub0044f447_0044f447_test.cpp'
        PassPattern = 'AUTO_TINY_0044f447_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f44c'
        Module = '_global'
        Source = 'global_sub0044f44c_0044f44c.cpp'
        TestSource = 'global_sub0044f44c_0044f44c_test.cpp'
        PassPattern = 'AUTO_TINY_0044f44c_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f451'
        Module = '_global'
        Source = 'global_sub0044f451_0044f451.cpp'
        TestSource = 'global_sub0044f451_0044f451_test.cpp'
        PassPattern = 'AUTO_TINY_0044f451_TEST PASS'
    }
    [pscustomobject]@{
        Address = '0044f456'
        Module = '_global'
        Source = 'global_sub0044f456_0044f456.cpp'
        TestSource = 'global_sub0044f456_0044f456_test.cpp'
        PassPattern = 'AUTO_TINY_0044f456_TEST PASS'
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
