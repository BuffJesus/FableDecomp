# Editor/tooling notes from FableWin symbols

Generated: 2026-07-18T13:32:48
Source: ghidra_out\fablewin_pdb_names.tsv

## Reading

FableWin.exe appears to include Lionhead's internal editor/runtime tool surface, not just
a debug build of the retail game. These names are useful in two ways:

- They can be ported through BSim to name matching retail Fable.exe engine functions.
- Editor-only code documents the native semantics behind community tools such as ChocolateBox
  and the local FQT/SilverChest WAD/TNG pipeline.

ChocolateBox is still best treated as the community GUI/front-end lineage; the FableWin symbols
are lower-level engine/editor ground truth for level save/load, TNG parsing, region graph,
definitions, and object/chest semantics.

## Editor activation and mode switching

Matches: 36

| Address | Symbol |
|---|---|
| 018a1210 | ?IsEditorActive@CMainGameComponent@@UBE_NXZ |
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
| 02029230 | ??0CEditControlCentre@@QAE@AAVCEditComponent@@AAVCEditWorld@@ABVCGameDefinitionManager@@AAVCEditDisplayEngine@@AAVCPlayerManager@@@Z |
| 02036920 | ?GetEditDisplayEngine@CEditComponent@@QAEAAVCEditDisplayEngine@@XZ |
| 02036d80 | ?GetEditGui@CEditComponent@@QAEAAVCGuiWindowManager@@XZ |
| 0204d210 | ?GetEditWorld@CEditComponent@@QAEAAVCEditWorld@@XZ |
| 0204d690 | ?PeekEditDisplayEngine@CEditComponent@@QBEAAVCEditDisplayEngine@@XZ |
| 0204d6b0 | ?PeekEditControlCentre@CEditComponent@@QBEABVCEditControlCentre@@XZ |
| 0204dfe0 | ?PeekComponent@CEditControlCentre@@QBEABVCEditComponent@@XZ |
| 0204e040 | ??0CDrawRiverWaypointGroup@CEditControlCentre@@QAE@ABVCEditComponent@@AAVCIEngine@@MM@Z |
| 020763d0 | ?PeekEditWorld@CEditComponent@@QBEABVCEditWorld@@XZ |
| 020777c0 | ?PeekEditGui@CEditComponent@@QBEABVCGuiWindowManager@@XZ |
| 0207a820 | ?PeekEditorFont@CEditComponent@@QBEABVCFontBank@@XZ |
| 0207cfb0 | ??$GFDebugPDynamicCast@PBVCMainGameComponent@@PBVCEditComponent@@@@YIPBVCEditComponent@@PBVCMainGameComponent@@PBV0@@Z |
| 0207d760 | ??0CEditWorld@@QAE@ABVCEditComponent@@ABVCWorldInit@@_N@Z |
| 025d16a0 | ??$GFDebugDynamicCast@$$CBVCMainGameComponent@@$$CBVCEditComponent@@@@YIPBVCEditComponent@@PBVCMainGameComponent@@PBV0@@Z |
| 02946a50 | ??0CMenu@NEditGui@@QAE@ABVCCharString@@AAVCEditComponent@@AAVCEditControlCentre@@@Z |
| 0296a4e0 | ??0CEditWorldMap@@QAE@ABVCEditComponent@@AAVCEditWorld@@ABVC2DBoxI@@K@Z |
| 02b35c50 | ?PeekGuiGraphicSymbols@CEditComponent@@QBEABVCSymbolMap@@XZ |

## Level save/load and world map editing

Matches: 191

