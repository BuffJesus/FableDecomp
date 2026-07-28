# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 474
- Unique retail target addresses: 474
- Needs reversal: 3
- Agent reviewed: 275
- Implementation verified: 194
- Hook approved: 0
- Direct wrapper/retail signatures: 169
- Wrapper adapters required: 279
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.CreateCreature` | `CGameScriptInterface::CreateCreature` | `0x008A9100` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 2 | `Quest.GetAllCreaturesInAreaWithScriptName` | `CGameScriptInterface::GetAllCreaturesInAreaWithScriptName` | `0x008A86C0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 3 | `Quest.MsgOnBoastsMade` | `CGameScriptInterface::MsgOnBoastsMade` | `0x008A9AE0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 4 | `Quest.ApplyHeroPenaltyForDeath` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | `0x008977F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 5 | `Quest.CameraDefault` | `CGameScriptInterface::CameraDefault` | `0x00890FF0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 6 | `Quest.CameraShake` | `CGameScriptInterface::CameraShake` | `0x0088ECE0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 7 | `Quest.CancelHeroTeleportEffects` | `CGameScriptInterface::CancelHeroTeleportEffects` | `0x008A0820` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 8 | `Quest.ChangeHeroHealthBy` | `CGameScriptInterface::ChangeHeroHealthBy` | `0x00891720` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 9 | `Quest.ChangeHeroMoralityDueToPicklock` | `CGameScriptInterface::ChangeHeroMoralityDueToPicklock` | `0x0089A0E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 10 | `Quest.ChangeHeroMoralityDueToTheft` | `CGameScriptInterface::ChangeHeroMoralityDueToTheft` | `0x0089A070` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 11 | `Quest.ConfiscateAllHeroItems` | `CGameScriptInterface::ConfiscateAllHeroItems` | `0x008986D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 12 | `Quest.FadeScreenOutUntilNextCallToFadeScreenIn` | `CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `0x008907E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 13 | `Quest.GetBestTimeGuessTheAddition` | `CGameScriptInterface::GetBestTimeGuessTheAddition` | `0x008A17D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 14 | `Quest.GetBestTimePairs` | `CGameScriptInterface::GetBestTimePairs` | `0x008A1590` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 15 | `Quest.GetBestTimeSorting` | `CGameScriptInterface::GetBestTimeSorting` | `0x008A15F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 16 | `Quest.GetDistanceHeroCanBeHeardFrom` | `CGameScriptInterface::GetDistanceHeroCanBeHeardFrom` | `0x00899700` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 17 | `Quest.GetHeroAge` | `CGameScriptInterface::GetHeroAge` | `0x00899560` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 18 | `Quest.GetHeroAttractiveness` | `CGameScriptInterface::GetHeroAttractiveness` | `0x00899930` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 19 | `Quest.GetHeroFatness` | `CGameScriptInterface::GetHeroFatness` | `0x00899850` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 20 | `Quest.GetHeroHasChildren` | `CGameScriptInterface::GetHeroHasChildren` | `0x00897BD0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 21 | `Quest.GetHeroHasCurrentMarriage` | `CGameScriptInterface::GetHeroHasCurrentMarriage` | `0x00897B10` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 22 | `Quest.GetHeroHasDivorcedMarriage` | `CGameScriptInterface::GetHeroHasDivorcedMarriage` | `0x00897B70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 23 | `Quest.GetHeroHasMarried` | `CGameScriptInterface::GetHeroHasMarried` | `0x00897AA0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 24 | `Quest.GetHeroHasMurderedWife` | `CGameScriptInterface::GetHeroHasMurderedWife` | `0x00897C30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 25 | `Quest.GetHeroMorality` | `CGameScriptInterface::GetHeroMorality` | `0x00899F90` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 26 | `Quest.GetHeroScariness` | `CGameScriptInterface::GetHeroScariness` | `0x008998C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 27 | `Quest.GetHeroWillEnergyLevel` | `CGameScriptInterface::GetHeroWillEnergyLevel` | `0x008999A0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 28 | `Quest.GiveHeroMorality` | `CGameScriptInterface::GiveHeroMorality` | `0x00899EF0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 29 | `Quest.IncreaseHeroRenownLevel` | `CGameScriptInterface::IncreaseHeroRenownLevel` | `0x00899220` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 30 | `Quest.IsHeroAllowedHenchmenInCurrentRegion` | `CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion` | `0x00891EA0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 31 | `Quest.IsHeroHandLampLit` | `CGameScriptInterface::IsHeroHandLampLit` | `0x00898E60` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 32 | `Quest.IsHeroRenownLevelFull` | `CGameScriptInterface::IsHeroRenownLevelFull` | `0x008991A0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 33 | `Quest.IsPlayerCreatureFlourishEnabled` | `CGameScriptInterface::IsPlayerCreatureFlourishEnabled` | `0x00899DB0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 34 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 35 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 36 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 37 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 38 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 39 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 40 | `Quest.RemoveHeroHairstyle` | `CGameScriptInterface::RemoveHeroHairstyle` | `0x0089B4D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 41 | `Quest.ResetPlayerCreatureCombatMultiplier` | `CGameScriptInterface::ResetPlayerCreatureCombatMultiplier` | `0x00899D50` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 42 | `Quest.ResetPlayerCreatureOnlyTarget` | `CGameScriptInterface::ResetPlayerCreatureOnlyTarget` | `0x00899E90` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 43 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 44 | `Quest.ScreenFilterFadeOut` | `CGameScriptInterface::ScreenFilterFadeOut` | `0x00890200` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 45 | `Quest.SetGuardsIgnoreCrimes` | `CGameScriptInterface::SetGuardsIgnoreCrimes` | `0x00898540` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 46 | `Quest.SetHeroAbleToGainExperience` | `CGameScriptInterface::SetHeroAbleToGainExperience` | `0x00899060` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 47 | `Quest.SetHeroAge` | `CGameScriptInterface::SetHeroAge` | `0x008994E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 48 | `Quest.SetHeroAsApprentice` | `CGameScriptInterface::SetHeroAsApprentice` | `0x00899680` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 49 | `Quest.SetHeroAsHavingHadGaySex` | `CGameScriptInterface::SetHeroAsHavingHadGaySex` | `0x00897DB0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 50 | `Quest.SetHeroAsHavingHadSex` | `CGameScriptInterface::SetHeroAsHavingHadSex` | `0x00897D50` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 51 | `Quest.SetHeroAsTeenager` | `CGameScriptInterface::SetHeroAsTeenager` | `0x008995D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 52 | `Quest.SetHeroHandLampAsLit` | `CGameScriptInterface::SetHeroHandLampAsLit` | `0x00898EC0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 53 | `Quest.SetHeroWeaponsAsUsable` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `0x00898410` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 54 | `Quest.SetHeroWillAsUsable` | `CGameScriptInterface::SetHeroWillAsUsable` | `0x00898340` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 55 | `Quest.SetHeroWillEnergyAsAbleToRefill` | `CGameScriptInterface::SetHeroWillEnergyAsAbleToRefill` | `0x00899A90` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 56 | `Quest.SetHeroWillEnergyLevel` | `CGameScriptInterface::SetHeroWillEnergyLevel` | `0x00899A10` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 57 | `Quest.SetMoralityChangingAsEnabled` | `CGameScriptInterface::SetMoralityChangingAsEnabled` | `0x0089EF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 58 | `Quest.SetTimeOfDay` | `CGameScriptInterface::SetTimeOfDay` | `0x0088FDC0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 59 | `Quest.SetWeaponOutCrimeEnabled` | `CGameScriptInterface::SetWeaponOutCrimeEnabled` | `0x008984E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 60 | `Quest.SetWhetherToFailQuestOnDeath` | `CGameScriptInterface::SetWhetherToFailQuestOnDeath` | `0x00897090` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 61 | `Quest.SheatheHeroWeapons` | `CGameScriptInterface::SheatheHeroWeapons` | `0x008916A0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 62 | `Quest.StartCountdownTimer` | `CGameScriptInterface::StartCountdownTimer` | `0x0088F8D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 63 | `Quest.AutoSaveCheckPoint` | `CGameScriptInterface::AutoSaveCheckPoint` | `0x0088F910` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 64 | `Quest.AutoSaveQuestStart` | `CGameScriptInterface::AutoSaveQuestStart` | `0x0088F920` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 65 | `Quest.DisplayQuestInfo` | `CGameScriptInterface::DisplayQuestInfo` | `0x00891920` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 66 | `Quest.DontPopulateNextLoadedRegion` | `CGameScriptInterface::DontPopulateNextLoadedRegion` | `0x0088E380` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 67 | `Quest.FastForwardTimeTo` | `CGameScriptInterface::FastForwardTimeTo` | `0x0088FE90` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 68 | `Quest.IsDangerMusicEnabled` | `CGameScriptInterface::IsDangerMusicEnabled` | `0x00891330` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 69 | `Quest.ResetToFrontEnd` | `CGameScriptInterface::ResetToFrontEnd` | `0x0088F970` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 70 | `Quest.SetEnvironmentalEffectsAlwaysUpdate` | `CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `0x00890B90` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 71 | `Quest.SetGuildMasterMessages` | `CGameScriptInterface::SetGuildMasterMessages` | `0x0088E200` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 72 | `Quest.AddCrimeCommitted` | `CGameScriptInterface::AddCrimeCommitted` | `0x0089FEB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 73 | `Quest.AddFeatCard` | `CGameScriptInterface::AddFeatCard` | `0x00891540` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 74 | `Quest.AddGossipVillage` | `CGameScriptInterface::AddGossipVillage` | `0x008A9E30` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 75 | `Quest.AddNewRumourToCategory` | `CGameScriptInterface::AddNewRumourToCategory` | `0x008A9DD0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 76 | `Quest.AddQuestCard` | `CGameScriptInterface::AddQuestCard` | `0x008913F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 77 | `Quest.AddQuestInfoBar` | `CGameScriptInterface::AddQuestInfoBar` | `0x00891AC0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 78 | `Quest.AddQuestInfoCounterList` | `CGameScriptInterface::AddQuestInfoCounterList` | `0x00891B50` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 79 | `Quest.AddQuestInfoTimer` | `CGameScriptInterface::AddQuestInfoTimer` | `0x00891B20` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 80 | `Quest.AddTattooToHero` | `CGameScriptInterface::AddTattooToHero` | `0x008981C0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 81 | `Quest.ApplyScriptBrush` | `CGameScriptInterface::ApplyScriptBrush` | `0x0088F480` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 82 | `Quest.AreEntitiesEnemies` | `CGameScriptInterface::AreEntitiesEnemies` | `0x0088E6F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 83 | `Quest.CacheMusicSet` | `CGameScriptInterface::CacheMusicSet` | `0x00891310` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 84 | `Quest.CameraCircleAroundPos` | `CGameScriptInterface::CameraCircleAroundPos` | `0x00892C20` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 85 | `Quest.CameraCircleAroundThing` | `CGameScriptInterface::CameraCircleAroundThing` | `0x00892B50` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 86 | `Quest.CameraMoveBetweenLookFromAndLookTo` | `CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo` | `0x00892610` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 87 | `Quest.CameraMoveToPosAndLookAtPos` | `CGameScriptInterface::CameraMoveToPosAndLookAtPos` | `0x00892410` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 88 | `Quest.CanThingBe_Heard_ByOtherThing` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | `0x0089CD70` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 89 | `Quest.CanThingBe_Smelled_ByOtherThing` | `CGameScriptInterface::CanThingBe_Smelled_ByOtherThing` | `0x0089CD50` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 90 | `Quest.ChangeHeroHairstyle` | `CGameScriptInterface::ChangeHeroHairstyle` | `0x0089B450` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 91 | `Quest.ChangeQuestInfoBarColour` | `CGameScriptInterface::ChangeQuestInfoBarColour` | `0x00891BA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 92 | `Quest.CheckForCameraMessage` | `CGameScriptInterface::CheckForCameraMessage` | `0x006E7920` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 93 | `Quest.ClearHeroEnemyOfGuards` | `CGameScriptInterface::ClearHeroEnemyOfGuards` | `0x008961B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 94 | `Quest.ClearThingBestEnemyTarget` | `CGameScriptInterface::ClearThingBestEnemyTarget` | `0x008A03F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 95 | `Quest.ConfiscateItemsOfTypeFromHero` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | `0x00898840` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 96 | `Quest.CreateExperienceOrb` | `CGameScriptInterface::CreateExperienceOrb` | `0x0089FBB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 97 | `Quest.CreateExplosion` | `CGameScriptInterface::CreateExplosion` | `0x0089FC80` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 98 | `Quest.CreateLight` | `CGameScriptInterface::CreateLight` | `0x0089FAA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 99 | `Quest.CreatePhysicalBarrier` | `CGameScriptInterface::CreatePhysicalBarrier` | `0x0089FD20` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 100 | `Quest.CreateRumble` | `CGameScriptInterface::CreateRumble` | `0x0089FDA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
