# Curated C++ promotion queue

Generated: `2026-08-28T20:12:49-06:00`

Uncompiled auto-RE candidates after bounded exclusions: **384**. Showing: **100**. Already-bounded parity residues excluded: **84**. Semantic-review quarantine: **56**.

Ranking favors checker/integrity/signature PASS and candidates without known source-level hazards, then the smallest declaration, dependency, VC7.1, and source-size repair surface. Structural fidelity does not by itself make an unsafe C++ expression promotable; every promotion still needs semantic review, a focused behavior oracle, and retail comparison.

| Rank | Address | Owner/function | Lane | Signature | Hazards | Missing deps | VC7.1 fixes | Source bytes | First blocker |
|---:|---|---|---|---|---|---:|---:|---:|---|
| 1 | `0x00890750` | `CGameScriptInterface::AddLineToConversation` | `manual-lift` | `PASS` |  | 9 | 11 | 1982 | 'CScriptConversationManager' does not name a type |
| 2 | `0x00A781A0` | `CNavQuadTree::UpdateLines` | `manual-lift` | `PASS` |  | 12 | 75 | 16017 | 'C2DVector' does not name a type |
| 3 | `0x00A7A8D0` | `CNavQuadTree::Initialise` | `manual-lift` | `PASS` |  | 13 | 142 | 26841 | 'C2DLineF' was not declared in this scope |
| 4 | `0x00A79B30` | `CNavQuadTreeNode::Initialise` | `manual-lift` | `PASS` |  | 18 | 78 | 15716 | 'CANavQuadTreeNode' does not name a type |
| 5 | `0x0050A650` | `CWorldMap::GetMapNavigationAreaInit` | `manual-lift` | `PASS` |  | 26 | 93 | 25507 | use of enum 'EHeroMorphType' without previous declaration |
| 6 | `0x00897030` | `CGameScriptInterface::IsToFailQuestOnDeath` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 3608 | 'reinterpret_cast' from integer to pointer |
| 7 | `0x00897480` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 26 | 3816 | 'reinterpret_cast' from integer to pointer |
| 8 | `0x00898DB0` | `CGameScriptInterface::TakeObjectFromHero` | `semantic-review` | `PASS` | possible-end-sentinel-dereference;process-termination | 0 | 28 | 4845 |  |
| 9 | `0x00898840` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 28 | 6295 | 'reinterpret_cast' from integer to pointer |
| 10 | `0x00898E60` | `CGameScriptInterface::IsHeroHandLampLit` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 29 | 3806 | 'reinterpret_cast' from integer to pointer |
| 11 | `0x0089CD70` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 30 | 4433 |  |
| 12 | `0x006E7920` | `CGameScriptInterface::CheckForCameraMessage` | `semantic-review` | `PASS` | explicit-review-marker;possible-end-sentinel-dereference | 0 | 31 | 6201 |  |
| 13 | `0x008986D0` | `CGameScriptInterface::ConfiscateAllHeroItems` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 31 | 6335 |  |
| 14 | `0x008A0820` | `CGameScriptInterface::CancelHeroTeleportEffects` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 35 | 5392 |  |
| 15 | `0x008A03F0` | `CGameScriptInterface::ClearThingBestEnemyTarget` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 37 | 4871 |  |
| 16 | `0x00898940` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 37 | 6734 | 'reinterpret_cast' from integer to pointer |
| 17 | `0x00898600` | `CGameScriptInterface::RemoveAllHeroWeapons` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 38 | 5077 | 'reinterpret_cast' from integer to pointer |
| 18 | `0x00891A00` | `CGameScriptInterface::SetQuestInfoText` | `semantic-review` | `PASS` | explicit-review-marker | 0 | 39 | 5388 | static assertion failed |
| 19 | `0x00895190` | `CGameScriptInterface::GiveThingItemInSlot` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 40 | 8843 |  |
| 20 | `0x008981C0` | `CGameScriptInterface::AddTattooToHero` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 48 | 9170 | static assertion failed |
| 21 | `0x006E7820` | `CGameScriptInterface::WaitForCameraMessage` | `semantic-review` | `PASS` | address-reinterpret-call;possible-end-sentinel-dereference | 0 | 52 | 7728 |  |
| 22 | `0x00898410` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 54 | 7742 | 'reinterpret_cast' from integer to pointer |
| 23 | `0x00898340` | `CGameScriptInterface::SetHeroWillAsUsable` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 57 | 7792 | 'reinterpret_cast' from integer to pointer |
| 24 | `0x00892850` | `CGameScriptInterface::AddScreenMessage` | `semantic-review` | `PASS` | explicit-review-marker | 0 | 59 | 8341 |  |
| 25 | `0x0089FEB0` | `CGameScriptInterface::AddCrimeCommitted` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 0 | 69 | 12255 | 'reinterpret_cast' from integer to pointer |
| 26 | `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | `semantic-review` | `PASS` | explicit-review-marker | 1 | 0 | 673 | 'CRGBColour' does not name a type |
| 27 | `0x008977F0` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 30 | 4820 | invalid use of incomplete type 'class CGameScriptInterface' |
| 28 | `0x008973A0` | `CGameScriptInterface::IsPlayerCarryingItemOfType` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 32 | 4555 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 29 | `0x0089C560` | `CGameScriptInterface::EntityPostOpinionDeedToAll` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 35 | 5377 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 30 | `0x008954A0` | `CGameScriptInterface::EntityFollowThing` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 38 | 6704 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 31 | `0x008A7980` | `CGameScriptInterface::EntityStopFollowing` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 45 | 5978 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 32 | `0x008A8C60` | `CGameScriptInterface::EntitySetAsSittingOnFloor` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 48 | 9898 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 33 | `0x008A0CB0` | `CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 51 | 8729 | invalid use of incomplete type 'class CGameScriptInterface' |
| 34 | `0x008A23B0` | `CGameScriptInterface::SetWanderCentrePoint` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 53 | 6956 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 35 | `0x008A0260` | `CGameScriptInterface::GiveThingBestEnemyTarget` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 56 | 8469 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 36 | `0x008A93A0` | `CGameScriptInterface::EntityWillTeleportToArea` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 1 | 68 | 10847 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 37 | `0x008A99F0` | `CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 2 | 37 | 6584 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 38 | `0x00891CA0` | `CGameScriptInterface::GetHero` | `semantic-review` | `PASS` | explicit-review-marker | 2 | 37 | 5612 | 'CScriptThing* NScript::operator&&(GetHeroPredicate5*, CIsThingAlive*)' must have an argument of class or enumerated type |
| 39 | `0x00896B60` | `CGameScriptInterface::KickOffQuestStartScreen` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 2 | 53 | 8379 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 40 | `0x00896390` | `CGameScriptInterface::OpenChest` | `semantic-review` | `PASS` | explicit-review-marker | 2 | 77 | 15099 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 41 | `0x00898F40` | `CGameScriptInterface::GiveHeroGold` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 3 | 17 | 2160 | 'ETCInterfaceType' was not declared in this scope |
| 42 | `0x00896A30` | `CGameScriptInterface::SetQuestCardObjective` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 3 | 29 | 3566 | 'ETCInterfaceType' was not declared in this scope |
| 43 | `0x009F1AC0` | `CGameEventPackageSet::InitFromCompressedBuffer` | `semantic-review` | `PASS` | address-reinterpret-call | 3 | 41 | 3193 | 'CGameEventPackageSet' has not been declared; did you mean 'CGameEventPackageOverlay'? |
| 44 | `0x00401067` | `entry` | `semantic-review` | `PASS` | explicit-review-marker | 3 | 50 | 6649 | static assertion failed |
| 45 | `0x008A8F20` | `CGameScriptInterface::GetItemDefNamesFromContainer` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 4 | 22 | 3561 | 'CDefinitionManager' does not name a type |
| 46 | `0x00670710` | `CTCCoopSpirit::UpdateScore` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 10 | 25 | 4000 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 47 | `0x0062C0E0` | `CWorld::EAMoveSpirit` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 10 | 33 | 3624 | 'CWorld' has not been declared |
| 48 | `0x0089B110` | `CGameScriptInterface::StartMovieSequence` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 10 | 45 | 4802 | static assertion failed |
| 49 | `0x00891940` | `CGameScriptInterface::SetQuestInfoName` | `semantic-review` | `PASS` | explicit-review-marker | 11 | 25 | 3022 | 'CGameScriptInterface' has not been declared |
| 50 | `0x008A1B10` | `CGameScriptInterface::GetSleepingPositionAndOrientationFromBed` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 15 | 21 | 3084 | 'CGameScriptInterface' has not been declared |
| 51 | `0x006701A0` | `CTCCoopSpirit::UpdateAttractionToMaster` | `semantic-review` | `PASS` | possible-end-sentinel-dereference | 16 | 32 | 4683 | macro 'offsetof' passed 3 arguments, but takes just 2 |
| 52 | `0x00462F1F` | `Copy_MemoryRange` | `compile-now` | `REVIEW` |  | 0 | 0 | 573 |  |
| 53 | `0x0046780E` | `Vector_CopyRange` | `compile-now` | `REVIEW` |  | 0 | 0 | 585 |  |
| 54 | `0x0045F008` | `CActiveFile_AllocateAndCopyVector` | `vc71-port` | `REVIEW` |  | 0 | 2 | 533 |  |
| 55 | `0x00C92F10` | `DSP_ComputeRunningMinEnvelope` | `vc71-port` | `REVIEW` |  | 0 | 2 | 2187 |  |
| 56 | `0x0045EFE5` | `CVector_CopyRange` | `vc71-port` | `REVIEW` |  | 0 | 3 | 533 |  |
| 57 | `0x00464AAB` | `Vector_CopyRangeForward` | `vc71-port` | `REVIEW` |  | 0 | 3 | 539 |  |
| 58 | `0x0042C1C1` | `CMemoryAllocatorVariableSize_FindInsertPointLeft` | `vc71-port` | `REVIEW` |  | 0 | 4 | 5661 |  |
| 59 | `0x0046496F` | `std_vector_push_copy_element` | `vc71-port` | `REVIEW` |  | 0 | 5 | 756 |  |
| 60 | `0x00462DC1` | `Vector_AllocateAndCopy_12` | `vc71-port` | `REVIEW` |  | 0 | 6 | 869 |  |
| 61 | `0x004698BE` | `Vector_Insert` | `vc71-port` | `REVIEW` |  | 0 | 6 | 3276 |  |
| 62 | `0x00466E1E` | `Vector_Insert` | `vc71-port` | `REVIEW` |  | 0 | 6 | 4066 |  |
| 63 | `0x004612D6` | `Vector_AllocateAndCopy_8` | `vc71-port` | `REVIEW` |  | 0 | 7 | 970 |  |
| 64 | `0x00468FE3` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 8 | 907 |  |
| 65 | `0x004640D1` | `Vector_RShift_4` | `vc71-port` | `REVIEW` |  | 0 | 8 | 933 |  |
| 66 | `0x00411B90` | `std_vector_assign` | `vc71-port` | `REVIEW` |  | 0 | 8 | 2436 |  |
| 67 | `0x0046594C` | `Vector_InsertElements_Realloc` | `vc71-port` | `REVIEW` |  | 0 | 8 | 3570 |  |
| 68 | `0x00463901` | `CEngineSceneGrid_AllocBoundingBoxNode` | `vc71-port` | `REVIEW` |  | 0 | 9 | 747 |  |
| 69 | `0x00450CCC` | `Buffer_InitializeWithSize` | `vc71-port` | `REVIEW` |  | 0 | 9 | 827 |  |
| 70 | `0x0046085B` | `CActiveFile_AllocateAndCopyValue` | `vc71-port` | `REVIEW` |  | 0 | 9 | 856 |  |
| 71 | `0x00451583` | `VertexListInsertVertices` | `vc71-port` | `REVIEW` |  | 0 | 9 | 871 |  |
| 72 | `0x00460D22` | `Vector_AllocateAndCopy_8` | `vc71-port` | `REVIEW` |  | 0 | 9 | 907 |  |
| 73 | `0x0046988F` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 9 | 986 |  |
| 74 | `0x0046BBA2` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1005 |  |
| 75 | `0x00466DE5` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1078 |  |
| 76 | `0x00460BEC` | `CActiveFile_AssignVector20` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1197 |  |
| 77 | `0x0046656F` | `CDialogueLayerDef_AllocateAndConstruct` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1362 |  |
| 78 | `0x00C37000` | `IntelligentPointer_BuildAndDispatchFloat` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1429 |  |
| 79 | `0x0045DF1E` | `Save_WriteMemoryAllocatorAreas` | `vc71-port` | `REVIEW` |  | 0 | 9 | 1728 |  |
| 80 | `0x00465DBC` | `RBTree_InsertNode` | `vc71-port` | `REVIEW` |  | 0 | 9 | 2345 |  |
| 81 | `0x004669BF` | `RBTree_InsertNode` | `vc71-port` | `REVIEW` |  | 0 | 9 | 2498 |  |
| 82 | `0x0042AEB5` | `BinarySearchTree_FindByUintComparator` | `vc71-port` | `REVIEW` |  | 0 | 10 | 1300 |  |
| 83 | `0x00C36E00` | `CSBAgents_BuildAndDispatch_SummonerLightning` | `vc71-port` | `REVIEW` |  | 0 | 10 | 1440 |  |
| 84 | `0x0042B4F3` | `BinarySearchTree_FindLessOrEqual_Copy` | `vc71-port` | `REVIEW` |  | 0 | 10 | 1513 |  |
| 85 | `0x0046B6C1` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 10 | 1713 |  |
| 86 | `0x00409090` | `Engine_FindPrimitiveInArray` | `vc71-port` | `REVIEW` |  | 0 | 10 | 2100 |  |
| 87 | `0x0045EE0E` | `RBTree_InsertNewNode4` | `vc71-port` | `REVIEW` |  | 0 | 10 | 2256 |  |
| 88 | `0x00464733` | `RBTree_LowerBound` | `vc71-port` | `REVIEW` |  | 0 | 10 | 2597 |  |
| 89 | `0x0045DBF2` | `RBTree_InsertWithFind` | `vc71-port` | `REVIEW` |  | 0 | 10 | 2903 |  |
| 90 | `0x0045CD9B` | `CActiveFile_AssignVector24` | `vc71-port` | `REVIEW` |  | 0 | 11 | 978 |  |
| 91 | `0x0046B182` | `Vector_Erase` | `vc71-port` | `REVIEW` |  | 0 | 11 | 1076 |  |
| 92 | `0x00460FC6` | `Vector_AllocateAndCopy_12` | `vc71-port` | `REVIEW` |  | 0 | 11 | 1138 |  |
| 93 | `0x00410ED0` | `Std_Deque_PopFront_ChunkBased` | `vc71-port` | `REVIEW` |  | 0 | 11 | 1169 |  |
| 94 | `0x00461136` | `Vector_AllocateAndCopy_84` | `vc71-port` | `REVIEW` |  | 0 | 11 | 1295 |  |
| 95 | `0x0042AD91` | `BinarySearchTree_FindExact` | `vc71-port` | `REVIEW` |  | 0 | 11 | 1440 |  |
| 96 | `0x004638C8` | `CEngineSceneGrid_InsertBoundingBoxNode` | `vc71-port` | `REVIEW` |  | 0 | 11 | 2071 |  |
| 97 | `0x0045DC78` | `RBTree_InsertNewNode` | `vc71-port` | `REVIEW` |  | 0 | 11 | 2121 |  |
| 98 | `0x00463258` | `Vector_AllocateAndCopy_8` | `vc71-port` | `REVIEW` |  | 0 | 12 | 1058 |  |
| 99 | `0x00449320` | `CList_PushBack` | `vc71-port` | `REVIEW` |  | 0 | 12 | 1339 |  |
| 100 | `0x0086FA00` | `CPersistInfo_InsertionSortStep_0086fa00` | `vc71-port` | `REVIEW` |  | 0 | 12 | 1701 |  |

## Semantic-review quarantine

These candidates may structurally match the retail path, but their generated C++ contains a known source-level hazard and is intentionally ranked behind ordinary manual lifts.

| Address | Owner/function | Hazards |
|---|---|---|
| `0x00897030` | `CGameScriptInterface::IsToFailQuestOnDeath` | possible-end-sentinel-dereference |
| `0x00897480` | `CGameScriptInterface::IsPlayerWieldingWeapon` | possible-end-sentinel-dereference |
| `0x00898DB0` | `CGameScriptInterface::TakeObjectFromHero` | possible-end-sentinel-dereference;process-termination |
| `0x00898840` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | possible-end-sentinel-dereference |
| `0x00898E60` | `CGameScriptInterface::IsHeroHandLampLit` | possible-end-sentinel-dereference |
| `0x0089CD70` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | possible-end-sentinel-dereference |
| `0x006E7920` | `CGameScriptInterface::CheckForCameraMessage` | explicit-review-marker;possible-end-sentinel-dereference |
| `0x008986D0` | `CGameScriptInterface::ConfiscateAllHeroItems` | possible-end-sentinel-dereference |
| `0x008A0820` | `CGameScriptInterface::CancelHeroTeleportEffects` | possible-end-sentinel-dereference |
| `0x008A03F0` | `CGameScriptInterface::ClearThingBestEnemyTarget` | possible-end-sentinel-dereference |
| `0x00898940` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | possible-end-sentinel-dereference |
| `0x00898600` | `CGameScriptInterface::RemoveAllHeroWeapons` | possible-end-sentinel-dereference |
| `0x00891A00` | `CGameScriptInterface::SetQuestInfoText` | explicit-review-marker |
| `0x00895190` | `CGameScriptInterface::GiveThingItemInSlot` | possible-end-sentinel-dereference |
| `0x008981C0` | `CGameScriptInterface::AddTattooToHero` | possible-end-sentinel-dereference |
| `0x006E7820` | `CGameScriptInterface::WaitForCameraMessage` | address-reinterpret-call;possible-end-sentinel-dereference |
| `0x00898410` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | possible-end-sentinel-dereference |
| `0x00898340` | `CGameScriptInterface::SetHeroWillAsUsable` | possible-end-sentinel-dereference |
| `0x00892850` | `CGameScriptInterface::AddScreenMessage` | explicit-review-marker |
| `0x0089FEB0` | `CGameScriptInterface::AddCrimeCommitted` | possible-end-sentinel-dereference |
| `0x00449B60` | `CPlayerManager::GetMultiplayerColour` | explicit-review-marker |
| `0x008977F0` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | possible-end-sentinel-dereference |
| `0x008973A0` | `CGameScriptInterface::IsPlayerCarryingItemOfType` | possible-end-sentinel-dereference |
| `0x0089C560` | `CGameScriptInterface::EntityPostOpinionDeedToAll` | possible-end-sentinel-dereference |
| `0x008954A0` | `CGameScriptInterface::EntityFollowThing` | possible-end-sentinel-dereference |
|  | _31 additional quarantined candidates omitted_ |  |