| Address | Symbol |
|---|---|
| 01d36eb0 | ?RuntimeSaveLevelPersistentEntitiesInLoadedMaps@CThingManager@@AAEXXZ |
| 0202bf50 | ?GetCurrentEditWorldMap@CEditWorld@@QAEAAVCEditWorldMap@@XZ |
| 0202ebc0 | ?InitEditTransactionManager@CEditControlCentre@@AAEXAAVCEditWorldMap@@@Z |
| 0202ecb0 | ?SetEditWorldMap@CEditTransactionBase@@SIXPAVCEditWorldMap@@@Z |
| 020377e0 | ?SaveLevel@CEditControlCentre@@QAE_NJVCWideString@@_N@Z |
| 02044540 | ?PeekCurrentEditWorldMap@CEditWorld@@QBEABVCEditWorldMap@@XZ |
| 0204b5d0 | ?SaveAllLevels@CEditControlCentre@@QAE_N_N0@Z |
| 0204b6e0 | ?SetSaveToBackupFiles@CEditWorldMap@@QAEX_N@Z |
| 0204b990 | ?SaveLevelIfChanged@CEditControlCentre@@AAE_NJ_N00@Z |
| 0204c3c0 | ?CreateAndSaveNewLevel@CEditControlCentre@@QAEXABVCWideString@@ABVC2DCoordI@@ABVCMapInit@@@Z |
| 0204c940 | ?GetEditWorldMap@CEditControlCentre@@QAEAAVCEditWorldMap@@XZ |
| 0204c960 | ?PeekEditWorldMap@CEditControlCentre@@QBEABVCEditWorldMap@@XZ |
| 0207fed0 | ?LoadSingleLevelFromFiles@CEditWorldMap@@QAEXJV?$CCountedPointer@VCAFile@@@@0@Z |
| 02951450 | ?GetNearestMapCoord@CEditInputProcessMapPlacement@@ABEJMABVCEditWorldMap@@@Z |
| 02952140 | ?CreateAndSaveNewLevel@CEditInputProcessMapPlacement@@AAEXABVC2DBoxI@@@Z |
| 0296a4e0 | ??0CEditWorldMap@@QAE@ABVCEditComponent@@AAVCEditWorld@@ABVC2DBoxI@@K@Z |
| 0296a6d0 | ?PostRegionLoad@CEditWorldMap@@UAEXJ@Z |
| 0296a6e0 | ??_ECEditWorldMap@@UAEPAXI@Z |
| 0296a740 | ??1CEditWorldMap@@UAE@XZ |
| 0296a7f0 | ?DrawGetMapInfo@CEditWorldMap@@UBE?AVCDrawMapInfo@@J@Z |
| 0296a890 | ?Update@CEditWorldMap@@UAEXXZ |
| 0296ade0 | ?AddLevelFromFileWithUID@CEditWorldMap@@QAEJABVC2DCoordI@@ABVCCharString@@K@Z |
| 0296ae80 | ?RemoveLevel@CEditWorldMap@@UAEXJ@Z |
| 0296b160 | ?PlaceMap@CEditWorldMap@@UAEXJ@Z |
| 0296b1b0 | ?GetEditMap@CEditWorldMap@@UAEAAVCEditMap@@J@Z |
| 0296b200 | ?PeekEditMap@CEditWorldMap@@UBEABVCEditMap@@J@Z |
| 0296b250 | ?SetMapPos@CEditWorldMap@@QAEXJABVC2DCoordI@@@Z |
| 0296b4c0 | ?GetRegionlessMaps@CEditWorldMap@@ABEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z |
| 0296b5c0 | ?GetRegion@CEditWorldMap@@QBEABVCRegion@@J@Z |
| 0296b5e0 | ?SaveToFile@CEditWorldMap@@UAEXAAVCAFile@@@Z |
| 0296b850 | ?EditSaveRegionsToString@CEditWorldMap@@AAEXAAVCCharString@@@Z |
| 0296d140 | ?LoadFromFile@CEditWorldMap@@UAEXAAVCAFile@@J@Z |
| 0296d210 | ?DeleteAllLevels@CEditWorldMap@@UAEXXZ |
| 0296d250 | ?AllocateNewMap@CEditWorldMap@@EAEJXZ |
| 0296d2d0 | ?CreateAndSaveNewLevel@CEditWorldMap@@QAEJVCWideString@@ABVC2DCoordI@@ABVCMapInit@@@Z |
| 0296d920 | ?CreateNewRegion@CEditWorldMap@@QAEJXZ |
| 0296dc10 | ?DeleteRegion@CEditWorldMap@@QAEXJ@Z |
| 0296de90 | ?DeleteAllRegions@CEditWorldMap@@UAEXXZ |
| 0296e000 | ?GetRegion@CEditWorldMap@@QAEAAVCRegion@@J@Z |
| 0296e150 | ?SaveSingleLevel@CEditWorldMap@@QAE?AVCEditLevelSaveInfo@@JVCWideString@@_N1@Z |
| ... | 151 more omitted |

## TNG/LEV merge and conflict tooling

Matches: 344

