# FableWin editor/tooling symbol survey

Generated: 2026-07-18T13:31:27
Source: ghidra_out\fablewin_pdb_names.tsv

## Category counts

| Category | Matches |
|---|---:|
| editor_classes | 2503 |
| level_region | 26275 |
| resource_defs | 6353 |
| terrain_landscape | 2229 |
| objects_placement | 25667 |
| chest_keys_items | 4501 |
| archives_formats | 4123 |
| console_debug | 8408 |

## Top class prefixes

| Class | Count |
|---|---:|
| CEditControlCentre | 300 |
| CThingCreatureBase | 198 |
| CThing | 183 |
| CEditWorldMap | 125 |
| CThingAICreature | 122 |
| CMap | 120 |
| CThingManager | 114 |
| CScriptThing | 80 |
| CThingPlayerCreature | 78 |
| CThingPhysical | 67 |
| CDefinitionManager | 65 |
| CEditDisplayEngine | 46 |
| CEditMap | 42 |
| CThingBuilding | 40 |
| CRegion | 40 |
| CThingTrackNode | 39 |
| CThingObject | 35 |
| CEditLevelMerger | 34 |
| CThingGameObject | 33 |
| CRegionMinimap | 32 |
| CThingSwitch | 30 |
| CEditEngine2DBase | 26 |
| CDefClassBase | 26 |
| CEditWorld | 23 |
| CEditComponent | 23 |
| CThingPhysicalSwitch | 22 |
| CResourceList | 22 |
| CThingDeadCreature | 21 |
| CThingSoundEmitter | 19 |
| CResource | 19 |
| CEditEngine2DWorldMap | 18 |
| CThingVillage | 17 |
| CThingMarker | 17 |
| CThingHolySite | 17 |
| CDefendingCombatantInfo | 16 |
| CEditInputProcessCopyPaste | 16 |
| CEditPrimitiveStats | 14 |
| CDefString | 13 |
| CDefStringTable | 12 |
| CThingNoise | 11 |

## Representative symbols

### editor_classes

| Address | Name |
|---|---|
| 018a1210 | ?IsEditorActive@CMainGameComponent@@UBE_NXZ |
| 018bde00 | ?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z |
| 018bde30 | ?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ |
| 018bde60 | ?SetEditBoxRegion@CIME@NUISystem@@QAEXJJJJ@Z |
| 01911b70 | GConsoleSetEditor |
| 01a48300 | ?Run@CEditComponent@@UAE_NPAPAVCGameComponent@@@Z |
| 01a48510 | ??0CEditComponent@@QAE@AAVCGame@@ABVCMainGameComponentInit@@@Z |
| 01a48600 | ?IsEditorActive@CEditComponent@@UBE_NXZ |
| 01a48610 | ??_GCEditComponent@@UAEPAXI@Z |
| 01a48650 | ??1CEditComponent@@UAE@XZ |
| 01a48750 | ?Init@CEditComponent@@UAEXXZ |
| 01a49bf0 | ?PreparePrimitivesForEngine@CEditComponent@@QBEXABVCEnginePrimitivePrepareInfo@@@Z |
| 01a49c60 | ?GetInputs@CEditComponent@@MAEXXZ |
| 01a49ce0 | ?Update@CEditComponent@@MAEXXZ |
| 01a49de0 | ?Render@CEditComponent@@MAEXXZ |
| 01a49f80 | ?SetAsEditingLevel@CEditComponent@@QAEXXZ |
| 01a49f90 | ?SetAsEditingWorld@CEditComponent@@QAEXXZ |
| 01a49fe0 | ?PostInit@CEditComponent@@MAEXXZ |
| 01a4a360 | ?GetEditControlCentre@CEditComponent@@QAEAAVCEditControlCentre@@XZ |
| 01a4a380 | ?EditInitWorld@CEditComponent@@IAEXXZ |
| 01a4a4a0 | ?EditInitDisplayEngine@CEditComponent@@IAEXXZ |
| 01a4a640 | ?InitGuiGraphics@CEditComponent@@IAEXXZ |
| 01a4ab50 | ??0?$scoped_ptr@VCEditControlCentre@@@boost@@QAE@PAVCEditControlCentre@@@Z |
| 01a4ab70 | ??1?$scoped_ptr@VCEditControlCentre@@@boost@@QAE@XZ |
| 01a4ab90 | ?reset@?$scoped_ptr@VCEditControlCentre@@@boost@@QAEXPAVCEditControlCentre@@@Z |
| 01a4ac10 | ??C?$scoped_ptr@VCEditControlCentre@@@boost@@QBEPAVCEditControlCentre@@XZ |
| 01a4ac30 | ?get@?$scoped_ptr@VCEditControlCentre@@@boost@@QBEPAVCEditControlCentre@@XZ |
| 01a4b130 | ?swap@?$scoped_ptr@VCEditControlCentre@@@boost@@QAEXAAV12@@Z |
| 01a4c460 | ??$checked_delete@VCEditControlCentre@@@boost@@YIXPAVCEditControlCentre@@@Z |
| 01b9c84e | ?Alloc@CTCEditor@@SIPAVCTCBase@@AAVCThing@@@Z |

