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
        Address = '00a7a650'; Module = 'CNavQuadTree'; Source = 'CNavQuadTree_LinkToNeighbouringMaps_00a7a650.cpp'
        TestSource = 'CNavQuadTree_LinkToNeighbouringMaps_00a7a650_test.cpp'; PassPattern = 'NAV_QUAD_TREE_LINK_NEIGHBOURING_MAPS_TEST PASS'
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