| Address | Symbol |
|---|---|
| 028c02b0 | ?SetLogText@CEditLevelMerger@@QAEXPAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z |
| 028c02d0 | ?SetOurText@CEditLevelMerger@@QAEXPAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z |
| 028c02f0 | ?SetTheirText@CEditLevelMerger@@QAEXPAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z |
| 028c0430 | ?GetConflicts@CEditLevelMerger@@QAEAAV?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@XZ |
| 028c0850 | ??0?$CCountedPointer@VCEditLevelMerger@@@@QAE@XZ |
| 028c0880 | ??1?$CCountedPointer@VCEditLevelMerger@@@@QAE@XZ |
| 028c08a0 | ??C?$CCountedPointer@VCEditLevelMerger@@@@QBEPAVCEditLevelMerger@@XZ |
| 028c08f0 | ?IsNull@?$CCountedPointer@VCEditLevelMerger@@@@QBE_NXZ |
| 028c0940 | ?Reset@?$CCountedPointer@VCEditLevelMerger@@@@QAEXPAVCEditLevelMerger@@@Z |
| 028c0a60 | ?size@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QBEIXZ |
| 028c0a90 | ??A?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QAEAAVCConflict@CEditLevelMerger@@I@Z |
| 028c0b40 | ?clear@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QAEXXZ |
| 028c0c40 | ??1?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QAE@XZ |
| 028c0c60 | ??0?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QAE@ABV01@@Z |
| 028c0ca0 | ??1?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QAE@XZ |
| 028c0cc0 | ??0?$_Iterator012@Urandom_access_iterator_tag@std@@VCConflict@CEditLevelMerger@@HPBV34@ABV34@U_Iterator_base12@2@@std@@QAE@ABU01@@Z |
| 028c0cf0 | ??1?$_Iterator012@Urandom_access_iterator_tag@std@@VCConflict@CEditLevelMerger@@HPBV34@ABV34@U_Iterator_base12@2@@std@@QAE@XZ |
| 028c0d10 | ?DeleteData@?$CCountedPointer@VCEditLevelMerger@@@@SIXPAX@Z |
| 028c0d60 | ?begin@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QAE?AV?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@XZ |
| 028c0db0 | ?end@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QAE?AV?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@XZ |
| 028c0e00 | ?erase@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QAE?AV?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@V?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@0@Z |
| 028c1000 | ??_GCEditLevelMerger@@QAEPAXI@Z |
| 028c1040 | ?_Make_iter@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@QBE?AV?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@V?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@2@@Z |
| 028c10c0 | ?_Destroy@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@IAEXPAVCConflict@CEditLevelMerger@@0@Z |
| 028c10f0 | ?_Orphan_range@?$vector@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@IBEXPAVCConflict@CEditLevelMerger@@0@Z |
| 028c11d0 | ??0?$_Vector_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QAE@PAVCConflict@CEditLevelMerger@@PBU_Container_base12@1@@Z |
| 028c1200 | ??9?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QBE_NABV01@@Z |
| 028c1230 | ??M?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QBE_NABV01@@Z |
| 028c1270 | ??0?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QAE@PAVCConflict@CEditLevelMerger@@PBU_Container_base12@1@@Z |
| 028c12e0 | ??8?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QBE_NABV01@@Z |
| 028c1320 | ?_Compat@?$_Vector_const_iterator@V?$_Vector_val@VCConflict@CEditLevelMerger@@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@@std@@QBEXABV12@@Z |
| 028c13e0 | ??0?$_Iterator012@Urandom_access_iterator_tag@std@@VCConflict@CEditLevelMerger@@HPBV34@ABV34@U_Iterator_base12@2@@std@@QAE@XZ |
| 028c1400 | ?ReleaseData@?$CCountedPointer@VCEditLevelMerger@@@@QAEXXZ |
| 028c1550 | ??$_Move@PAVCConflict@CEditLevelMerger@@PAV12@@std@@YIPAVCConflict@CEditLevelMerger@@PAV12@00@Z |
| 028c15a0 | ??$_Destroy_range@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@YIXPAVCConflict@CEditLevelMerger@@0AAV?$allocator@VCConflict@CEditLevelMerger@@@0@@Z |
| 028c15f0 | ??$_Ptr_cat@VCConflict@CEditLevelMerger@@V12@@std@@YI?AU_Nonscalar_ptr_iterator_tag@0@PAVCConflict@CEditLevelMerger@@0@Z |
| 028c1610 | ??$_Move@PAVCConflict@CEditLevelMerger@@PAV12@@std@@YIPAVCConflict@CEditLevelMerger@@PAV12@00U_Nonscalar_ptr_iterator_tag@0@@Z |
| 028c1670 | ??$_Destroy_range@V?$allocator@VCConflict@CEditLevelMerger@@@std@@@std@@YIXPAVCConflict@CEditLevelMerger@@0AAV?$allocator@VCConflict@CEditLevelMerger@@@0@U_Nonscalar_ptr_iterator_tag@0@@Z |
| 028c16b0 | ??4CConflict@CEditLevelMerger@@QAEAAV01@ABV01@@Z |
| 028c1710 | ??4CThingText@CEditLevelMerger@@QAEAAV01@ABV01@@Z |
| ... | 304 more omitted |

