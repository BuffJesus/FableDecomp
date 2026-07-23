# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 462
- Unique retail target addresses: 462
- Needs reversal: 446
- Agent reviewed: 14
- Implementation verified: 2
- Hook approved: 0
- Direct wrapper/retail signatures: 167
- Wrapper adapters required: 269
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.ChangeHeroMoralityDueToPicklock` | `CGameScriptInterface::ChangeHeroMoralityDueToPicklock` | `0x0089A0E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 2 | `Quest.ChangeHeroMoralityDueToTheft` | `CGameScriptInterface::ChangeHeroMoralityDueToTheft` | `0x0089A070` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 3 | `Quest.ClearActiveGossipCategories` | `CGameScriptInterface::ClearActiveGossipCategories` | `0x008A78E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 4 | `Quest.ClearIsGossipForPlayer` | `CGameScriptInterface::ClearIsGossipForPlayer` | `0x008A7930` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 5 | `Quest.ConfiscateAllHeroItems` | `CGameScriptInterface::ConfiscateAllHeroItems` | `0x008986D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 6 | `Quest.ConfiscateAllHeroWeapons` | `CGameScriptInterface::ConfiscateAllHeroWeapons` | `0x008987E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 7 | `Quest.DisplacementMonochromeEffectColourFadeOut` | `CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut` | `0x008901E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 8 | `Quest.DisplayMoneyBag` | `CGameScriptInterface::DisplayMoneyBag` | `0x00891C40` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 9 | `Quest.DisplayTime` | `CGameScriptInterface::DisplayTime` | `0x0088E210` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 10 | `Quest.EnableDangerMusic` | `CGameScriptInterface::EnableDangerMusic` | `0x00891320` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 11 | `Quest.EnableDecals` | `CGameScriptInterface::EnableDecals` | `0x0088F4A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 12 | `Quest.EnableSounds` | `CGameScriptInterface::EnableSounds` | `0x0088F620` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 13 | `Quest.EndLetterBox` | `CGameScriptInterface::EndLetterBox` | `0x006E7290` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 14 | `Quest.FadeScreenIn` | `CGameScriptInterface::FadeScreenIn` | `0x0088E4C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 15 | `Quest.FadeScreenOutUntilNextCallToFadeScreenIn` | `CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn` | `0x008907E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 16 | `Quest.FailAllActiveQuests` | `CGameScriptInterface::FailAllActiveQuests` | `0x00891520` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 17 | `Quest.FixMovieSequenceCamera` | `CGameScriptInterface::FixMovieSequenceCamera` | `0x0088E500` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 18 | `Quest.GetActiveGossipCategoriesSize` | `CGameScriptInterface::GetActiveGossipCategoriesSize` | `0x00893040` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 19 | `Quest.GetBestTimeGuessTheAddition` | `CGameScriptInterface::GetBestTimeGuessTheAddition` | `0x008A17D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 20 | `Quest.GetBestTimePairs` | `CGameScriptInterface::GetBestTimePairs` | `0x008A1590` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 21 | `Quest.GetBestTimeSorting` | `CGameScriptInterface::GetBestTimeSorting` | `0x008A15F0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 22 | `Quest.GetBettingAccept` | `CGameScriptInterface::GetBettingAccept` | `0x0088FB70` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 23 | `Quest.GetBettingActive` | `CGameScriptInterface::GetBettingActive` | `0x0088FB50` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 24 | `Quest.GetBoastingEnabled` | `CGameScriptInterface::GetBoastingEnabled` | `0x0088FC30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 25 | `Quest.GetCheapHeadLooking` | `CGameScriptInterface::GetCheapHeadLooking` | `0x0088FAF0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 26 | `Quest.GetCountBetMoneyDown` | `CGameScriptInterface::GetCountBetMoneyDown` | `0x0088FBB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 27 | `Quest.GetCountdownTimer` | `CGameScriptInterface::GetCountdownTimer` | `0x0088F900` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 28 | `Quest.GetDistanceHeroCanBeHeardFrom` | `CGameScriptInterface::GetDistanceHeroCanBeHeardFrom` | `0x00899700` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 29 | `Quest.GetGameAngleXY` | `CGameScriptInterface::GetGameAngleXY` | `0x006E7340` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 30 | `Quest.GetGlobalTargetingDistanceOffset` | `CGameScriptInterface::GetGlobalTargetingDistanceOffset` | `0x0088FBF0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 31 | `Quest.GetGuildSealRecallAngleXY` | `CGameScriptInterface::GetGuildSealRecallAngleXY` | `0x0088F9E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 32 | `Quest.GetHeroAge` | `CGameScriptInterface::GetHeroAge` | `0x00899560` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 33 | `Quest.GetHeroAttractiveness` | `CGameScriptInterface::GetHeroAttractiveness` | `0x00899930` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 34 | `Quest.GetHeroFatness` | `CGameScriptInterface::GetHeroFatness` | `0x00899850` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 35 | `Quest.GetHeroHasChildren` | `CGameScriptInterface::GetHeroHasChildren` | `0x00897BD0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 36 | `Quest.GetHeroHasCurrentMarriage` | `CGameScriptInterface::GetHeroHasCurrentMarriage` | `0x00897B10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 37 | `Quest.GetHeroHasDivorcedMarriage` | `CGameScriptInterface::GetHeroHasDivorcedMarriage` | `0x00897B70` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 38 | `Quest.GetHeroHasMarried` | `CGameScriptInterface::GetHeroHasMarried` | `0x00897AA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 39 | `Quest.GetHeroHasMurderedWife` | `CGameScriptInterface::GetHeroHasMurderedWife` | `0x00897C30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 40 | `Quest.GetHeroHealth` | `CGameScriptInterface::GetHeroHealth` | `0x00890370` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 41 | `Quest.GetHeroHealthMax` | `CGameScriptInterface::GetHeroHealthMax` | `0x008903A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 42 | `Quest.GetHeroHealthPercentage` | `CGameScriptInterface::GetHeroHealthPercentage` | `0x008903D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 43 | `Quest.GetHeroMorality` | `CGameScriptInterface::GetHeroMorality` | `0x00899F90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 44 | `Quest.GetHeroScariness` | `CGameScriptInterface::GetHeroScariness` | `0x008998C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 45 | `Quest.GetHeroWillEnergyLevel` | `CGameScriptInterface::GetHeroWillEnergyLevel` | `0x008999A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 46 | `Quest.GetIsGossipForPlayerSize` | `CGameScriptInterface::GetIsGossipForPlayerSize` | `0x00893050` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 47 | `Quest.GetPlayerUsingMeleeDummies` | `CGameScriptInterface::GetPlayerUsingMeleeDummies` | `0x0088FA90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 48 | `Quest.GetPlayerUsingRangedDummies` | `CGameScriptInterface::GetPlayerUsingRangedDummies` | `0x0088FAB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 49 | `Quest.GetPlayerUsingWillDummies` | `CGameScriptInterface::GetPlayerUsingWillDummies` | `0x0088FAD0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 50 | `Quest.GetPrizeTavernTable` | `CGameScriptInterface::GetPrizeTavernTable` | `0x0088FB30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 51 | `Quest.GetQuitTavernGame` | `CGameScriptInterface::GetQuitTavernGame` | `0x0088FB10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 52 | `Quest.GetSpotTheAdditionBeaten` | `CGameScriptInterface::GetSpotTheAdditionBeaten` | `0x0088FBD0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 53 | `Quest.GetTradingPriceMult` | `CGameScriptInterface::GetTradingPriceMult` | `0x0088FC10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 54 | `Quest.GiveHeroMorality` | `CGameScriptInterface::GiveHeroMorality` | `0x00899EF0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 55 | `Quest.IncreaseHeroRenownLevel` | `CGameScriptInterface::IncreaseHeroRenownLevel` | `0x00899220` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 56 | `Quest.IsCameraInScriptedMode` | `CGameScriptInterface::IsCameraInScriptedMode` | `0x0088ECC0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 57 | `Quest.IsHeroAllowedHenchmenInCurrentRegion` | `CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion` | `0x00891EA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 58 | `Quest.IsHeroChild` | `CGameScriptInterface::IsHeroChild` | `0x008902B0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 59 | `Quest.IsHeroHandLampLit` | `CGameScriptInterface::IsHeroHandLampLit` | `0x00898E60` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 60 | `Quest.IsHeroInTavernGame` | `CGameScriptInterface::IsHeroInTavernGame` | `0x008913C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 61 | `Quest.IsHeroNaked` | `CGameScriptInterface::IsHeroNaked` | `0x0089B330` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 62 | `Quest.IsHeroOnQuest` | `CGameScriptInterface::IsHeroOnQuest` | `0x00891870` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 63 | `Quest.IsHeroRenownLevelFull` | `CGameScriptInterface::IsHeroRenownLevelFull` | `0x008991A0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 64 | `Quest.IsHeroSleepingEnabled` | `CGameScriptInterface::IsHeroSleepingEnabled` | `0x0088F450` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 65 | `Quest.IsInCutscene` | `CGameScriptInterface::IsInCutscene` | `0x00892270` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 66 | `Quest.IsPlayerCreatureBlocking` | `CGameScriptInterface::IsPlayerCreatureBlocking` | `0x00899BA0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 67 | `Quest.IsPlayerCreatureFlourishEnabled` | `CGameScriptInterface::IsPlayerCreatureFlourishEnabled` | `0x00899DB0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 68 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 69 | `Quest.IsRadialBlurFadeActive` | `CGameScriptInterface::IsRadialBlurFadeActive` | `0x00890170` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 70 | `Quest.IsSafeToDisplayGameInfo` | `CGameScriptInterface::IsSafeToDisplayGameInfo` | `0x00892B30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 71 | `Quest.IsSavingEnabled` | `CGameScriptInterface::IsSavingEnabled` | `0x0088F950` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 72 | `Quest.IsScreenFadingOut` | `CGameScriptInterface::IsScreenFadingOut` | `0x0088E4D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 73 | `Quest.IsTeleportingActive` | `CGameScriptInterface::IsTeleportingActive` | `0x0088F420` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 74 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 75 | `Quest.IsTutorialSystemEnabled` | `CGameScriptInterface::IsTutorialSystemEnabled` | `0x0088F3E0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 76 | `Quest.MiniMapRemoveAllMarkers` | `CGameScriptInterface::MiniMapRemoveAllMarkers` | `0x00892030` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 77 | `Quest.MiniMapSetAsEnabled` | `CGameScriptInterface::MiniMapSetAsEnabled` | `0x0088E400` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 78 | `Quest.MsgIsGameInfoClickedPast` | `CGameScriptInterface::MsgIsGameInfoClickedPast` | `0x00894370` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 79 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 80 | `Quest.MsgOnChestOpeningCancelled` | `CGameScriptInterface::MsgOnChestOpeningCancelled` | `0x008942C0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 81 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 82 | `Quest.RadialBlurFadeOut` | `CGameScriptInterface::RadialBlurFadeOut` | `0x00890150` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 83 | `Quest.RemoveAllAvailableQuestCardsFromGuild` | `CGameScriptInterface::RemoveAllAvailableQuestCardsFromGuild` | `0x00891510` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 84 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 85 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 86 | `Quest.RemoveAllQuestInfoElements` | `CGameScriptInterface::RemoveAllQuestInfoElements` | `0x00891C30` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 87 | `Quest.RemoveHeroClothing` | `CGameScriptInterface::RemoveHeroClothing` | `0x0089B390` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 88 | `Quest.RemoveHeroHairstyle` | `CGameScriptInterface::RemoveHeroHairstyle` | `0x0089B4D0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 89 | `Quest.ResetCreatureCreationDelayFrames` | `CGameScriptInterface::ResetCreatureCreationDelayFrames` | `0x006E7D40` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 90 | `Quest.ResetDeathRecoveryMarkerNameToDefault` | `CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault` | `0x00896FD0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 91 | `Quest.ResetPlayerCreatureCombatMultiplier` | `CGameScriptInterface::ResetPlayerCreatureCombatMultiplier` | `0x00899D50` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 92 | `Quest.ResetPlayerCreatureOnlyTarget` | `CGameScriptInterface::ResetPlayerCreatureOnlyTarget` | `0x00899E90` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 93 | `Quest.ResetToDefaultTheme` | `CGameScriptInterface::ResetToDefaultTheme` | `0x0088FF10` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 94 | `Quest.ResetToDefaultThemeExternals` | `CGameScriptInterface::ResetToDefaultThemeExternals` | `0x0088FFF0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 95 | `Quest.ResetWhetherToFailQuestOnDeathToDefault` | `CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault` | `0x008970F0` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 96 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 97 | `Quest.ScreenFilterFadeOut` | `CGameScriptInterface::ScreenFilterFadeOut` | `0x00890200` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 98 | `Quest.SetAllowScreenFadingIfAlreadyFaded` | `CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded` | `0x0088E920` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 99 | `Quest.SetAllowScreenFadingOnNextRegionChange` | `CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange` | `0x0088E910` | direct-signature | unique | strong | yes | needs-reversal | 100 |
| 100 | `Quest.SetAllSoundsAsMuted` | `CGameScriptInterface::SetAllSoundsAsMuted` | `0x0088E090` | direct-signature | unique | strong | yes | needs-reversal | 100 |
