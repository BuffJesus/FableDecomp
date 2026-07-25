# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 472
- Unique retail target addresses: 472
- Needs reversal: 59
- Agent reviewed: 219
- Implementation verified: 192
- Hook approved: 0
- Direct wrapper/retail signatures: 169
- Wrapper adapters required: 277
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.TakeObjectFromHero` | `CGameScriptInterface::TakeObjectFromHero` | `0x00898DB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 2 | `Quest.TellHeroQuestObjectiveCompleted` | `CGameScriptInterface::TellHeroQuestObjectiveCompleted` | `0x00891800` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 3 | `Quest.TellHeroQuestObjectiveFailed` | `CGameScriptInterface::TellHeroQuestObjectiveFailed` | `0x00891810` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 4 | `Quest.TransitionToThemeAllInternals` | `CGameScriptInterface::TransitionToThemeAllInternals` | `0x0088FF30` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 5 | `Quest.UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | `CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | `0x00897150` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 6 | `Quest.UpdateQuestInfoBar` | `CGameScriptInterface::UpdateQuestInfoBar` | `0x00891B80` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 7 | `Quest.UpdateQuestInfoCounter` | `CGameScriptInterface::UpdateQuestInfoCounter` | `0x00891BD0` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 8 | `Quest.UpdateQuestInfoCounterList` | `CGameScriptInterface::UpdateQuestInfoCounterList` | `0x00891BE0` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 9 | `Quest.UpdateQuestInfoTick` | `CGameScriptInterface::UpdateQuestInfoTick` | `0x00891BF0` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 10 | `Quest.UpdateQuestInfoTimer` | `CGameScriptInterface::UpdateQuestInfoTimer` | `0x00891BB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 11 | `Quest.WaitForCameraMessage` | `CGameScriptInterface::WaitForCameraMessage` | `0x006E7820` | wrapper-adapter | unique | strong | yes | needs-reversal | 220 |
| 12 | `Quest.AddBoast` | `CGameScriptInterface::AddBoast` | `0x00893060` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 13 | `Quest.AddGossipFactionToCategory` | `CGameScriptInterface::AddGossipFactionToCategory` | `0x008A9E60` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 14 | `Quest.AddQuestRegion` | `CGameScriptInterface::AddQuestRegion` | `0x00891820` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 15 | `Quest.AddScreenMessage` | `CGameScriptInterface::AddScreenMessage` | `0x00892850` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 16 | `Quest.CameraUseCameraPoint` | `CGameScriptInterface::CameraUseCameraPoint` | `0x00891070` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 17 | `Quest.ClearGossip` | `CGameScriptInterface::ClearGossip` | `0x008AA010` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 18 | `Quest.DeactivateQuest` | `CGameScriptInterface::DeactivateQuest` | `0x00892F00` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 19 | `Quest.DisplayTutorial` | `CGameScriptInterface::DisplayTutorial` | `0x0089E710` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 20 | `Quest.EnableGuards` | `CGameScriptInterface::EnableGuards` | `0x00896270` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 21 | `Quest.EnableVillagerDefTypes` | `CGameScriptInterface::EnableVillagerDefTypes` | `0x008962D0` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 22 | `Quest.GetNumberOfItemsOfTypeInInventory` | `CGameScriptInterface::GetNumberOfItemsOfTypeInInventory` | `0x00897190` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 23 | `Quest.GetWaterHeightAtPosition` | `CGameScriptInterface::GetWaterHeightAtPosition` | `0x0088DF60` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 24 | `Quest.GiveHeroExpression` | `CGameScriptInterface::GiveHeroExpression` | `0x0088FC60` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 25 | `Quest.GiveHeroGold` | `CGameScriptInterface::GiveHeroGold` | `0x00898F40` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 26 | `Quest.IsBoastTaken` | `CGameScriptInterface::IsBoastTaken` | `0x00891550` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 27 | `Quest.IsQuestCompleted` | `CGameScriptInterface::IsQuestCompleted` | `0x00892F60` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 28 | `Quest.IsQuestRegistered` | `CGameScriptInterface::IsQuestRegistered` | `0x00892F50` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 29 | `Quest.RadialBlurSetCenterWorldPos` | `CGameScriptInterface::RadialBlurSetCenterWorldPos` | `0x008901A0` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 30 | `Quest.RemoveBoast` | `CGameScriptInterface::RemoveBoast` | `0x00891580` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 31 | `Quest.SetBoastAsCompleted` | `CGameScriptInterface::SetBoastAsCompleted` | `0x008915E0` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 32 | `Quest.SetQuestAsPersistent` | `CGameScriptInterface::SetQuestAsPersistent` | `0x00893020` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 33 | `Quest.SetQuestCardObjective` | `CGameScriptInterface::SetQuestCardObjective` | `0x00896A30` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 34 | `Quest.SetQuestInfoName` | `CGameScriptInterface::SetQuestInfoName` | `0x00891940` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 35 | `Quest.SetSaveGameMarkerPos` | `CGameScriptInterface::SetSaveGameMarkerPos` | `0x0088F960` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 36 | `Quest.SetVillageLimbo` | `CGameScriptInterface::SetVillageLimbo` | `0x00896060` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 37 | `Quest.TryToRespawnDefNamed` | `CGameScriptInterface::TryToRespawnDefNamed` | `0x00896120` | wrapper-adapter | ambiguous | strong | yes | needs-reversal | 220 |
| 38 | `Quest.CreateCreature` | `CGameScriptInterface::CreateCreature` | `0x008A9100` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 39 | `Quest.CreateCreatureNearby` | `CGameScriptInterface::CreateCreatureNearby` | `0x0089F300` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 40 | `Quest.DebugGetAllTextEntriesForTargetedThing` | `CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing` | `0x008A0CB0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 41 | `Quest.EntityGetAppearanceSeed` | `CGameScriptInterface::EntityGetAppearanceSeed` | `0x008A0C10` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 42 | `Quest.EntityGetShotStrikePos` | `CGameScriptInterface::EntityGetShotStrikePos` | `0x0089DF70` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 43 | `Quest.FadeScreenOut` | `CGameScriptInterface::FadeScreenOut` | `0x00890820` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 44 | `Quest.GetAllCreaturesExcludingHero` | `CGameScriptInterface::GetAllCreaturesExcludingHero` | `0x008A89D0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 45 | `Quest.GetAllCreaturesInAreaWithScriptName` | `CGameScriptInterface::GetAllCreaturesInAreaWithScriptName` | `0x008A86C0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 46 | `Quest.GetAllThingsWithDefNameByDistanceFrom` | `CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom` | `0x008A9C40` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 47 | `Quest.GetItemDefNamesFromContainer` | `CGameScriptInterface::GetItemDefNamesFromContainer` | `0x008A8F20` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 48 | `Quest.GetSleepingPositionAndOrientationFromBed` | `CGameScriptInterface::GetSleepingPositionAndOrientationFromBed` | `0x008A1B10` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 49 | `Quest.IsQuestStartScreenActive` | `CGameScriptInterface::IsQuestStartScreenActive` | `0x0089AD90` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 50 | `Quest.MsgOnBoastsMade` | `CGameScriptInterface::MsgOnBoastsMade` | `0x008A9AE0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 51 | `Quest.MsgOnExpressionPerformed` | `CGameScriptInterface::MsgOnExpressionPerformed` | `0x00893B00` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 52 | `Quest.MsgOnHeroSlept` | `CGameScriptInterface::MsgOnHeroSlept` | `0x00893EC0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 53 | `Quest.StartMovieSequence` | `CGameScriptInterface::StartMovieSequence` | `0x0089B110` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 54 | `Entity.MsgExpressionPerformedTo` | `CScriptThing::MsgExpressionPerformedTo` | `0x004AABF0` | signature-review | unique | strong | yes | needs-reversal | 100 |
| 55 | `Entity.MsgIsPresentedWithItem` | `CScriptThing::MsgIsPresentedWithItem` | `0x004AAC60` | signature-review | unique | strong | yes | needs-reversal | 100 |
| 56 | `Entity.MsgReceivedMoney` | `CScriptThing::MsgReceivedMoney` | `0x004AAC40` | signature-review | unique | strong | yes | needs-reversal | 100 |
| 57 | `Quest.GetAllActiveQuestInfo` | `CGameScriptInterface::GetAllActiveQuestInfo` | `0x00891530` | signature-review | ambiguous | strong | yes | needs-reversal | 220 |
| 58 | `Quest.SetQuestWorldMapOffset` | `CGameScriptInterface::SetQuestWorldMapOffset` | `0x00891860` | signature-review | ambiguous | strong | yes | needs-reversal | 220 |
| 59 | `Quest.SetTimeAsStopped` | `CGameScriptInterface::SetTimeAsStopped` | `0x0088FE80` | signature-review | ambiguous | strong | yes | needs-reversal | 220 |
| 60 | `Quest.ApplyHeroPenaltyForDeath` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | `0x008977F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 61 | `Quest.CameraDefault` | `CGameScriptInterface::CameraDefault` | `0x00890FF0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 62 | `Quest.CameraShake` | `CGameScriptInterface::CameraShake` | `0x0088ECE0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 63 | `Quest.CancelHeroTeleportEffects` | `CGameScriptInterface::CancelHeroTeleportEffects` | `0x008A0820` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 64 | `Quest.ChangeHeroHealthBy` | `CGameScriptInterface::ChangeHeroHealthBy` | `0x00891720` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 65 | `Quest.ChangeHeroMoralityDueToPicklock` | `CGameScriptInterface::ChangeHeroMoralityDueToPicklock` | `0x0089A0E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 66 | `Quest.ChangeHeroMoralityDueToTheft` | `CGameScriptInterface::ChangeHeroMoralityDueToTheft` | `0x0089A070` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 67 | `Quest.ConfiscateAllHeroItems` | `CGameScriptInterface::ConfiscateAllHeroItems` | `0x008986D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 68 | `Quest.FadeScreenOutUntilNextCallToFadeScreenIn` | `CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `0x008907E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 69 | `Quest.GetBestTimeGuessTheAddition` | `CGameScriptInterface::GetBestTimeGuessTheAddition` | `0x008A17D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 70 | `Quest.GetBestTimePairs` | `CGameScriptInterface::GetBestTimePairs` | `0x008A1590` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 71 | `Quest.GetBestTimeSorting` | `CGameScriptInterface::GetBestTimeSorting` | `0x008A15F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 72 | `Quest.GetDistanceHeroCanBeHeardFrom` | `CGameScriptInterface::GetDistanceHeroCanBeHeardFrom` | `0x00899700` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 73 | `Quest.GetHeroAge` | `CGameScriptInterface::GetHeroAge` | `0x00899560` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 74 | `Quest.GetHeroAttractiveness` | `CGameScriptInterface::GetHeroAttractiveness` | `0x00899930` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 75 | `Quest.GetHeroFatness` | `CGameScriptInterface::GetHeroFatness` | `0x00899850` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 76 | `Quest.GetHeroHasChildren` | `CGameScriptInterface::GetHeroHasChildren` | `0x00897BD0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 77 | `Quest.GetHeroHasCurrentMarriage` | `CGameScriptInterface::GetHeroHasCurrentMarriage` | `0x00897B10` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 78 | `Quest.GetHeroHasDivorcedMarriage` | `CGameScriptInterface::GetHeroHasDivorcedMarriage` | `0x00897B70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 79 | `Quest.GetHeroHasMarried` | `CGameScriptInterface::GetHeroHasMarried` | `0x00897AA0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 80 | `Quest.GetHeroHasMurderedWife` | `CGameScriptInterface::GetHeroHasMurderedWife` | `0x00897C30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 81 | `Quest.GetHeroMorality` | `CGameScriptInterface::GetHeroMorality` | `0x00899F90` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 82 | `Quest.GetHeroScariness` | `CGameScriptInterface::GetHeroScariness` | `0x008998C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 83 | `Quest.GetHeroWillEnergyLevel` | `CGameScriptInterface::GetHeroWillEnergyLevel` | `0x008999A0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 84 | `Quest.GiveHeroMorality` | `CGameScriptInterface::GiveHeroMorality` | `0x00899EF0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 85 | `Quest.IncreaseHeroRenownLevel` | `CGameScriptInterface::IncreaseHeroRenownLevel` | `0x00899220` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 86 | `Quest.IsHeroAllowedHenchmenInCurrentRegion` | `CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion` | `0x00891EA0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 87 | `Quest.IsHeroHandLampLit` | `CGameScriptInterface::IsHeroHandLampLit` | `0x00898E60` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 88 | `Quest.IsHeroRenownLevelFull` | `CGameScriptInterface::IsHeroRenownLevelFull` | `0x008991A0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 89 | `Quest.IsPlayerCreatureFlourishEnabled` | `CGameScriptInterface::IsPlayerCreatureFlourishEnabled` | `0x00899DB0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 90 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 91 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 92 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 93 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 94 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 95 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 96 | `Quest.RemoveHeroHairstyle` | `CGameScriptInterface::RemoveHeroHairstyle` | `0x0089B4D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 97 | `Quest.ResetPlayerCreatureCombatMultiplier` | `CGameScriptInterface::ResetPlayerCreatureCombatMultiplier` | `0x00899D50` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 98 | `Quest.ResetPlayerCreatureOnlyTarget` | `CGameScriptInterface::ResetPlayerCreatureOnlyTarget` | `0x00899E90` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 99 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 100 | `Quest.ScreenFilterFadeOut` | `CGameScriptInterface::ScreenFilterFadeOut` | `0x00890200` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
