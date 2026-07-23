# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 462
- Unique retail target addresses: 462
- Needs reversal: 382
- Agent reviewed: 78
- Implementation verified: 2
- Hook approved: 0
- Direct wrapper/retail signatures: 167
- Wrapper adapters required: 269
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.IsInCutscene` | `CGameScriptInterface::IsInCutscene` | `0x00892270` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 2 | `Quest.IsPlayerCreatureBlocking` | `CGameScriptInterface::IsPlayerCreatureBlocking` | `0x00899BA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 3 | `Quest.IsPlayerCreatureFlourishEnabled` | `CGameScriptInterface::IsPlayerCreatureFlourishEnabled` | `0x00899DB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 4 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 5 | `Quest.IsRadialBlurFadeActive` | `CGameScriptInterface::IsRadialBlurFadeActive` | `0x00890170` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 6 | `Quest.IsSafeToDisplayGameInfo` | `CGameScriptInterface::IsSafeToDisplayGameInfo` | `0x00892B30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 7 | `Quest.IsSavingEnabled` | `CGameScriptInterface::IsSavingEnabled` | `0x0088F950` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 8 | `Quest.IsScreenFadingOut` | `CGameScriptInterface::IsScreenFadingOut` | `0x0088E4D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 9 | `Quest.IsTeleportingActive` | `CGameScriptInterface::IsTeleportingActive` | `0x0088F420` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 10 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 11 | `Quest.IsTutorialSystemEnabled` | `CGameScriptInterface::IsTutorialSystemEnabled` | `0x0088F3E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 12 | `Quest.MiniMapRemoveAllMarkers` | `CGameScriptInterface::MiniMapRemoveAllMarkers` | `0x00892030` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 13 | `Quest.MiniMapSetAsEnabled` | `CGameScriptInterface::MiniMapSetAsEnabled` | `0x0088E400` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 14 | `Quest.MsgIsGameInfoClickedPast` | `CGameScriptInterface::MsgIsGameInfoClickedPast` | `0x00894370` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 15 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 16 | `Quest.MsgOnChestOpeningCancelled` | `CGameScriptInterface::MsgOnChestOpeningCancelled` | `0x008942C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 17 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 18 | `Quest.RadialBlurFadeOut` | `CGameScriptInterface::RadialBlurFadeOut` | `0x00890150` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 19 | `Quest.RemoveAllAvailableQuestCardsFromGuild` | `CGameScriptInterface::RemoveAllAvailableQuestCardsFromGuild` | `0x00891510` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 20 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 21 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 22 | `Quest.RemoveAllQuestInfoElements` | `CGameScriptInterface::RemoveAllQuestInfoElements` | `0x00891C30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 23 | `Quest.RemoveHeroClothing` | `CGameScriptInterface::RemoveHeroClothing` | `0x0089B390` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 24 | `Quest.RemoveHeroHairstyle` | `CGameScriptInterface::RemoveHeroHairstyle` | `0x0089B4D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 25 | `Quest.ResetCreatureCreationDelayFrames` | `CGameScriptInterface::ResetCreatureCreationDelayFrames` | `0x006E7D40` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 26 | `Quest.ResetDeathRecoveryMarkerNameToDefault` | `CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault` | `0x00896FD0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 27 | `Quest.ResetPlayerCreatureCombatMultiplier` | `CGameScriptInterface::ResetPlayerCreatureCombatMultiplier` | `0x00899D50` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 28 | `Quest.ResetPlayerCreatureOnlyTarget` | `CGameScriptInterface::ResetPlayerCreatureOnlyTarget` | `0x00899E90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 29 | `Quest.ResetToDefaultTheme` | `CGameScriptInterface::ResetToDefaultTheme` | `0x0088FF10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 30 | `Quest.ResetToDefaultThemeExternals` | `CGameScriptInterface::ResetToDefaultThemeExternals` | `0x0088FFF0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 31 | `Quest.ResetWhetherToFailQuestOnDeathToDefault` | `CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault` | `0x008970F0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 32 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 33 | `Quest.ScreenFilterFadeOut` | `CGameScriptInterface::ScreenFilterFadeOut` | `0x00890200` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 34 | `Quest.SetAllowScreenFadingIfAlreadyFaded` | `CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded` | `0x0088E920` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 35 | `Quest.SetAllowScreenFadingOnNextRegionChange` | `CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange` | `0x0088E910` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 36 | `Quest.SetAllSoundsAsMuted` | `CGameScriptInterface::SetAllSoundsAsMuted` | `0x0088E090` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 37 | `Quest.SetBettingAccept` | `CGameScriptInterface::SetBettingAccept` | `0x0088FB60` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 38 | `Quest.SetBettingActive` | `CGameScriptInterface::SetBettingActive` | `0x0088FB40` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 39 | `Quest.SetBoastingEnabled` | `CGameScriptInterface::SetBoastingEnabled` | `0x0088FC20` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 40 | `Quest.SetCheapHeadLooking` | `CGameScriptInterface::SetCheapHeadLooking` | `0x0088FAE0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 41 | `Quest.SetCountBetMoneyDown` | `CGameScriptInterface::SetCountBetMoneyDown` | `0x0088FBA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 42 | `Quest.SetExperienceSpendingAsEnabled` | `CGameScriptInterface::SetExperienceSpendingAsEnabled` | `0x0088F460` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 43 | `Quest.SetGlobalTargetingDistanceOffset` | `CGameScriptInterface::SetGlobalTargetingDistanceOffset` | `0x0088FBE0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 44 | `Quest.SetGuardsIgnoreCrimes` | `CGameScriptInterface::SetGuardsIgnoreCrimes` | `0x00898540` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 45 | `Quest.SetHeroAbleToGainExperience` | `CGameScriptInterface::SetHeroAbleToGainExperience` | `0x00899060` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 46 | `Quest.SetHeroAge` | `CGameScriptInterface::SetHeroAge` | `0x008994E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 47 | `Quest.SetHeroAsApprentice` | `CGameScriptInterface::SetHeroAsApprentice` | `0x00899680` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 48 | `Quest.SetHeroAsHavingHadGaySex` | `CGameScriptInterface::SetHeroAsHavingHadGaySex` | `0x00897DB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 49 | `Quest.SetHeroAsHavingHadSex` | `CGameScriptInterface::SetHeroAsHavingHadSex` | `0x00897D50` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 50 | `Quest.SetHeroAsTeenager` | `CGameScriptInterface::SetHeroAsTeenager` | `0x008995D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 51 | `Quest.SetHeroHandLampAsLit` | `CGameScriptInterface::SetHeroHandLampAsLit` | `0x00898EC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 52 | `Quest.SetHeroSleepingAsEnabled` | `CGameScriptInterface::SetHeroSleepingAsEnabled` | `0x0088F440` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 53 | `Quest.SetHeroWeaponsAsUsable` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `0x00898410` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 54 | `Quest.SetHeroWillAsUsable` | `CGameScriptInterface::SetHeroWillAsUsable` | `0x00898340` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 55 | `Quest.SetHeroWillEnergyAsAbleToRefill` | `CGameScriptInterface::SetHeroWillEnergyAsAbleToRefill` | `0x00899A90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 56 | `Quest.SetHeroWillEnergyLevel` | `CGameScriptInterface::SetHeroWillEnergyLevel` | `0x00899A10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 57 | `Quest.SetHUDEnabled` | `CGameScriptInterface::SetHUDEnabled` | `0x00892050` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 58 | `Quest.SetMoralityChangingAsEnabled` | `CGameScriptInterface::SetMoralityChangingAsEnabled` | `0x0089EF30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 59 | `Quest.SetPlayerUsingMeleeDummies` | `CGameScriptInterface::SetPlayerUsingMeleeDummies` | `0x0088FA80` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 60 | `Quest.SetPlayerUsingRangedDummies` | `CGameScriptInterface::SetPlayerUsingRangedDummies` | `0x0088FAA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 61 | `Quest.SetPlayerUsingWillDummies` | `CGameScriptInterface::SetPlayerUsingWillDummies` | `0x0088FAC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 62 | `Quest.SetPrizeTavernTable` | `CGameScriptInterface::SetPrizeTavernTable` | `0x0088FB20` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 63 | `Quest.SetQuitTavernGame` | `CGameScriptInterface::SetQuitTavernGame` | `0x0088FB00` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 64 | `Quest.SetRegionTextDisplayAsActive` | `CGameScriptInterface::SetRegionTextDisplayAsActive` | `0x0088F430` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 65 | `Quest.SetSavingAsEnabled` | `CGameScriptInterface::SetSavingAsEnabled` | `0x0088F940` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 66 | `Quest.SetSpotTheAdditionBeaten` | `CGameScriptInterface::SetSpotTheAdditionBeaten` | `0x0088FBC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 67 | `Quest.SetSummonerDeathExplosionAffectsHero` | `CGameScriptInterface::SetSummonerDeathExplosionAffectsHero` | `0x0088F470` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 68 | `Quest.SetTeleportingAsActive` | `CGameScriptInterface::SetTeleportingAsActive` | `0x0088F410` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 69 | `Quest.SetTimeOfDay` | `CGameScriptInterface::SetTimeOfDay` | `0x0088FDC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 70 | `Quest.SetTradingPriceMult` | `CGameScriptInterface::SetTradingPriceMult` | `0x0088FC00` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 71 | `Quest.SetWeaponOutCrimeEnabled` | `CGameScriptInterface::SetWeaponOutCrimeEnabled` | `0x008984E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 72 | `Quest.SetWhetherToFailQuestOnDeath` | `CGameScriptInterface::SetWhetherToFailQuestOnDeath` | `0x00897090` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 73 | `Quest.SheatheHeroWeapons` | `CGameScriptInterface::SheatheHeroWeapons` | `0x008916A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 74 | `Quest.StartCountdownTimer` | `CGameScriptInterface::StartCountdownTimer` | `0x0088F8D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 75 | `Quest.StopOverrideMusic` | `CGameScriptInterface::StopOverrideMusic` | `0x008912A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 76 | `Quest.UpdateMiniGameInfoBar` | `CGameScriptInterface::UpdateMiniGameInfoBar` | `0x00891C80` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 77 | `Entity.IsDead` | `CScriptThing::IsDead` | `0x004AA890` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 78 | `Quest.AutoSave` | `CGameScriptInterface::AutoSave` | `0x0088F930` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 79 | `Quest.AutoSaveCheckPoint` | `CGameScriptInterface::AutoSaveCheckPoint` | `0x0088F910` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 80 | `Quest.AutoSaveQuestStart` | `CGameScriptInterface::AutoSaveQuestStart` | `0x0088F920` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 81 | `Quest.ClearAllRumbles` | `CGameScriptInterface::ClearAllRumbles` | `0x00892DF0` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 82 | `Quest.DisplayQuestInfo` | `CGameScriptInterface::DisplayQuestInfo` | `0x00891920` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 83 | `Quest.FastForwardTimeTo` | `CGameScriptInterface::FastForwardTimeTo` | `0x0088FE90` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 84 | `Quest.IsDangerMusicEnabled` | `CGameScriptInterface::IsDangerMusicEnabled` | `0x00891330` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 85 | `Quest.ResetToFrontEnd` | `CGameScriptInterface::ResetToFrontEnd` | `0x0088F970` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 86 | `Quest.SetEnvironmentalEffectsAlwaysUpdate` | `CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `0x00890B90` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 87 | `Quest.StartSneaking` | `CGameScriptInterface::StartSneaking` | `0x008A1300` | direct-signature | ambiguous | strong | yes | needs-reversal | 100 |
| 88 | `Entity.GetCurrentMapName` | `CScriptThing::GetCurrentMapName` | `0x004AA9E0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 89 | `Entity.GetHomeMapName` | `CScriptThing::GetHomeMapName` | `0x004AAA10` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 90 | `Entity.GetHomePos` | `CGameScriptThing::GetHomePos` | `0x008CFE30` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 91 | `Entity.IsOpenDoor` | `CGameScriptThing::IsOpenDoor` | `0x008D0820` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 92 | `Entity.MsgHowLongWasExpressionPerformed` | `CScriptThing::MsgHowLongWasExpressionPerformed` | `0x004AAC10` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 93 | `Entity.MsgIsKilledBy` | `CScriptThing::MsgIsKilledBy` | `0x004AAAF0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 94 | `Entity.MsgWhoHitMe` | `CGameScriptThing::MsgWhoHitMe` | `0x008D1890` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 95 | `Entity.MsgWhoKilledMe` | `CScriptThing::MsgWhoKilledMe` | `0x004ABE60` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 96 | `Quest.ActivateQuest` | `CGameScriptInterface::ActivateQuest` | `0x00892E80` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 97 | `Quest.AddCrimeCommitted` | `CGameScriptInterface::AddCrimeCommitted` | `0x0089FEB0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 98 | `Quest.AddFeatCard` | `CGameScriptInterface::AddFeatCard` | `0x00891540` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 99 | `Quest.AddGossipVillage` | `CGameScriptInterface::AddGossipVillage` | `0x008A9E30` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
| 100 | `Quest.AddNewRumourToCategory` | `CGameScriptInterface::AddNewRumourToCategory` | `0x008A9DD0` | wrapper-adapter | unique | strong | yes | needs-reversal | 100 |