### level_region

| Address | Name |
|---|---|
| 0187bf90 | ?GetAutoSaveRegionLeavingFileName@CUserProfileManager@@QBE?AVCWideString@@XZ |
| 0187c030 | ?GetAutoSaveRegionLeavingPathName@CUserProfileManager@@QBE?AVCWideString@@XZ |
| 01892910 | ?_Growmap@?$deque@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@IAEXI@Z |
| 01896640 | ?Find@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAE?AV?$_Vector_iterator@V?$_Vector_val@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$allocator@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@@2@@std@@@std@@ABW4ETCInterfaceType@@@Z |
| 018968a0 | ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAE?AV?$_Vector_iterator@V?$_Vector_val@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$allocator@U?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@@2@@std@@@std@@ABW4ETCInterfaceType@@@Z |
| 01898af0 | ?PeekPSymbolMap@CPersistContext@@QBEPBVCSymbolMap@@XZ |
| 018a5140 | ?GetPRegionDef@CRegion@@QBE?AV?$CDefPointer@$$CBVCRegionDef@@@@XZ |
| 018aa8fe | ??0?$CDefPointer@$$CBVCRegionDef@@@@QAE@ABV0@@Z |
| 018aa932 | ??1?$CDefPointer@$$CBVCRegionDef@@@@QAE@XZ |
| 018aa9ba | ?empty@?$_Tree@V?$_Tmap_traits@PAXVCBulletTimeInfo@CBulletTimeManager@@U?$less@PAX@std@@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@4@$0A@@std@@@std@@QBE_NXZ |
| 018ac3cb | ?Release@?$CDefPointer@$$CBVCRegionDef@@@@QAEXXZ |
| 018ac4f6 | ?size@?$_Tree@V?$_Tmap_traits@PAXVCBulletTimeInfo@CBulletTimeManager@@U?$less@PAX@std@@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@4@$0A@@std@@@std@@QBEIXZ |
| 018bde60 | ?SetEditBoxRegion@CIME@NUISystem@@QAEXJJJJ@Z |
| 018bf390 | ?ConsoleAudio_SetAtmosMapBlendTime@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018bf550 | ?ConsoleSetMipmapBias@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c2a50 | ??1?$map@JVCCharString@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@3@@std@@QAE@XZ |
| 018c3490 | ?ConsoleSetLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c5a10 | ?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ |
| 018c5c10 | ?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ |
| 018cccd0 | ?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z |
| 018cdb40 | ?ConsoleSetDrawMapwhoInfo@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018cff50 | ?ConsoleSetAllSpellsToLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018cffe0 | ?ConsoleSetSpellToLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d00e0 | ?ConsoleSetAllSpellsToNextLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d0150 | ?ConsoleSetSpellToNextLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d0570 | ?ConsoleSetHeroStrengthPhysiqueLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d0600 | ?ConsoleSetHeroStrengthHealthLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d0690 | ?ConsoleSetHeroStrengthToughnessLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d0720 | ?ConsoleSetHeroSkillSpeedLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d07b0 | ?ConsoleSetHeroSkillAccuracyLevel@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |

### resource_defs

