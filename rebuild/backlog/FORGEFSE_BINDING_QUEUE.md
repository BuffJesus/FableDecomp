# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 462
- Unique retail target addresses: 462
- Needs reversal: 350
- Agent reviewed: 110
- Implementation verified: 2
- Hook approved: 0
- Direct wrapper/retail signatures: 167
- Wrapper adapters required: 269
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.ScreenFilterFadeOut` | `CGameScriptInterface::ScreenFilterFadeOut` | `0x00890200` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 2 | `Quest.SetAllowScreenFadingIfAlreadyFaded` | `CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded` | `0x0088E920` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 3 | `Quest.SetAllowScreenFadingOnNextRegionChange` | `CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange` | `0x0088E910` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 4 | `Quest.SetAllSoundsAsMuted` | `CGameScriptInterface::SetAllSoundsAsMuted` | `0x0088E090` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 5 | `Quest.SetBettingAccept` | `CGameScriptInterface::SetBettingAccept` | `0x0088FB60` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 6 | `Quest.SetBettingActive` | `CGameScriptInterface::SetBettingActive` | `0x0088FB40` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 7 | `Quest.SetBoastingEnabled` | `CGameScriptInterface::SetBoastingEnabled` | `0x0088FC20` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 8 | `Quest.SetCheapHeadLooking` | `CGameScriptInterface::SetCheapHeadLooking` | `0x0088FAE0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 9 | `Quest.SetCountBetMoneyDown` | `CGameScriptInterface::SetCountBetMoneyDown` | `0x0088FBA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 10 | `Quest.SetExperienceSpendingAsEnabled` | `CGameScriptInterface::SetExperienceSpendingAsEnabled` | `0x0088F460` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 11 | `Quest.SetGlobalTargetingDistanceOffset` | `CGameScriptInterface::SetGlobalTargetingDistanceOffset` | `0x0088FBE0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 12 | `Quest.SetGuardsIgnoreCrimes` | `CGameScriptInterface::SetGuardsIgnoreCrimes` | `0x00898540` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 13 | `Quest.SetHeroAbleToGainExperience` | `CGameScriptInterface::SetHeroAbleToGainExperience` | `0x00899060` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 14 | `Quest.SetHeroAge` | `CGameScriptInterface::SetHeroAge` | `0x008994E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 15 | `Quest.SetHeroAsApprentice` | `CGameScriptInterface::SetHeroAsApprentice` | `0x00899680` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 16 | `Quest.SetHeroAsHavingHadGaySex` | `CGameScriptInterface::SetHeroAsHavingHadGaySex` | `0x00897DB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 17 | `Quest.SetHeroAsHavingHadSex` | `CGameScriptInterface::SetHeroAsHavingHadSex` | `0x00897D50` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 18 | `Quest.SetHeroAsTeenager` | `CGameScriptInterface::SetHeroAsTeenager` | `0x008995D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 19 | `Quest.SetHeroHandLampAsLit` | `CGameScriptInterface::SetHeroHandLampAsLit` | `0x00898EC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 20 | `Quest.SetHeroSleepingAsEnabled` | `CGameScriptInterface::SetHeroSleepingAsEnabled` | `0x0088F440` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 21 | `Quest.SetHeroWeaponsAsUsable` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `0x00898410` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 22 | `Quest.SetHeroWillAsUsable` | `CGameScriptInterface::SetHeroWillAsUsable` | `0x00898340` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 23 | `Quest.SetHeroWillEnergyAsAbleToRefill` | `CGameScriptInterface::SetHeroWillEnergyAsAbleToRefill` | `0x00899A90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 24 | `Quest.SetHeroWillEnergyLevel` | `CGameScriptInterface::SetHeroWillEnergyLevel` | `0x00899A10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 25 | `Quest.SetHUDEnabled` | `CGameScriptInterface::SetHUDEnabled` | `0x00892050` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 26 | `Quest.SetMoralityChangingAsEnabled` | `CGameScriptInterface::SetMoralityChangingAsEnabled` | `0x0089EF30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 27 | `Quest.SetPlayerUsingMeleeDummies` | `CGameScriptInterface::SetPlayerUsingMeleeDummies` | `0x0088FA80` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 28 | `Quest.SetPlayerUsingRangedDummies` | `CGameScriptInterface::SetPlayerUsingRangedDummies` | `0x0088FAA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 29 | `Quest.SetPlayerUsingWillDummies` | `CGameScriptInterface::SetPlayerUsingWillDummies` | `0x0088FAC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 30 | `Quest.SetPrizeTavernTable` | `CGameScriptInterface::SetPrizeTavernTable` | `0x0088FB20` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 31 | `Quest.SetQuitTavernGame` | `CGameScriptInterface::SetQuitTavernGame` | `0x0088FB00` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 32 | `Quest.SetRegionTextDisplayAsActive` | `CGameScriptInterface::SetRegionTextDisplayAsActive` | `0x0088F430` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 33 | `Quest.SetSavingAsEnabled` | `CGameScriptInterface::SetSavingAsEnabled` | `0x0088F940` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 34 | `Quest.SetSpotTheAdditionBeaten` | `CGameScriptInterface::SetSpotTheAdditionBeaten` | `0x0088FBC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 35 | `Quest.SetSummonerDeathExplosionAffectsHero` | `CGameScriptInterface::SetSummonerDeathExplosionAffectsHero` | `0x0088F470` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 36 | `Quest.SetTeleportingAsActive` | `CGameScriptInterface::SetTeleportingAsActive` | `0x0088F410` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 37 | `Quest.SetTimeOfDay` | `CGameScriptInterface::SetTimeOfDay` | `0x0088FDC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 38 | `Quest.SetTradingPriceMult` | `CGameScriptInterface::SetTradingPriceMult` | `0x0088FC00` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 39 | `Quest.SetWeaponOutCrimeEnabled` | `CGameScriptInterface::SetWeaponOutCrimeEnabled` | `0x008984E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 40 | `Quest.SetWhetherToFailQuestOnDeath` | `CGameScriptInterface::SetWhetherToFailQuestOnDeath` | `0x00897090` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 41 | `Quest.SheatheHeroWeapons` | `CGameScriptInterface::SheatheHeroWeapons` | `0x008916A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 42 | `Quest.StartCountdownTimer` | `CGameScriptInterface::StartCountdownTimer` | `0x0088F8D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 43 | `Quest.StopOverrideMusic` | `CGameScriptInterface::StopOverrideMusic` | `0x008912A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 44 | `Quest.UpdateMiniGameInfoBar` | `CGameScriptInterface::UpdateMiniGameInfoBar` | `0x00891C80` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 45 | `Entity.IsDead` | `CScriptThing::IsDead` | `0x004AA890` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 46 | `Quest.AutoSave` | `CGameScriptInterface::AutoSave` | `0x0088F930` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 47 | `Quest.AutoSaveCheckPoint` | `CGameScriptInterface::AutoSaveCheckPoint` | `0x0088F910` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 48 | `Quest.AutoSaveQuestStart` | `CGameScriptInterface::AutoSaveQuestStart` | `0x0088F920` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 49 | `Quest.ClearAllRumbles` | `CGameScriptInterface::ClearAllRumbles` | `0x00892DF0` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 50 | `Quest.DisplayQuestInfo` | `CGameScriptInterface::DisplayQuestInfo` | `0x00891920` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 51 | `Quest.FastForwardTimeTo` | `CGameScriptInterface::FastForwardTimeTo` | `0x0088FE90` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 52 | `Quest.IsDangerMusicEnabled` | `CGameScriptInterface::IsDangerMusicEnabled` | `0x00891330` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 53 | `Quest.ResetToFrontEnd` | `CGameScriptInterface::ResetToFrontEnd` | `0x0088F970` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 54 | `Quest.SetEnvironmentalEffectsAlwaysUpdate` | `CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `0x00890B90` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 55 | `Quest.StartSneaking` | `CGameScriptInterface::StartSneaking` | `0x008A1300` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 56 | `Entity.GetCurrentMapName` | `CScriptThing::GetCurrentMapName` | `0x004AA9E0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 57 | `Entity.GetHomeMapName` | `CScriptThing::GetHomeMapName` | `0x004AAA10` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 58 | `Entity.GetHomePos` | `CGameScriptThing::GetHomePos` | `0x008CFE30` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 59 | `Entity.IsOpenDoor` | `CGameScriptThing::IsOpenDoor` | `0x008D0820` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 60 | `Entity.MsgHowLongWasExpressionPerformed` | `CScriptThing::MsgHowLongWasExpressionPerformed` | `0x004AAC10` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 61 | `Entity.MsgIsKilledBy` | `CScriptThing::MsgIsKilledBy` | `0x004AAAF0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 62 | `Entity.MsgWhoHitMe` | `CGameScriptThing::MsgWhoHitMe` | `0x008D1890` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 63 | `Entity.MsgWhoKilledMe` | `CScriptThing::MsgWhoKilledMe` | `0x004ABE60` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 64 | `Quest.ActivateQuest` | `CGameScriptInterface::ActivateQuest` | `0x00892E80` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 65 | `Quest.AddCrimeCommitted` | `CGameScriptInterface::AddCrimeCommitted` | `0x0089FEB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 66 | `Quest.AddFeatCard` | `CGameScriptInterface::AddFeatCard` | `0x00891540` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 67 | `Quest.AddGossipVillage` | `CGameScriptInterface::AddGossipVillage` | `0x008A9E30` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 68 | `Quest.AddNewRumourToCategory` | `CGameScriptInterface::AddNewRumourToCategory` | `0x008A9DD0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 69 | `Quest.AddQuestCard` | `CGameScriptInterface::AddQuestCard` | `0x008913F0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 70 | `Quest.AddQuestInfoBar` | `CGameScriptInterface::AddQuestInfoBar` | `0x00891AC0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 71 | `Quest.AddQuestInfoCounterList` | `CGameScriptInterface::AddQuestInfoCounterList` | `0x00891B50` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 72 | `Quest.AddQuestInfoTimer` | `CGameScriptInterface::AddQuestInfoTimer` | `0x00891B20` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 73 | `Quest.AddRumourCategory` | `CGameScriptInterface::AddRumourCategory` | `0x008A9DB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 74 | `Quest.AddScreenTitleMessage` | `CGameScriptInterface::AddScreenTitleMessage` | `0x0089E4C0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 75 | `Quest.AddTattooToHero` | `CGameScriptInterface::AddTattooToHero` | `0x008981C0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 76 | `Quest.ApplyScriptBrush` | `CGameScriptInterface::ApplyScriptBrush` | `0x0088F480` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 77 | `Quest.AreEntitiesEnemies` | `CGameScriptInterface::AreEntitiesEnemies` | `0x0088E6F0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 78 | `Quest.CacheMusicSet` | `CGameScriptInterface::CacheMusicSet` | `0x00891310` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 79 | `Quest.CameraCircleAroundPos` | `CGameScriptInterface::CameraCircleAroundPos` | `0x00892C20` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 80 | `Quest.CameraCircleAroundThing` | `CGameScriptInterface::CameraCircleAroundThing` | `0x00892B50` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 81 | `Quest.CameraEarthquakeIntensityAtPos` | `CGameScriptInterface::CameraEarthquakeIntensityAtPos` | `0x0088ED20` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 82 | `Quest.CameraMoveBetweenLookFromAndLookTo` | `CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo` | `0x00892610` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 83 | `Quest.CameraMoveToPosAndLookAtPos` | `CGameScriptInterface::CameraMoveToPosAndLookAtPos` | `0x00892410` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 84 | `Quest.CanThingBe_Heard_ByOtherThing` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | `0x0089CD70` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 85 | `Quest.CanThingBe_NearlySeen_ByOtherThing` | `CGameScriptInterface::CanThingBe_NearlySeen_ByOtherThing` | `0x0089CD30` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 86 | `Quest.CanThingBe_Seen_ByOtherThing` | `CGameScriptInterface::CanThingBe_Seen_ByOtherThing` | `0x0089CD10` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 87 | `Quest.CanThingBe_Smelled_ByOtherThing` | `CGameScriptInterface::CanThingBe_Smelled_ByOtherThing` | `0x0089CD50` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 88 | `Quest.ChangeHeroHairstyle` | `CGameScriptInterface::ChangeHeroHairstyle` | `0x0089B450` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 89 | `Quest.ChangeQuestInfoBarColour` | `CGameScriptInterface::ChangeQuestInfoBarColour` | `0x00891BA0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 90 | `Quest.CheckForCameraMessage` | `CGameScriptInterface::CheckForCameraMessage` | `0x006E7920` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 91 | `Quest.ClearGossipFactions` | `CGameScriptInterface::ClearGossipFactions` | `0x008AA4D0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 92 | `Quest.ClearGossipVillages` | `CGameScriptInterface::ClearGossipVillages` | `0x008AA270` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 93 | `Quest.ClearHeroEnemyOfGuards` | `CGameScriptInterface::ClearHeroEnemyOfGuards` | `0x008961B0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 94 | `Quest.ClearThingBestEnemyTarget` | `CGameScriptInterface::ClearThingBestEnemyTarget` | `0x008A03F0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 95 | `Quest.ConfiscateItemsOfTypeFromHero` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | `0x00898840` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 96 | `Quest.CreateExperienceOrb` | `CGameScriptInterface::CreateExperienceOrb` | `0x0089FBB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 97 | `Quest.CreateExplosion` | `CGameScriptInterface::CreateExplosion` | `0x0089FC80` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 98 | `Quest.CreateLight` | `CGameScriptInterface::CreateLight` | `0x0089FAA0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 99 | `Quest.CreatePhysicalBarrier` | `CGameScriptInterface::CreatePhysicalBarrier` | `0x0089FD20` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 100 | `Quest.CreateRumble` | `CGameScriptInterface::CreateRumble` | `0x0089FDA0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