## Region graph, mapwho, navigation

Matches: 658

| Address | Symbol |
|---|---|
| 018cda20 | ?ConsoleSetDrawNavigationMeshes@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018cdb40 | ?ConsoleSetDrawMapwhoInfo@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d90c0 | ?ConsoleGenerateRegionGraph@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 018d9100 | ?ConsoleLoadRegionGraphData@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 01b52ba0 | ?ConsoleSetDrawNavigationInfo@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
| 01b5fba0 | ??$PeekTC@$$CBVCTCCreatureNavigation@@@CThing@@QBE_NPAPBVCTCCreatureNavigation@@@Z |
| 01b5fc00 | ?StatGetInterfaceType@CTCCreatureNavigation@@SI?AW4ETCInterfaceType@@XZ |
| 01b67e40 | ??$GFDebugPDynamicCast@PBVCTCBase@@PBVCTCCreatureNavigation@@@@YIPBVCTCCreatureNavigation@@PBVCTCBase@@PBV0@@Z |
| 01b96f97 | ?Alloc@CCreatureNavigationDef@@SIPAVCDefClassBase@@XZ |
| 01b97007 | ??0CCreatureNavigationDef@@QAE@XZ |
| 01b9702b | ?Copy@CCreatureNavigationDef@@UAEXPBVCDefClassBase@@@Z |
| 01b97046 | ?GetSizeofClass@CCreatureNavigationDef@@UAEKXZ |
| 01b97055 | ?Transfer@CCreatureNavigationDef@@UAEXAAVCPersistContext@@@Z |
| 01b970d2 | ??4CCreatureNavigationDef@@QAEAAV0@ABV0@@Z |
| 01b9712d | ??_ECCreatureNavigationDef@@UAEPAXI@Z |
| 01b9715d | ??1CCreatureNavigationDef@@UAE@XZ |
| 01b97172 | ?Alloc@CTCCreatureNavigation@@SIPAVCTCBase@@AAVCThing@@@Z |
| 01b971ea | ?StatGetClassTypeName@CTCCreatureNavigation@@SI?AVCCharString@@XZ |
| 01ba8b4f | ?Alloc@CTCDNavigationSeed@@SIPAVCTCBase@@AAVCThing@@@Z |
| 01ba8bc7 | ?StatGetClassTypeName@CTCDNavigationSeed@@SI?AVCCharString@@XZ |
| 01ba8bfa | ?StatGetDriverType@CTCDNavigationSeed@@SIKXZ |
| 01ba8c04 | ??0CTCDNavigationSeed@@QAE@AAVCThing@@@Z |
| 01ba8c2e | ?GetInterfaceType@CTCDNavigationSeed@@UBE?AW4ETCInterfaceType@@XZ |
| 01ba8c3f | ?StatGetInterfaceType@CTCDNavigationSeed@@SI?AW4ETCInterfaceType@@XZ |
| 01ba8c49 | ?GetClassTypeName@CTCDNavigationSeed@@UBE?AVCCharString@@XZ |
| 01ba8c76 | ?IsDriverType@CTCDNavigationSeed@@UBE_NJ@Z |
| 01ba8c8f | ?IsComponentSerialisable@CTCDNavigationSeed@@UBE_NXZ |
| 01ba8c9c | ??_GCTCDNavigationSeed@@UAEPAXI@Z |
| 01ba8ccc | ??1CTCDNavigationSeed@@UAE@XZ |
| 01babfe8 | ?Alloc@CTCSwitchableNavigation@@SIPAVCTCBase@@AAVCThing@@@Z |
| 01bac060 | ?StatGetClassTypeName@CTCSwitchableNavigation@@SI?AVCCharString@@XZ |
| 01bac093 | ??0CTCSwitchableNavigation@@QAE@AAVCThing@@@Z |
| 01bac0c6 | ?GetInterfaceType@CTCSwitchableNavigation@@UBE?AW4ETCInterfaceType@@XZ |
| 01bac0d7 | ?StatGetInterfaceType@CTCSwitchableNavigation@@SI?AW4ETCInterfaceType@@XZ |
| 01bac0e1 | ?GetClassTypeName@CTCSwitchableNavigation@@UBE?AVCCharString@@XZ |
| 01bac10e | ??_ECTCSwitchableNavigation@@UAEPAXI@Z |
| 01baef61 | ?Alloc@CTCPreCalculatedNavigationRoute@@SIPAVCTCBase@@AAVCThing@@@Z |
| 01baefd9 | ?StatGetClassTypeName@CTCPreCalculatedNavigationRoute@@SI?AVCCharString@@XZ |
| 01baf00c | ??0CTCPreCalculatedNavigationRoute@@QAE@AAVCThing@@@Z |
| 01baf095 | ?GetInterfaceType@CTCPreCalculatedNavigationRoute@@UBE?AW4ETCInterfaceType@@XZ |
| ... | 618 more omitted |