| Address | Name |
|---|---|
| 01873370 | ?SetCompiledDefDirectory@CDefinitionManager@@SIXABVCWideString@@@Z |
| 01873390 | ?SetUsingCompiledDefs@CDefinitionManager@@SIX_N@Z |
| 018733b0 | ?SetCreatingCompiledDefs@CDefinitionManager@@SIX_N@Z |
| 018733d0 | ?SetCreateChecksums@CDefinitionManager@@SIX_N@Z |
| 01878430 | ?GetUIGraphics@NLocalisation@@YIXAAV?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@@Z |
| 01878740 | ??$GetDef@VCUILocaleGraphicsDef@NUISystem@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@@Z |
| 018787f0 | ??1?$CDefPointer@$$CBVCDefClassBase@@@@QAE@XZ |
| 01878810 | ?Get@?$CDefPointer@$$CBVCDefClassBase@@@@QBEPBVCDefClassBase@@XZ |
| 01878830 | ?Release@?$CDefPointer@$$CBVCDefClassBase@@@@QAEXXZ |
| 01878860 | ??$SetDynamicCast@$$CBVCDefClassBase@@@?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@QAEXABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z |
| 01878900 | ?Release@?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@QAEXXZ |
| 01878930 | ?InternalGetObject@?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@QBEPAVCDefPointeeBase@@XZ |
| 01878950 | ?InternalGetObject@?$CDefPointer@$$CBVCDefClassBase@@@@QBEPAVCDefPointeeBase@@XZ |
| 01878970 | ??$GFDebugPDynamicCast@PAVCDefPointeeBase@@PBVCUILocaleGraphicsDef@NUISystem@@@@YIPBVCUILocaleGraphicsDef@NUISystem@@PAVCDefPointeeBase@@PBV01@@Z |
| 01888a60 | ?IsAllocated@CStartupDefinitionManager@@SI_NXZ |
| 01889a70 | ?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ |
| 0188cfc0 | ??0?$CDefPointer@$$CBVCControlsDef@@@@QAE@W4ENullDef@@@Z |
| 0188cfe0 | ??1?$CDefPointer@$$CBVCControlsDef@@@@QAE@XZ |
| 0188d000 | ??C?$CDefPointer@$$CBVCControlsDef@@@@QBEPBVCControlsDef@@XZ |
| 0188daf0 | ?IsNull@?$CCountedPointer@VCStartupDefinitionManager@@@@QBE_NXZ |
| 0188ddc0 | ??0?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QAE@W4ENullDef@@@Z |
| 0188dde0 | ??0?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QAE@ABV0@@Z |
| 0188de20 | ??1?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QAE@XZ |
| 0188de40 | ??C?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QBEPBVCConfigOptionsDefaultsDef@@XZ |
| 0188df70 | ?IsNull@?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QBE_NXZ |
| 0188e220 | ??1?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QAE@XZ |
| 0188e240 | ??C?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QBEPAVCUIMiscThingsDef@NUISystem@@XZ |
| 01890960 | ?Release@?$CDefPointer@$$CBVCControlsDef@@@@QAEXXZ |
| 01890e70 | ?Release@?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QAEXXZ |
| 018910d0 | ?Release@?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QAEXXZ |

### terrain_landscape

