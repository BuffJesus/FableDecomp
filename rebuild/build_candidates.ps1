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