## Definitions and compiled defs

Matches: 61

| Address | Symbol |
|---|---|
| 01873370 | ?SetCompiledDefDirectory@CDefinitionManager@@SIXABVCWideString@@@Z |
| 01873390 | ?SetUsingCompiledDefs@CDefinitionManager@@SIX_N@Z |
| 018733b0 | ?SetCreatingCompiledDefs@CDefinitionManager@@SIX_N@Z |
| 018a3ffa | ?ValidateDefinitions@CMainGameComponent@@IAEXXZ |
| 018a643e | ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ |
| 018a8790 | ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z |
| 018ad00d | ??$GFDebugPDynamicCast@PAVCGameDefinitionManager@@PBV1@@@YIPBVCGameDefinitionManager@@PAV0@PBV0@@Z |
| 018d4a60 | ?PeekDefinitionManager@CWorld@@QBEABVCGameDefinitionManager@@XZ |
| 01913a45 | ?Set@CGameDefinitionManager@@SIXPAV1@@Z |
| 01913a60 | ?Get@CGameDefinitionManager@@SIAAV1@XZ |
| 01913a72 | ?IsAllocated@CGameDefinitionManager@@SI_NXZ |
| 01913a8e | ??0CGameDefinitionManager@@QAE@XZ |
| 01913ccf | ??_ECGameDefinitionManager@@UAEPAXI@Z |
| 01913cff | ??1CGameDefinitionManager@@UAE@XZ |
| 01913d9d | ?InitAndCompile@CGameDefinitionManager@@UAEXXZ |
| 0194fe12 | ?ErrorCallback@CGameDefinitionManager@@KIXABVCCharString@@@Z |
| 01950caa | ??D?$CCountedPointer@VCGameDefinitionManager@@@@QBEAAVCGameDefinitionManager@@XZ |
| 01950cf2 | ?IsNull@?$CCountedPointer@VCGameDefinitionManager@@@@QBE_NXZ |
| 01950d39 | ?Reset@?$CCountedPointer@VCGameDefinitionManager@@@@QAEXPAVCGameDefinitionManager@@@Z |
| 01958e3b | ?DeleteData@?$CCountedPointer@VCGameDefinitionManager@@@@SIXPAX@Z |
| 0199df2e | ?ReleaseData@?$CCountedPointer@VCGameDefinitionManager@@@@QAEXXZ |
| 01a3a708 | ??0?$CCountedPointer@VCGameDefinitionManager@@@@QAE@XZ |
| 01a3a727 | ??1?$CCountedPointer@VCGameDefinitionManager@@@@QAE@XZ |
| 01a59ec0 | ??0CPlayerManager@@QAE@AAVCMainGameComponent@@ABVCGameDefinitionManager@@@Z |
| 01a5c4b0 | ?GetDefinitionManager@CPlayerManager@@ABEABVCGameDefinitionManager@@XZ |
| 01aa0c60 | ?PeekGlobalDef@CGameDefinitionManager@@QBEABVCGlobalDef@@XZ |
| 01aa0c80 | ?GetPGlobalDef@CGameDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCGlobalDef@@@@XZ |
| 01ad0780 | ?PeekPDefinitionManager@CPlayer@@ABEPBVCGameDefinitionManager@@XZ |
| 01c8e870 | ?PeekDefinitionManager@CWorldMap@@IBEABVCGameDefinitionManager@@XZ |
| 01cf3be0 | ?PeekDefinitionManager@CThingManager@@QBEABVCGameDefinitionManager@@XZ |
| 01cf60c0 | ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ |
| 01d30600 | ??0CThingManager@@QAE@ABVCMainGameComponent@@ABVCGameDefinitionManager@@AAVCWorld@@AAVCPlayerManager@@AAVCNavigatorManager@@PAKKKK@Z |
| 01d87170 | ?PeekHeroStatsDef@CGameDefinitionManager@@QBEABVCHeroStatsDef@@XZ |
| 01db63d0 | ?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ |
| 01e64af0 | ?GetPHeroStatsDef@CGameDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCHeroStatsDef@@@@XZ |
| 02029230 | ??0CEditControlCentre@@QAE@AAVCEditComponent@@AAVCEditWorld@@ABVCGameDefinitionManager@@AAVCEditDisplayEngine@@AAVCPlayerManager@@@Z |
| 02030f20 | ?PeekDefinitionManager@CEditControlCentre@@QBEABVCGameDefinitionManager@@XZ |
| 02185be0 | ?PeekDefinitionManager@CRegionMinimap@@QBEABVCGameDefinitionManager@@XZ |
| 0218cac0 | ?PeekDefinitionManager@CMinimapData@@QBEABVCGameDefinitionManager@@XZ |
| 02190d80 | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ |
| ... | 21 more omitted |