| Address | Name |
|---|---|
| 01871210 | ?GetMaxTextureHeight@CRenderManagerCore@@QBEKXZ |
| 018858c0 | ?CaptureBackBufferForUIBackgroundIfNonExistant@CUserProfileManager@@QAEXXZ |
| 018858f0 | ?CaptureBackBufferForUIBackground@CUserProfileManager@@QAEXXZ |
| 01885a90 | ?RenderUIBackground@CUserProfileManager@@QAEXM@Z |
| 018a67f0 | ?GetHeight@CSurface@@QBEKXZ |
| 018aee90 | ?GetFontHeight@CFontBank@@QBEJXZ |
| 018b4c00 | ?GetVFontMaxHeight@CFontBank@@QBEMXZ |
| 018bc640 | ?SetHeight@CIMEFont@NUISystem@@UAEXI@Z |
| 018cda20 | ?ConsoleSetDrawNavigationMeshes@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 01a66560 | ?GetHeight@C2DBoxF@@QBEMXZ |
| 01a6a478 | ?GetBlackBarHeight@CSubtitleRenderer@@ABEMXZ |
| 01a7eca0 | ?GetHeight@CTexture@@QBEKK@Z |
| 01aad1a9 | ?GetBackBufferHeight@CUIScreenUnitConverter@NUISystem@@SIMXZ |
| 01ae38f0 | ?GetHeight@C2DBoxI@@QBEJXZ |
| 01b52570 | ?ConsoleSetFreeCameraHeightLock@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 01b525d0 | ?SetHeightLock@CCamera@@QAEX_N@Z |
| 01b52d10 | ?ConsoleSetOutputNavMeshInfo@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 01bb354f | ?Alloc@CTCShotDefinitionTearInGround@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z |
| 01bb35ca | ?StatGetClassTypeName@CTCShotDefinitionTearInGround@NThingShot@@SI?AVCCharString@@XZ |
| 01bb35fd | ??0CTCShotDefinitionTearInGround@NThingShot@@QAE@AAVCThing@@@Z |
| 01bb3627 | ?GetClassTypeName@CTCShotDefinitionTearInGround@NThingShot@@UBE?AVCCharString@@XZ |
| 01bb3654 | ??_ECTCShotDefinitionTearInGround@NThingShot@@UAEPAXI@Z |
| 01bb3684 | ??1CTCShotDefinitionTearInGround@NThingShot@@UAE@XZ |
| 01bbf599 | ?GetTerrainDesirabilityAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z |
| 01bbf5a9 | ?GetTerrainHeightAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z |
| 01bbf5b9 | ?GetThemeHeightAt@CObstacleNavigatorExternalInterface@@UBEMABVC3DVector@@@Z |
| 01c83ff0 | ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z |
| 01c85a40 | ?DrawFindLastUnblockedGroundPositionAndReportCollision@CWorldMap@@UBE_NABVC3DVector@@0AAV2@@Z |
| 01d0fde0 | ?DrawNavigationMesh@CTCGraphicAppearance@NGraphicAppearance@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@ABVCRightHandedSet@@@Z |
| 01d2cd10 | ?SetHeight@CSprite@NUISystem@@UAEXK@Z |

### objects_placement

| Address | Name |
|---|---|
| 0186c660 | ?DoEULAThings@EULA@@YIHXZ |
| 01878930 | ?InternalGetObject@?$CDefPointer@$$CBVCUILocaleGraphicsDef@NUISystem@@@@QBEPAVCDefPointeeBase@@XZ |
| 01878950 | ?InternalGetObject@?$CDefPointer@$$CBVCDefClassBase@@@@QBEPAVCDefPointeeBase@@XZ |
| 0187fc50 | ?GetPosition@CMemoryFile@@UBEJXZ |
| 01881110 | ?GetPosition@CDataInputStream@@UBEKXZ |
| 0188e220 | ??1?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QAE@XZ |
| 0188e240 | ??C?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QBEPAVCUIMiscThingsDef@NUISystem@@XZ |
| 018910d0 | ?Release@?$CDefPointer@VCUIMiscThingsDef@NUISystem@@@@QAEXXZ |
| 018962e0 | ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z |
| 01896320 | ?IsDead@CThing@@QBE_NXZ |
| 01896340 | ??$GetTC@VCTCInGameMenu@@@CThing@@QAE_NPAPAVCTCInGameMenu@@@Z |
| 018963a0 | ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z |
| 018963d0 | ?GetTC@CThing@@QAEAAVCTCBase@@W4ETCInterfaceType@@@Z |
| 0189e3e0 | ?InternalGetObject@?$CDefPointer@$$CBVCControlsDef@@@@QBEPAVCDefPointeeBase@@XZ |
| 0189e400 | ?InternalGetObject@?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@QBEPAVCDefPointeeBase@@XZ |
| 018bc7a0 | ?SetPosition@CIMEFont@NUISystem@@UAEXHH@Z |
| 018bf320 | ?ConsoleAudio_SetHeadRelativePosition@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c0bd0 | ??0CThingAICreatureInit@@QAE@PAVCThingPhysical@@0_NABVCCharString@@21@Z |
| 018c0ca0 | ??1CThingAICreatureInit@@QAE@XZ |
| 018c0d10 | ?GetPlayerNumber@CThing@@QBEKXZ |
| 018c0d30 | ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ |
| 018c1bd0 | ?ConsoleCreateObject@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c2170 | ?ConsoleMovePlayerToPosition@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c2460 | ??0CCreatureAction_NavToPos@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@@Z |
| 018c29d0 | ??1?$CIntelligentPointer@VCThing@@@@UAE@XZ |
| 018c29f0 | ??1?$CIntelligentPointer@VCThingCreatureBase@@@@UAE@XZ |
| 018c2b90 | ?ConsoleGivePlayerObject@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c3570 | ?ConsolePlayerAddClothing@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c36b0 | ?ConsolePlayerRemoveClothing@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c37b0 | ?ConsoleZoomToThingNext@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |

