# ForgeFSE binding verification queue

Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and
prototype-review targets, not executable bindings. The unattended agent may advance a row
to structural review; it cannot set `hookApproved`.

- FSE recommendations: 483
- Unique retail target addresses: 483
- Needs reversal: 0
- Agent reviewed: 112
- Implementation verified: 370
- Hook approved: 0
- Direct wrapper/retail signatures: 174
- Wrapper adapters required: 283
- Signature/arity review required: 26

| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |
|---:|---|---|---|---|---|---|:---:|---|---:|
| 1 | `Quest.ApplyHeroPenaltyForDeath` | `CGameScriptInterface::ApplyHeroPenaltyForDeath` | `0x008977F0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 2 | `Quest.CancelHeroTeleportEffects` | `CGameScriptInterface::CancelHeroTeleportEffects` | `0x008A0820` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 3 | `Quest.ConfiscateAllHeroItems` | `CGameScriptInterface::ConfiscateAllHeroItems` | `0x008986D0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 4 | `Quest.IsHeroHandLampLit` | `CGameScriptInterface::IsHeroHandLampLit` | `0x00898E60` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 5 | `Quest.IsPlayerWieldingWeapon` | `CGameScriptInterface::IsPlayerWieldingWeapon` | `0x00897480` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 6 | `Quest.IsToFailQuestOnDeath` | `CGameScriptInterface::IsToFailQuestOnDeath` | `0x00897030` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 7 | `Quest.MsgIsTutorialClickedPast` | `CGameScriptInterface::MsgIsTutorialClickedPast` | `0x008943C0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 8 | `Quest.MsgOnGameSavedManually` | `CGameScriptInterface::MsgOnGameSavedManually` | `0x00893E70` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 9 | `Quest.RemoveAllCutSceneSkippedMessages` | `CGameScriptInterface::RemoveAllCutSceneSkippedMessages` | `0x0089AF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 10 | `Quest.RemoveAllHeroWeapons` | `CGameScriptInterface::RemoveAllHeroWeapons` | `0x00898600` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 11 | `Quest.ReturnAllConfiscatedItemsToHero` | `CGameScriptInterface::ReturnAllConfiscatedItemsToHero` | `0x00898940` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 12 | `Quest.SetGuardsIgnoreCrimes` | `CGameScriptInterface::SetGuardsIgnoreCrimes` | `0x00898540` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 13 | `Quest.SetHeroAsHavingHadGaySex` | `CGameScriptInterface::SetHeroAsHavingHadGaySex` | `0x00897DB0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 14 | `Quest.SetHeroAsHavingHadSex` | `CGameScriptInterface::SetHeroAsHavingHadSex` | `0x00897D50` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 15 | `Quest.SetHeroHandLampAsLit` | `CGameScriptInterface::SetHeroHandLampAsLit` | `0x00898EC0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 16 | `Quest.SetHeroWeaponsAsUsable` | `CGameScriptInterface::SetHeroWeaponsAsUsable` | `0x00898410` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 17 | `Quest.SetHeroWillAsUsable` | `CGameScriptInterface::SetHeroWillAsUsable` | `0x00898340` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 18 | `Quest.SetMoralityChangingAsEnabled` | `CGameScriptInterface::SetMoralityChangingAsEnabled` | `0x0089EF30` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 19 | `Quest.SetWeaponOutCrimeEnabled` | `CGameScriptInterface::SetWeaponOutCrimeEnabled` | `0x008984E0` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 20 | `Quest.SetWhetherToFailQuestOnDeath` | `CGameScriptInterface::SetWhetherToFailQuestOnDeath` | `0x00897090` | direct-signature | unique | strong | yes | agent-reviewed | 245 |
| 21 | `Quest.AddCrimeCommitted` | `CGameScriptInterface::AddCrimeCommitted` | `0x0089FEB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 22 | `Quest.AddTattooToHero` | `CGameScriptInterface::AddTattooToHero` | `0x008981C0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 23 | `Quest.ApplyScriptBrush` | `CGameScriptInterface::ApplyScriptBrush` | `0x0088F480` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 24 | `Quest.CanThingBe_Heard_ByOtherThing` | `CGameScriptInterface::CanThingBe_Heard_ByOtherThing` | `0x0089CD70` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 25 | `Quest.ChangeHeroHairstyle` | `CGameScriptInterface::ChangeHeroHairstyle` | `0x0089B450` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 26 | `Quest.CheckForCameraMessage` | `CGameScriptInterface::CheckForCameraMessage` | `0x006E7920` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 27 | `Quest.ClearHeroEnemyOfGuards` | `CGameScriptInterface::ClearHeroEnemyOfGuards` | `0x008961B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 28 | `Quest.ClearThingBestEnemyTarget` | `CGameScriptInterface::ClearThingBestEnemyTarget` | `0x008A03F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 29 | `Quest.ConfiscateItemsOfTypeFromHero` | `CGameScriptInterface::ConfiscateItemsOfTypeFromHero` | `0x00898840` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 30 | `Quest.CreateExperienceOrb` | `CGameScriptInterface::CreateExperienceOrb` | `0x0089FBB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 31 | `Quest.CreateExplosion` | `CGameScriptInterface::CreateExplosion` | `0x0089FC80` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 32 | `Quest.CreateLight` | `CGameScriptInterface::CreateLight` | `0x0089FAA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 33 | `Quest.CreatePhysicalBarrier` | `CGameScriptInterface::CreatePhysicalBarrier` | `0x0089FD20` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 34 | `Quest.CreateRumble` | `CGameScriptInterface::CreateRumble` | `0x0089FDA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 35 | `Quest.DeregisterTimer` | `CGameScriptInterface::DeregisterTimer` | `0x006E7690` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 36 | `Quest.EntityFollowThing` | `CGameScriptInterface::EntityFollowThing` | `0x008954A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 37 | `Quest.EntityPostOpinionDeedKeepSearchingForWitnesses` | `CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses` | `0x0089C7B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 38 | `Quest.EntityPostOpinionDeedToAll` | `CGameScriptInterface::EntityPostOpinionDeedToAll` | `0x0089C560` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 39 | `Quest.EntityPostOpinionDeedToRecipient` | `CGameScriptInterface::EntityPostOpinionDeedToRecipient` | `0x0089C670` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 40 | `Quest.EntityPostOpinionDeedToRecipientVillage` | `CGameScriptInterface::EntityPostOpinionDeedToRecipientVillage` | `0x0089C710` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 41 | `Quest.EntitySetAppearanceSeed` | `CGameScriptInterface::EntitySetAppearanceSeed` | `0x008A0BC0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 42 | `Quest.EntitySetAsAbleToRegionFollowWhenMarried` | `CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried` | `0x008978E0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 43 | `Quest.EntitySetAsDamageable` | `CGameScriptInterface::EntitySetAsDamageable` | `0x0088EAD0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 44 | `Quest.EntitySetAsMarryable` | `CGameScriptInterface::EntitySetAsMarryable` | `0x00894DD0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 45 | `Quest.EntitySetAsSittingOnFloor` | `CGameScriptInterface::EntitySetAsSittingOnFloor` | `0x008A8C60` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 46 | `Quest.EntitySetInLimbo` | `CGameScriptInterface::EntitySetInLimbo` | `0x0088FD00` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 47 | `Quest.EntitySetMaxNumberOfAttackers` | `CGameScriptInterface::EntitySetMaxNumberOfAttackers` | `0x0089E130` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 48 | `Quest.EntitySetOpinionAttitudeEnabled` | `CGameScriptInterface::EntitySetOpinionAttitudeEnabled` | `0x0089C380` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 49 | `Quest.EntitySetOpinionDeedTypeEnabled` | `CGameScriptInterface::EntitySetOpinionDeedTypeEnabled` | `0x0089C330` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 50 | `Quest.EntitySetOpinionReactionEnabled` | `CGameScriptInterface::EntitySetOpinionReactionEnabled` | `0x0089C3D0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 51 | `Quest.EntityStopFollowing` | `CGameScriptInterface::EntityStopFollowing` | `0x008A7980` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 52 | `Quest.EntityWillTeleportToArea` | `CGameScriptInterface::EntityWillTeleportToArea` | `0x008A93A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 53 | `Quest.GetBestScoreBlackjack` | `CGameScriptInterface::GetBestScoreBlackjack` | `0x008A1650` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 54 | `Quest.GetBestScoreCoinGolfOakVale` | `CGameScriptInterface::GetBestScoreCoinGolfOakVale` | `0x008A16B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 55 | `Quest.GetBestScoreCoinGolfSnowSpire` | `CGameScriptInterface::GetBestScoreCoinGolfSnowSpire` | `0x008A1710` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 56 | `Quest.GetBestScoreShoveHaPenny` | `CGameScriptInterface::GetBestScoreShoveHaPenny` | `0x008A1770` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 57 | `Quest.GetHero` | `CGameScriptInterface::GetHero` | `0x00891CA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 58 | `Quest.GetHeroGold` | `CGameScriptInterface::GetHeroGold` | `0x00898FA0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 59 | `Quest.GetHeroTitle` | `CGameScriptInterface::GetHeroTitle` | `0x00897880` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 60 | `Quest.GetNumberOfTimesHeroHasHadSex` | `CGameScriptInterface::GetNumberOfTimesHeroHasHadSex` | `0x00897C90` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 61 | `Quest.GiveHeroExperience` | `CGameScriptInterface::GiveHeroExperience` | `0x00899000` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 62 | `Quest.GiveThingBestEnemyTarget` | `CGameScriptInterface::GiveThingBestEnemyTarget` | `0x008A0260` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 63 | `Quest.GiveThingItemInSlot` | `CGameScriptInterface::GiveThingItemInSlot` | `0x00895190` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 64 | `Quest.IsEntityFollowingHero` | `CGameScriptInterface::IsEntityFollowingHero` | `0x008955C0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 65 | `Quest.IsGiftFriendly` | `CGameScriptInterface::IsGiftFriendly` | `0x00894C90` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 66 | `Quest.IsGiftOffensive` | `CGameScriptInterface::IsGiftOffensive` | `0x00894D30` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 67 | `Quest.IsGiftRomantic` | `CGameScriptInterface::IsGiftRomantic` | `0x00894BF0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 68 | `Quest.IsHeroPerformingExpression` | `CGameScriptInterface::IsHeroPerformingExpression` | `0x008953A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 69 | `Quest.IsLevelLoaded` | `CGameScriptInterface::IsLevelLoaded` | `0x0088E2A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 70 | `Quest.IsPlayerCarryingItemOfType` | `CGameScriptInterface::IsPlayerCarryingItemOfType` | `0x008973A0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 71 | `Quest.KickOffQuestStartScreen` | `CGameScriptInterface::KickOffQuestStartScreen` | `0x00896B60` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 72 | `Quest.MiniMapAllowRouteBetweenRegions` | `CGameScriptInterface::MiniMapAllowRouteBetweenRegions` | `0x0088E3D0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 73 | `Quest.MsgIsLevelLoaded` | `CGameScriptInterface::MsgIsLevelLoaded` | `0x008931B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 74 | `Quest.MsgIsLevelUnloaded` | `CGameScriptInterface::MsgIsLevelUnloaded` | `0x00893240` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 75 | `Quest.OpenChest` | `CGameScriptInterface::OpenChest` | `0x00896390` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 76 | `Quest.OpinionSourceSetAsAttentionGrabbing` | `CGameScriptInterface::OpinionSourceSetAsAttentionGrabbing` | `0x0089C510` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 77 | `Quest.Play2DSound` | `CGameScriptInterface::Play2DSound` | `0x0088F570` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 78 | `Quest.PlaySoundAtPos` | `CGameScriptInterface::PlaySoundAtPos` | `0x00891170` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 79 | `Quest.RemoveOpinionDeedStillSearchingForWitnesses` | `CGameScriptInterface::RemoveOpinionDeedStillSearchingForWitnesses` | `0x0089C850` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 80 | `Quest.SetCreatureGeneratorsEnabled` | `CGameScriptInterface::SetCreatureGeneratorsEnabled` | `0x00892CF0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 81 | `Quest.SetHeroAsWearing` | `CGameScriptInterface::SetHeroAsWearing` | `0x0089B3F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 82 | `Quest.SetNumberOfTimesHeroHasHadSex` | `CGameScriptInterface::SetNumberOfTimesHeroHasHadSex` | `0x00897CF0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 83 | `Quest.SetQuestInfoText` | `CGameScriptInterface::SetQuestInfoText` | `0x00891A00` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 84 | `Quest.SetReadableObjectText` | `CGameScriptInterface::SetReadableObjectText` | `0x008A0F70` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 85 | `Quest.SetThingAndCarriedItemsNotAffectedByScreenFilter` | `CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter` | `0x008A99F0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 86 | `Quest.SetTrapAsActive` | `CGameScriptInterface::SetTrapAsActive` | `0x00895130` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 87 | `Quest.SetWanderCentrePoint` | `CGameScriptInterface::SetWanderCentrePoint` | `0x008A23B0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 88 | `Quest.TakeObjectFromHero` | `CGameScriptInterface::TakeObjectFromHero` | `0x00898DB0` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 89 | `Quest.UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | `CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter` | `0x00897150` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 90 | `Quest.WaitForCameraMessage` | `CGameScriptInterface::WaitForCameraMessage` | `0x006E7820` | wrapper-adapter | unique | strong | yes | agent-reviewed | 245 |
| 91 | `Entity.MsgWhoHitMe` | `CGameScriptThing::MsgWhoHitMe` | `0x008D1890` | wrapper-adapter | unique | strong | yes | agent-reviewed | 125 |
| 92 | `Quest.AddLineToConversation` | `CGameScriptInterface::AddLineToConversation` | `0x00890750` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 93 | `Quest.AddScreenMessage` | `CGameScriptInterface::AddScreenMessage` | `0x00892850` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 94 | `Quest.CameraUseCameraPoint` | `CGameScriptInterface::CameraUseCameraPoint` | `0x00891070` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 95 | `Quest.EnableGuards` | `CGameScriptInterface::EnableGuards` | `0x00896270` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 96 | `Quest.EnableVillagerDefTypes` | `CGameScriptInterface::EnableVillagerDefTypes` | `0x008962D0` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 97 | `Quest.GetNumberOfItemsOfTypeInInventory` | `CGameScriptInterface::GetNumberOfItemsOfTypeInInventory` | `0x00897190` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 98 | `Quest.GiveHeroGold` | `CGameScriptInterface::GiveHeroGold` | `0x00898F40` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 99 | `Quest.SetQuestCardObjective` | `CGameScriptInterface::SetQuestCardObjective` | `0x00896A30` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
| 100 | `Quest.SetQuestInfoName` | `CGameScriptInterface::SetQuestInfoName` | `0x00891940` | wrapper-adapter | ambiguous | strong | yes | agent-reviewed | 245 |