## Chests, silver keys, inventory rewards

Matches: 3351

| Address | Symbol |
|---|---|
| 018c3ae0 | ?EnableInventoryTest@NGlobalConsole@@YIXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z |
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
| 0191f169 | ?Alloc@CPlayerInventoryDef@@SIPAVCDefClassBase@@XZ |
| 0191f1dd | ??0CPlayerInventoryDef@@QAE@XZ |
| 0191f29c | ?Copy@CPlayerInventoryDef@@UAEXPBVCDefClassBase@@@Z |
| 0191f2b7 | ?GetSizeofClass@CPlayerInventoryDef@@UAEKXZ |
| 0191f2c8 | ?Transfer@CPlayerInventoryDef@@UAEXAAVCPersistContext@@@Z |
| 0191f357 | ??4CPlayerInventoryDef@@QAEAAV0@ABV0@@Z |
| 0191f3ed | ??_ECPlayerInventoryDef@@UAEPAXI@Z |
| 0191f41d | ??1CPlayerInventoryDef@@UAE@XZ |
| 0192a2d1 | ?Alloc@CInventoryDef@@SIPAVCDefClassBase@@XZ |
| 0192a345 | ??0CInventoryDef@@QAE@XZ |
| 0192b09e | ?Copy@CInventoryDef@@UAEXPBVCDefClassBase@@@Z |
| 0192b0b9 | ?GetSizeofClass@CInventoryDef@@UAEKXZ |
| 0192b0ca | ?Transfer@CInventoryDef@@UAEXAAVCPersistContext@@@Z |
| 0192da93 | ??4CInventoryDef@@QAEAAV0@ABV0@@Z |
| 019305af | ??_GCInventoryDef@@UAEPAXI@Z |
| 019305df | ??1CInventoryDef@@UAE@XZ |
| 019306fd | ?Alloc@CInventoryCategoryDef@@SIPAVCDefClassBase@@XZ |
| 0193076d | ??0CInventoryCategoryDef@@QAE@XZ |
| 01930791 | ?Copy@CInventoryCategoryDef@@UAEXPBVCDefClassBase@@@Z |
| 019307ac | ?GetSizeofClass@CInventoryCategoryDef@@UAEKXZ |
| 019307bb | ?Transfer@CInventoryCategoryDef@@UAEXAAVCPersistContext@@@Z |
| ... | 3311 more omitted |

## Local tool overlap

- FQT calls SilverChest.WadBridge.exe via FableQuestTool/Services/WadBridgeClient.cs to
  list/extract FinalAlbion.wad content and feed TNG scanning.
- FQT currently notes that vanilla TNG data is stored in LEV/WAD paths and that extraction
  through LEV structures is incomplete. FableWin editor symbols identify native functions
  around CEditWorldMap, CEditLevelMerger::LoadLevFile, and ParseTngFile that can guide
  a better extractor/writer.
- ChocolateBox maps to the same domain at the tool level: region editing, lev/tng edits,
  FinalAlbion WAD/loose level workflows, and definition metadata. The PDB names give us the
  engine/editor side of those workflows.