### chest_keys_items

| Address | Name |
|---|---|
| 01886880 | ?GetAssignedInputForContextSensitiveItemAction@CUserProfileManager@@QAE_NW4EGameAction@@_NJAAVCActionInputControl@@@Z |
| 018c3ae0 | ?EnableInventoryTest@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018c8c90 | ?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z |
| 018c8e70 | ?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ |
| 018c9f10 | ?MsgOpenedChest@CScriptThing@@UBE_NXZ |
| 018d2fd0 | ?EnableInventory@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d3010 | ?EnableInventory@CPlayerGui@@SIX_N@Z |
| 018ffea0 | ??$GetTC@VCTCInventoryItem@@@CThing@@QAE_NPAPAVCTCInventoryItem@@@Z |
| 018fff00 | ?StatGetInterfaceType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@XZ |
| 018fff10 | ??$GetTC@VCTCInventoryBase@NInventory@@@CThing@@QAE_NW4ETCInterfaceType@@PAPAVCTCInventoryBase@NInventory@@@Z |
| 01900230 | ??$GetTC@VCTCInventoryClothing@@@CThing@@QAE_NPAPAVCTCInventoryClothing@@@Z |
| 01900290 | ?StatGetInterfaceType@CTCInventoryClothing@@SI?AW4ETCInterfaceType@@XZ |
| 01900a90 | ??$GetTC@VCTCInventoryAbilities@@@CThing@@QAE_NPAPAVCTCInventoryAbilities@@@Z |
| 01900af0 | ?StatGetInterfaceType@CTCInventoryAbilities@@SI?AW4ETCInterfaceType@@XZ |
| 01901ce0 | ??$GetTC@VCTCInventoryQuests@@@CThing@@QAE_NPAPAVCTCInventoryQuests@@@Z |
| 01901d40 | ?StatGetInterfaceType@CTCInventoryQuests@@SI?AW4ETCInterfaceType@@XZ |
| 01905b00 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInventoryItem@@@@YIPAVCTCInventoryItem@@PAVCTCBase@@PAV0@@Z |
| 01905cc0 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInventoryBase@NInventory@@@@YIPAVCTCInventoryBase@NInventory@@PAVCTCBase@@PAV01@@Z |
| 01906040 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInventoryClothing@@@@YIPAVCTCInventoryClothing@@PAVCTCBase@@PAV0@@Z |
| 01906e40 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInventoryAbilities@@@@YIPAVCTCInventoryAbilities@@PAVCTCBase@@PAV0@@Z |
| 019084a0 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInventoryQuests@@@@YIPAVCTCInventoryQuests@@PAVCTCBase@@PAV0@@Z |
| 01911740 | ??0CParsedItem@@QAE@XZ |
| 01911760 | ??1CParsedItem@@QAE@XZ |
| 0191f169 | ?Alloc@CPlayerInventoryDef@@SIPAVCDefClassBase@@XZ |
| 0191f1dd | ??0CPlayerInventoryDef@@QAE@XZ |
| 0191f29c | ?Copy@CPlayerInventoryDef@@UAEXPBVCDefClassBase@@@Z |
| 0191f2b7 | ?GetSizeofClass@CPlayerInventoryDef@@UAEKXZ |
| 0191f2c8 | ?Transfer@CPlayerInventoryDef@@UAEXAAVCPersistContext@@@Z |
| 0191f357 | ??4CPlayerInventoryDef@@QAEAAV0@ABV0@@Z |
| 0191f3ed | ??_ECPlayerInventoryDef@@UAEPAXI@Z |

### archives_formats

