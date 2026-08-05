# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 476
- Unique retail target addresses: 476
- Needs reversal: 1
- Agent reviewed: 144
- Implementation verified: 328
- Hook approved: 0
- Direct wrapper/retail signatures: 171
- Wrapper adapters required: 279
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.GetAllCreaturesInAreaWithScriptName` | `CGameScriptInterface::GetAllCreaturesInAreaWithScriptName` | `0x008A86C0` | signature-review | unique | strong | yes | needs-reversal | 220 |
| 2 | `Quest.ApplyHeroPenaltyForDeath` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | `0x008977F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 3 | `Quest.CameraDefault` | `CGameScriptInterface::CameraDefault` | `0x00890FF0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 4 | `Quest.CancelHeroTeleportEffects` | `CGameScriptInterface::CancelHeroTeleportEffects` | `0x008A0820` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 5 | `Quest.ChangeHeroHealthBy` | `CGameScriptInterface::ChangeHeroHealthBy` | `0x00891720` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 6 | `Quest.ConfiscateAllHeroItems` | `CGameScriptInterface::ConfiscateAllHeroItems` | `0x008986D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 7 | `Quest.GetBestTimeGuessTheAddition` | `CGameScriptInterface::GetBestTimeGuessTheAddition` | `0x008A17D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 8 | `Quest.GetBestTimePairs` | `CGameScriptInterface::GetBestTimePairs` | `0x008A1590` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 9 | `Quest.GetBestTimeSorting` | `CGameScriptInterface::GetBestTimeSorting` | `0x008A15F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 10 | `Quest.GetHeroHasChildren` | `CGameScriptInterface::GetHeroHasChildren` | `0x00897BD0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 11 | `Quest.GetHeroHasCurrentMarriage` | `CGameScriptInterface::GetHeroHasCurrentMarriage` | `0x00897B10` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 12 | `Quest.GetHeroHasDivorcedMarriage` | `CGameScriptInterface::GetHeroHasDivorcedMarriage` | `0x00897B70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 13 | `Quest.GetHeroHasMarried` | `CGameScriptInterface::GetHeroHasMarried` | `0x00897AA0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 14 | `Quest.GetHeroHasMurderedWife` | `CGameScriptInterface::GetHeroHasMurderedWife` | `0x00897C30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 15 | `Quest.IsHeroHandLampLit` | `CGameScriptInterface::IsHeroHandLampLit` | `0x00898E60` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 16 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 17 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 18 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 19 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 20 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 21 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 22 | `Quest.RemoveHeroHairstyle` | `CGameScriptInterface::RemoveHeroHairstyle` | `0x0089B4D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 23 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 24 | `Quest.SetGuardsIgnoreCrimes` | `CGameScriptInterface::SetGuardsIgnoreCrimes` | `0x00898540` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 25 | `Quest.SetHeroAsHavingHadGaySex` | `CGameScriptInterface::SetHeroAsHavingHadGaySex` | `0x00897DB0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 26 | `Quest.SetHeroAsHavingHadSex` | `CGameScriptInterface::SetHeroAsHavingHadSex` | `0x00897D50` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 27 | `Quest.SetHeroHandLampAsLit` | `CGameScriptInterface::SetHeroHandLampAsLit` | `0x00898EC0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 28 | `Quest.SetHeroWeaponsAsUsable` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `0x00898410` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 29 | `Quest.SetHeroWillAsUsable` | `CGameScriptInterface::SetHeroWillAsUsable` | `0x00898340` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 30 | `Quest.SetMoralityChangingAsEnabled` | `CGameScriptInterface::SetMoralityChangingAsEnabled` | `0x0089EF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 31 | `Quest.SetTimeOfDay` | `CGameScriptInterface::SetTimeOfDay` | `0x0088FDC0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 32 | `Quest.SetWeaponOutCrimeEnabled` | `CGameScriptInterface::SetWeaponOutCrimeEnabled` | `0x008984E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 33 | `Quest.SetWhetherToFailQuestOnDeath` | `CGameScriptInterface::SetWhetherToFailQuestOnDeath` | `0x00897090` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 34 | `Quest.FastForwardTimeTo` | `CGameScriptInterface::FastForwardTimeTo` | `0x0088FE90` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 35 | `Quest.SetEnvironmentalEffectsAlwaysUpdate` | `CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate` | `0x00890B90` | direct-signature | ambiguous | strong | yes | agent-reviewed | 245 |
| 36 | `Quest.AddCrimeCommitted` | `CGameScriptInterface::AddCrimeCommitted` | `0x0089FEB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 37 | `Quest.AddTattooToHero` | `CGameScriptInterface::AddTattooToHero` | `0x008981C0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 38 | `Quest.ApplyScriptBrush` | `CGameScriptInterface::ApplyScriptBrush` | `0x0088F480` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 39 | `Quest.CanThingBe_Heard_ByOtherThing` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | `0x0089CD70` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 40 | `Quest.ChangeHeroHairstyle` | `CGameScriptInterface::ChangeHeroHairstyle` | `0x0089B450` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 41 | `Quest.CheckForCameraMessage` | `CGameScriptInterface::CheckForCameraMessage` | `0x006E7920` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 42 | `Quest.ClearHeroEnemyOfGuards` | `CGameScriptInterface::ClearHeroEnemyOfGuards` | `0x008961B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 43 | `Quest.ClearThingBestEnemyTarget` | `CGameScriptInterface::ClearThingBestEnemyTarget` | `0x008A03F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 44 | `Quest.ConfiscateItemsOfTypeFromHero` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | `0x00898840` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 45 | `Quest.CreateExperienceOrb` | `CGameScriptInterface::CreateExperienceOrb` | `0x0089FBB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 46 | `Quest.CreateExplosion` | `CGameScriptInterface::CreateExplosion` | `0x0089FC80` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 47 | `Quest.CreateLight` | `CGameScriptInterface::CreateLight` | `0x0089FAA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 48 | `Quest.CreatePhysicalBarrier` | `CGameScriptInterface::CreatePhysicalBarrier` | `0x0089FD20` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 49 | `Quest.CreateRumble` | `CGameScriptInterface::CreateRumble` | `0x0089FDA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 50 | `Quest.CreatureGeneratorAddTriggerer` | `CGameScriptInterface::CreatureGeneratorAddTriggerer` | `0x0089EC60` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 51 | `Quest.DeregisterTimer` | `CGameScriptInterface::DeregisterTimer` | `0x006E7690` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 52 | `Quest.EntityFollowThing` | `CGameScriptInterface::EntityFollowThing` | `0x008954A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 53 | `Quest.EntityPostOpinionDeedKeepSearchingForWitnesses` | `CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses` | `0x0089C7B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 54 | `Quest.EntityPostOpinionDeedToAll` | `CGameScriptInterface::EntityPostOpinionDeedToAll` | `0x0089C560` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 55 | `Quest.EntityPostOpinionDeedToRecipient` | `CGameScriptInterface::EntityPostOpinionDeedToRecipient` | `0x0089C670` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 56 | `Quest.EntityPostOpinionDeedToRecipientVillage` | `CGameScriptInterface::EntityPostOpinionDeedToRecipientVillage` | `0x0089C710` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 57 | `Quest.EntitySetAppearanceSeed` | `CGameScriptInterface::EntitySetAppearanceSeed` | `0x008A0BC0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 58 | `Quest.EntitySetAsAbleToRegionFollowWhenMarried` | `CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried` | `0x008978E0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 59 | `Quest.EntitySetAsDamageable` | `CGameScriptInterface::EntitySetAsDamageable` | `0x0088EAD0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 60 | `Quest.EntitySetAsMarryable` | `CGameScriptInterface::EntitySetAsMarryable` | `0x00894DD0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 61 | `Quest.EntitySetAsSittingOnFloor` | `CGameScriptInterface::EntitySetAsSittingOnFloor` | `0x008A8C60` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 62 | `Quest.EntitySetCombatEnabled` | `CGameScriptInterface::EntitySetCombatEnabled` | `0x0088F710` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 63 | `Quest.EntitySetInLimbo` | `CGameScriptInterface::EntitySetInLimbo` | `0x0088FD00` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 64 | `Quest.EntitySetMaxNumberOfAttackers` | `CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `0x0089E130` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 65 | `Quest.EntitySetOpinionAttitudeEnabled` | `CGameScriptInterface::EntitySetOpinionAttitudeEnabled` | `0x0089C380` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 66 | `Quest.EntitySetOpinionDeedTypeEnabled` | `CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `0x0089C330` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 67 | `Quest.EntitySetOpinionReactionEnabled` | `CGameScriptInterface::EntitySetOpinionReactionEnabled` | `0x0089C3D0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 68 | `Quest.EntityStopFollowing` | `CGameScriptInterface::EntityStopFollowing` | `0x008A7980` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 69 | `Quest.EntityWillTeleportToArea` | `CGameScriptInterface::EntityWillTeleportToArea` | `0x008A93A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 70 | `Quest.GetBestScoreBlackjack` | `CGameScriptInterface::GetBestScoreBlackjack` | `0x008A1650` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 71 | `Quest.GetBestScoreCoinGolfOakVale` | `CGameScriptInterface::GetBestScoreCoinGolfOakVale` | `0x008A16B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 72 | `Quest.GetBestScoreCoinGolfSnowSpire` | `CGameScriptInterface::GetBestScoreCoinGolfSnowSpire` | `0x008A1710` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 73 | `Quest.GetBestScoreShoveHaPenny` | `CGameScriptInterface::GetBestScoreShoveHaPenny` | `0x008A1770` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 74 | `Quest.GetHero` | `CGameScriptInterface::GetHero` | `0x00891CA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 75 | `Quest.GetHeroGold` | `CGameScriptInterface::GetHeroGold` | `0x00898FA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 76 | `Quest.GetHeroTitle` | `CGameScriptInterface::GetHeroTitle` | `0x00897880` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 77 | `Quest.GetNumberOfTimesHeroHasHadSex` | `CGameScriptInterface::GetNumberOfTimesHeroHasHadSex` | `0x00897C90` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 78 | `Quest.GetNumHousesOwned` | `CGameScriptInterface::GetNumHousesOwned` | `0x008A12A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 79 | `Quest.GiveHeroExperience` | `CGameScriptInterface::GiveHeroExperience` | `0x00899000` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 80 | `Quest.GiveThingBestEnemyTarget` | `CGameScriptInterface::GiveThingBestEnemyTarget` | `0x008A0260` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 81 | `Quest.GiveThingItemInSlot` | `CGameScriptInterface::GiveThingItemInSlot` | `0x00895190` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 82 | `Quest.HeroHasExpression` | `CGameScriptInterface::HeroHasExpression` | `0x0088FC90` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 83 | `Quest.IsEntityFollowingHero` | `CGameScriptInterface::IsEntityFollowingHero` | `0x008955C0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 84 | `Quest.IsGiftFriendly` | `CGameScriptInterface::IsGiftFriendly` | `0x00894C90` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 85 | `Quest.IsGiftOffensive` | `CGameScriptInterface::IsGiftOffensive` | `0x00894D30` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 86 | `Quest.IsGiftRomantic` | `CGameScriptInterface::IsGiftRomantic` | `0x00894BF0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 87 | `Quest.IsHeroPerformingExpression` | `CGameScriptInterface::IsHeroPerformingExpression` | `0x008953A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 88 | `Quest.IsLevelLoaded` | `CGameScriptInterface::IsLevelLoaded` | `0x0088E2A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 89 | `Quest.IsPlayerCarryingItemOfType` | `CGameScriptInterface::IsPlayerCarryingItemOfType` | `0x008973A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 90 | `Quest.IsSoundPlaying` | `CGameScriptInterface::IsSoundPlaying` | `0x0088F610` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 91 | `Quest.KickOffQuestStartScreen` | `CGameScriptInterface::KickOffQuestStartScreen` | `0x00896B60` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 92 | `Quest.ManuallyResetTrap` | `CGameScriptInterface::ManuallyResetTrap` | `0x008950E0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 93 | `Quest.MiniMapAllowRouteBetweenRegions` | `CGameScriptInterface::MiniMapAllowRouteBetweenRegions` | `0x0088E3D0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 94 | `Quest.MsgIsLevelLoaded` | `CGameScriptInterface::MsgIsLevelLoaded` | `0x008931B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 95 | `Quest.MsgIsLevelUnloaded` | `CGameScriptInterface::MsgIsLevelUnloaded` | `0x00893240` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 96 | `Quest.OpenChest` | `CGameScriptInterface::OpenChest` | `0x00896390` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 97 | `Quest.OpinionSourceSetAsAttentionGrabbing` | `CGameScriptInterface::OpinionSourceSetAsAttentionGrabbing` | `0x0089C510` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 98 | `Quest.Play2DSound` | `CGameScriptInterface::Play2DSound` | `0x0088F570` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 99 | `Quest.PlaySoundAtPos` | `CGameScriptInterface::PlaySoundAtPos` | `0x00891170` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 100 | `Quest.RemoveOpinionDeedStillSearchingForWitnesses` | `CGameScriptInterface::RemoveOpinionDeedStillSearchingForWitnesses` | `0x0089C850` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