| Address | Name |
|---|---|
| 01874a40 | ?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z |
| 01874a90 | ?TransferBinaryOut@?$CPersistTraits@_N@@QAEXAAVCDataOutputStream@@AB_N@Z |
| 01884d30 | ?ApplyNewProfileValues@CUserProfileManager@@QAEXXZ |
| 01894bc0 | ?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z |
| 01894c00 | ?TransferBinaryOut@?$CPersistTraits@M@@QAEXAAVCDataOutputStream@@ABM@Z |
| 01895bc0 | ?TransferBinaryIn@?$CPersistTraits@VCWideString@@@@QAEXAAVCMemoryDataInputStream@@AAVCWideString@@@Z |
| 01895c50 | ?TransferBinaryOut@?$CPersistTraits@VCWideString@@@@QAEXAAVCDataOutputStream@@ABVCWideString@@@Z |
| 018981e0 | ?TransferBinaryIn@?$CPersistTraits@J@@QAEXAAVCMemoryDataInputStream@@AAJ@Z |
| 01898260 | ?TransferBinaryOut@?$CPersistTraits@J@@QAEXAAVCDataOutputStream@@J@Z |
| 01898450 | ?TransferBinaryIn@?$CPersistTraits@K@@QAEXAAVCMemoryDataInputStream@@AAK@Z |
| 01898480 | ?TransferBinaryOut@?$CPersistTraits@K@@QAEXAAVCDataOutputStream@@K@Z |
| 01898c20 | ??0?$CPersistContext_TransferableVector@VCActionInputControl@@@@QAE@AAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z |
| 01898c60 | ?Size@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEJXZ |
| 01898c80 | ?Resize@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEXJ@Z |
| 01898e90 | ?TransferIn@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEXAAVCPersistContext@@AAVCStringParser@@J@Z |
| 0189a030 | ?TransferOut@?$CPersistContext_TransferableVector@VCActionInputControl@@@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z |
| 0189a980 | ??0?$CPersistContext_TransferableVector@W4ETutorialCategory@@@@QAE@AAV?$vector@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@Z |
| 0189a9c0 | ?Size@?$CPersistContext_TransferableVector@W4ETutorialCategory@@@@EAEJXZ |
| 0189a9e0 | ?Resize@?$CPersistContext_TransferableVector@W4ETutorialCategory@@@@EAEXJ@Z |
| 0189aa60 | ?TransferIn@?$CPersistContext_TransferableVector@W4ETutorialCategory@@@@EAEXAAVCPersistContext@@AAVCStringParser@@J@Z |
| 0189aaf0 | ?TransferOut@?$CPersistContext_TransferableVector@W4ETutorialCategory@@@@EAEXAAVCPersistContext@@AAVCCharString@@J@Z |
| 0189ab80 | ??0CPersistContext_TransferableVectorGenericType@@QAE@XZ |
| 0189bfe0 | ??$GFSerialiseVectorBinaryOut@VCActionInputControl@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z |
| 0189c0b0 | ?TransferBinaryOut@?$CPersistTraits@VCActionInputControl@@@@QBEXAAVCDataOutputStream@@ABVCActionInputControl@@@Z |
| 0189c0d0 | ?TransferBinaryOut@CActionInputControl@@QBEXAAVCDataOutputStream@@@Z |
| 0189c150 | ??$GFSerialiseVectorBinaryIn@VCActionInputControl@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z |
| 0189c230 | ?TransferBinaryIn@?$CPersistTraits@VCActionInputControl@@@@QAE?AVCActionInputControl@@AAVCMemoryDataInputStream@@@Z |
| 0189c280 | ?TransferBinaryIn@?$CPersistTraits@VCActionInputControl@@@@QAEXAAVCMemoryDataInputStream@@AAVCActionInputControl@@@Z |
| 0189c2a0 | ?TransferBinaryIn@CActionInputControl@@QAEXAAVCMemoryDataInputStream@@@Z |
| 0189c9a0 | ?TransferBinaryIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCMemoryDataInputStream@@AAVCCharString@@@Z |

### console_debug

| Address | Name |
|---|---|
| 0186aeb0 | DebuggerProbe |
| 0186af60 | DebuggerRuntime |
| 0186b9f0 | __ValidateImageBase |
| 01876730 | ??$_Debug_lt_pred@U?$less@D@std@@DD@std@@YI_NU?$less@D@0@AADABDPB_WI@Z |
| 018767b0 | ??$_Debug_lt_pred@U?$less@D@std@@DD@std@@YI_NU?$less@D@0@ABDAADPB_WI@Z |
| 01877380 | ??$_Debug_pointer@_W@std@@YIXPB_W0I@Z |
| 01878970 | ??$GFDebugPDynamicCast@PAVCDefPointeeBase@@PBVCUILocaleGraphicsDef@NUISystem@@@@YIPBVCUILocaleGraphicsDef@NUISystem@@PAVCDefPointeeBase@@PBV01@@Z |
| 01879810 | ??0CDebugString@@QAE@XZ |
| 01879890 | ??_GCDebugString@@UAEPAXI@Z |
| 018798d0 | ??1CDebugString@@UAE@XZ |
| 0187c0f0 | ?IsDebugProfile@CUserProfileManager@@QBE_NXZ |
| 0187d730 | ?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ |
| 0188db40 | ??$_Debug_range@PAVCWideString@@@std@@YIXPAVCWideString@@0PB_WI@Z |
| 0188db90 | ??$_Debug_pointer@VCWideString@@@std@@YIXPAVCWideString@@PB_WI@Z |
| 0188dc50 | ??$_Debug_range2@PAVCWideString@@@std@@YIXPAVCWideString@@0PB_WIUrandom_access_iterator_tag@0@@Z |
| 0189ca50 | ??$_Debug_range@V?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@std@@@std@@YIXV?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@0@0PB_WI@Z |
| 0189cdb0 | ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCInGameMenu@@@@YIPAVCTCInGameMenu@@PAVCTCBase@@PAV0@@Z |
| 0189eb50 | ??$_Debug_range2@V?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@std@@@std@@YIXV?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@0@0PB_WIUrandom_access_iterator_tag@0@@Z |
| 0189ec30 | ??$GFDebugPDynamicCast@PAVCDefPointeeBase@@PBVCControlsDef@@@@YIPBVCControlsDef@@PAVCDefPointeeBase@@PBV0@@Z |
| 0189edf0 | ??$GFDebugPDynamicCast@PAVCDefPointeeBase@@PBVCConfigOptionsDefaultsDef@@@@YIPBVCConfigOptionsDefaultsDef@@PAVCDefPointeeBase@@PBV0@@Z |
| 0189f3d0 | ??$_Debug_range@PAD@std@@YIXPAD0PB_WI@Z |
| 0189f420 | ??$_Debug_pointer@D@std@@YIXPADPB_WI@Z |
| 0189f490 | ??$_Debug_range2@PAD@std@@YIXPAD0PB_WIUrandom_access_iterator_tag@0@@Z |
| 0189f6f0 | ??$_Debug_range@PAPAW4ETutorialCategory@@@std@@YIXPAPAW4ETutorialCategory@@0PB_WI@Z |
| 0189f740 | ??$_Debug_pointer@PAW4ETutorialCategory@@@std@@YIXPAPAW4ETutorialCategory@@PB_WI@Z |
| 0189f990 | ??$_Debug_range@PAVCActionInputControl@@@std@@YIXPAVCActionInputControl@@0PB_WI@Z |
| 0189fab0 | ??$_Debug_range@PAW4ETutorialCategory@@@std@@YIXPAW4ETutorialCategory@@0PB_WI@Z |
| 0189fe00 | ??$_Debug_pointer@V?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@std@@@std@@YIXAAV?$_Vector_const_iterator@V?$_Vector_val@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@0@PB_WI@Z |
| 0189fe20 | ??$_Debug_pointer@VCActionInputControl@@@std@@YIXPAVCActionInputControl@@PB_WI@Z |
| 0189fe90 | ??$_Debug_range@V?$_Vector_const_iterator@V?$_Vector_val@VCDisplayModeDesc@@V?$allocator@VCDisplayModeDesc@@@std@@@std@@@std@@@std@@YIXV?$_Vector_const_iterator@V?$_Vector_val@VCDisplayModeDesc@@V?$allocator@VCDisplayModeDesc@@@std@@@std@@@0@0PB_WI@Z |
