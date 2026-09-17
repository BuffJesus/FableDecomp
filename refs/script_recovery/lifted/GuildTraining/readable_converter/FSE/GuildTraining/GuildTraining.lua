-- Readable native conversion: Q_GuildTraining. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTraining.Main (retail 0x00d3bc60)
function Main(quest)
    local isLevelLoaded2
    if quest:GetStateInt("GameState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveAllHeroWeapons()
        quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("GuildArrivalHSP"), false)
        while not quest:IsLevelLoaded("LookoutPoint") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_MELEE", "Q_GuildTraining", false)
        if not quest:NewScriptFrame() then return end
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), false, true)
        RunArrivalCutscene(quest)
        quest:FadeScreenOut(0.5, 0.0)
        if not quest:NewScriptFrame() then return end
    end
    quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("GuildTrainingHSP"), false)
    isLevelLoaded2 = quest:IsLevelLoaded("HeroGuildComplex")
    while true do
        if isLevelLoaded2 then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetAllowScreenFadingOnNextRegionChange(true)
            quest:SetRegionTextDisplayAsActive(true)
            quest:AddEntityBinding("AppleGirl", "GuildTraining/Entities/AppleGirl")
            quest:AddEntityBinding("ApprenticeSpeedTest", "GuildTraining/Entities/ApprenticeSpeedTest")
            quest:AddEntityBinding("SpeedFriend", "GuildTraining/Entities/SpeedFriend")
            quest:AddEntityBinding("RaceMarker", "GuildTraining/Entities/RaceMarker")
            quest:AddEntityBinding("MeleeApprentice", "GuildTraining/Entities/MeleeApprentice")
            quest:AddEntityBinding("CombatApprentice", "GuildTraining/Entities/CombatApprentice")
            quest:AddEntityBinding("SkillApprentice", "GuildTraining/Entities/SkillApprentice")
            quest:AddEntityBinding("WillApprentice", "GuildTraining/Entities/WillApprentice")
            quest:AddEntityBinding("HeroBed", "GuildTraining/Entities/HeroBed")
            quest:AddEntityBinding("SkillTarget", "GuildTraining/Entities/SkillTarget")
            quest:AddEntityBinding("WillDummy", "GuildTraining/Entities/WillDummy")
            quest:AddEntityBinding("BirdKiller", "GuildTraining/Entities/BirdKiller")
            quest:AddEntityBinding("KillBird", "GuildTraining/Entities/KillBird")
            quest:AddEntityBinding("PreMeleeMaze", "GuildTraining/Entities/PreMeleeMaze")
            quest:FinalizeEntityBindings()
            quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(false)
            quest:SetWeaponOutCrimeEnabled(false)
            quest:SetGuardsIgnoreCrimes(true)
            quest:EnableGuards(quest:GetNearestWithDefName(quest:GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE"), false)
            quest:CreateThread("RunTutorials")  -- native thread body 0x00D45DD0: lift it as function RunTutorials(quest)
            quest:CreateThread("CheckFriendlyAttacks")  -- native thread body 0x00D45060: lift it as function CheckFriendlyAttacks(quest)
            quest:CreateThread("KeepTabsOnWhisper")  -- native thread body KeepTabsOnWhisper: lift it as function KeepTabsOnWhisper(quest)
            quest:CreateThread("WatchForSparrowKilled")  -- native thread body 0x00D3C840: lift it as function WatchForSparrowKilled(quest)
            quest:CreateThread("KeepBookcaseExitRemoved")  -- native thread body KeepBookcaseExitRemoved: lift it as function KeepBookcaseExitRemoved(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded2 = quest:IsLevelLoaded("HeroGuildComplex")
    end
end

-- Q_GuildTraining.Init (retail 0x00d3b3d0)
function Init(quest)
    quest:SetStateInt("WillHelpTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("GameState", 0)
    quest:SetStateInt("HeroWarnings", 0)
    quest:SetStateBool("DomeCutsceneStart", false)
    quest:SetStateInt("StartedTesting", 0)
    quest:SetStateBool("StartedMeleeTesting", false)
    quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 0)
    quest:SetStateBool("FightFinished", false)
    quest:SetStateBool("DisplayBirdKilledMessage", false)
    GossipSetup(quest)
end

-- Q_GuildTraining.OnPersist (retail 0x00d3bc10)
function OnPersist(quest, context)
end

-- Q_GuildTraining.RunTutorials (retail 0x00d45dd0)
function RunTutorials(quest)
    local resources = quest:RetailResources()
    local scratchValue, conversationId, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local meleeOpponent, preMeleeDummy, meleeApprentice, combatApprentice, skillApprentice
    local willApprentice, birdKiller, preMeleeDummy3, preMeleeWhisper, meleeApprentice3
    local combatApprentice3, skillApprentice3, willApprentice3, birdKiller3, hero6
    local combatApprentice5, meleeApprentice5, skillApprentice5, willApprentice5, birdKiller5
    local skillApprentice7, birdKiller7, meleeApprentice7, willApprentice7, meleeApprentice9
    local skillApprentice9, willApprentice9, scratchValue10, scratchValue11, scratchValue12, r13_2
    local theRealGuildmaster4, guildDoors, scratchValue13, scratchValue18, scratchValue19
    local scratchValue20, scratchValue21, scratchValue22, getAllThingsWithDefName, scratchValue23
    local timerId
    quest:SetRegionExitAsActive(quest:GetNearestWithDefName(quest:GetThingWithScriptName("SecretBookcase"), "REGION_EXIT_POINT"), false)
    quest:SetExperienceSpendingAsEnabled(false)
    quest:SetHeroSleepingAsEnabled(false)
    guildDoors = quest:GetThingWithScriptName("GuildDoors")
    quest:MiniMapAddMarker(quest:GetThingWithScriptName("HUD_ORB_QUEST_CORE"), "TheRealGuildmaster")
    scratchValue23 = nil
    if quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsQuestActive("Q_GuildTrainingDeparture") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsQuestActive("Q_GuildTrainingMelee") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsQuestActive("Q_GuildTrainingSkill") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:GetStateInt("GameState") == 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:SetThingAsUsable(guildDoors, false)
        quest:SetThingPersistent(nil --[[missing]], true)
        if quest:IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", quest:GetHero()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_GUILD_SEAL_1")
        end
        quest:SetStateInt("GameState", 1)
    end
    quest:SetTeleporterAsActive(quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER"), false)
    meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    if meleeOpponent ~= nil and meleeOpponent:IsAlive() then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:RemoveThing(quest:GetThingWithScriptName("MeleeOpponent"), false, true)
    end
    if quest:GetStateInt("GameState") == 1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:SetMasterGameState("ScorpionsDestroyed", false)
        quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", false)
        quest:SetMasterGameState("SkillTrainingStarted", false)
        preMeleeDummy = quest:GetThingWithScriptName("PreMeleeDummy")
        if not (preMeleeDummy ~= nil and preMeleeDummy:IsAlive()) then
            scratchValue13 = quest:CreateObject("PreMeleeDummy", nil --[[missing]], "PreMeleeDummyMarker")
            quest:EntitySetFacingAngle(quest:GetThingWithScriptName("PreMeleeDummyMarker"), quest:GetThingWithScriptName("PreMeleeDummy"):GetAngleXY(), true)
        end
        meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice ~= nil and meleeApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        combatApprentice = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice ~= nil and combatApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("CombatApprentice"), false, true)
        end
        skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice ~= nil and skillApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        willApprentice = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice ~= nil and willApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        birdKiller = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller ~= nil and birdKiller:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
        end
        if quest:IsObjectInThingsPossession("OBJECT_HERO_STICK", quest:GetHero()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_HERO_STICK")
        end
        quest:SetCategoryActivity("Sweet Kid", true)
        if not quest:IsQuestActive("Q_GuildTrainingPreMelee") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingPreMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingPreMelee", false)
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
            -- TODO(native): xStack_54._0_4_ = (int *)0x0;
            scratchValue23 = quest:GetAllThingsWithScriptName("AppleMarker")
            scratchValue3 = (scratchValue23._4_4_ - scratchValue23._0_4_) >> 31
            if (scratchValue23._4_4_ - scratchValue23._0_4_) / 12 + scratchValue3 ~= scratchValue3 then
                scratchValue4 = 0
                scratchValue19 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
                    quest:SetThingPersistent(quest:CreateObject((**(*(scratchValue23._0_4_ + scratchValue4) + 24))(), scratchValue19, ""), true)
                    quest:SetThingPersistent(scratchValue13, scratchValue23._0_4_ + scratchValue4)
                    quest:RemoveThing(nil --[[missing]], scratchValue23._0_4_ + scratchValue4, false)
                    scratchValue19 = scratchValue19 + 1
                    scratchValue4 = scratchValue4 + 12
                until scratchValue19 >= ((scratchValue23._4_4_ - scratchValue23._0_4_) / 12)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        end
        while quest:IsQuestActive("Q_GuildTrainingPreMelee") do
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        if quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        end
        quest:SetCategoryActivity("Sweet Kid", false)
        quest:SetStateInt("GameState", 3)
        quest:GiveHeroGold(50)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        preMeleeDummy3 = quest:GetThingWithScriptName("PreMeleeDummy")
        if preMeleeDummy3 ~= nil and preMeleeDummy3:IsAlive() then
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeDummy"), false, true)
        end
        preMeleeWhisper = quest:GetThingWithScriptName("PreMeleeWhisper")
        if preMeleeWhisper ~= nil and preMeleeWhisper:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
        end
    end
    if quest:GetStateInt("GameState") == 3 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", quest:GetHero()) then
            quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
        end
        if quest:IsObjectInThingsPossession("OBJECT_IRON_KATANA", quest:GetHero()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_KATANA")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        meleeApprentice3 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice3 ~= nil and meleeApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        combatApprentice3 = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice3 ~= nil and combatApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("CombatApprentice"), false, true)
        end
        skillApprentice3 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice3 ~= nil and skillApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice3 ~= nil and willApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        birdKiller3 = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller3 ~= nil and birdKiller3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
        end
        quest:SetCategoryActivity("Wannabe Hero", true)
        quest:SetCategoryActivity("Young Apprentice", true)
        quest:SetCategoryActivity("Young Annoyance", true)
        quest:TurnCreatureInto(quest:GetHero(), "CREATURE_HERO")
        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
        quest:GiveHeroExpression("EXPRESSION_FLIRT", -1, true)
        quest:GiveHeroExpression("EXPRESSION_COSSACK", -1, true)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1)
        end
        hero6 = quest:GetHero()
        -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
        quest:EntitySetAsOpinionSource(hero6, "OPINION_SOURCE_HERO_AS_APPRENTICE")
        quest:SetHeroAsTeenager(true)
        quest:SetHeroAsApprentice(true)
        quest:GiveHeroAbility(11, false)
        if not quest:IsQuestActive("Q_GuildTrainingMelee") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingMelee", false)
        end
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
        while quest:IsQuestActive("Q_GuildTrainingMelee") do
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
        end
        quest:RemoveQuestCardFromGuild("Q_GuildTraining")
        quest:SetCategoryActivity("Complete Melee", true)
        quest:SetStateInt("GameState", 5)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        combatApprentice5 = quest:GetThingWithScriptName("CombatApprentice")
        if not (combatApprentice5 ~= nil and combatApprentice5:IsAlive()) then
            quest:CreateCreature("CombatApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "CombatApprenticeMarker")
            quest:GetThingWithScriptName("CombatApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    if quest:GetStateInt("GameState") == 5 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", quest:GetHero()) then
            quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
        end
        if quest:IsObjectInThingsPossession("OBJECT_YEW_CROSSBOW", quest:GetHero()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_CROSSBOW")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        quest:SetMasterGameState("MovingDummiesNeeded", false)
        meleeApprentice5 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice5 ~= nil and meleeApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        quest:CreateCreature("MeleeApprentice", quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"):GetPos(), "M_MeleeOpponentStand")
        skillApprentice5 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice5 ~= nil and skillApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        willApprentice5 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice5 ~= nil and willApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        birdKiller5 = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller5 ~= nil and birdKiller5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
        end
        if not quest:IsQuestActive("Q_GuildTrainingSkill") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingSkill")
            quest:SetQuestAsPersistent("Q_GuildTrainingSkill", false)
        end
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
        while quest:IsQuestActive("Q_GuildTrainingSkill") do
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
        end
        quest:SetCategoryActivity("Complete Skill", true)
        quest:SetStateInt("GameState", 7)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("MovingDummiesNeeded", true)
        skillApprentice7 = quest:GetThingWithScriptName("SkillApprentice")
        if not (skillApprentice7 ~= nil and skillApprentice7:IsAlive()) then
            scratchValue18 = quest:CreateCreature("SkillApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "SkillApprenticeMarker")
            quest:GetThingWithScriptName("SkillApprentice"):SetToKillOnLevelUnload(false)
        end
        birdKiller7 = quest:GetThingWithScriptName("BirdKiller")
        if not (birdKiller7 ~= nil and birdKiller7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            scratchValue10 = quest:CreateCreature("BirdKiller", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "BirdKillerMarker")
        end
    end
    if quest:GetStateInt("GameState") == 7 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:GiveHeroAbility(11, false)
        meleeApprentice7 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice7 ~= nil and meleeApprentice7:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        scratchValue11 = quest:CreateCreature("MeleeApprentice", quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"):GetPos(), "M_MeleeOpponentStand")
        if not quest:IsQuestActive("Q_GuildTrainingWill") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingWill")
        end
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
        while quest:IsQuestActive("Q_GuildTrainingWill") do
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
        end
        quest:SetCategoryActivity("Complete Will", true)
        quest:SetStateInt("GameState", 9)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        willApprentice7 = quest:GetThingWithScriptName("WillApprentice")
        if not (willApprentice7 ~= nil and willApprentice7:IsAlive()) then
            scratchValue12 = quest:CreateCreature("WillApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "WillApprenticeMarker")
            quest:GetThingWithScriptName("WillApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    meleeApprentice9 = quest:GetThingWithScriptName("MeleeApprentice")
    if meleeApprentice9 ~= nil and meleeApprentice9:IsAlive() then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
    end
    quest:CreateCreature("MeleeApprentice", quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"):GetPos(), "MeleeApprenticeMarker")
    if not quest:IsQuestActive("Q_GuildTrainingDeparture") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:ActivateQuest("Q_GuildTrainingDeparture")
        quest:SetQuestAsPersistent("Q_GuildTrainingDeparture", false)
    end
    if not quest:NewScriptFrame() then goto LAB_00d496bc end
    while quest:IsQuestActive("Q_GuildTrainingDeparture") do
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_13", "HeroGuildComplexInside", "")
    scratchValue = quest:IsLevelLoaded("HeroGuildComplex")
    while not scratchValue do
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
        scratchValue = quest:IsLevelLoaded("HeroGuildComplex")
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
    theRealGuildmaster4 = quest:GetThingWithScriptName("TheRealGuildmaster")
    scratchValue22 = resources:NewResource()
    scratchValue = resources:TryAcquire(scratchValue22, scratchValue23[0 + 1][0 + 1], 4)
    while not scratchValue do
        if not quest:NewScriptFrame() then goto LAB_00d48800 end
        scratchValue = resources:TryAcquire(scratchValue22, scratchValue23[0 + 1][0 + 1], 4)
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue21 = resources:NewResource()
        scratchValue = resources:TryAcquire(scratchValue21, quest:GetHero(), 4)
        while not scratchValue do
            if not quest:NewScriptFrame() then goto LAB_00d487f4 end
            scratchValue = resources:TryAcquire(scratchValue21, quest:GetHero(), 4)
        end
        if not quest:IsActiveThreadTerminating() then
            r13_2 = resources:NewActorMap()
            resources:SetActor(r13_2, "HERO", scratchValue21)
            resources:SetActor(r13_2, "GM", scratchValue22)
            scratchValue20 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_DEPARTURE_EXIT_WOODS", r13_2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue20)
            resources:DestroyActorMap(r13_2)
            resources:ReleaseResource(scratchValue21)
            resources:DestroyMovie(scratchValue22)
            quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("FrescoDomeHSP"), false)
            quest:SetStateBool("DomeCutsceneStart", true)
            RunCeremonyCutscene(quest)
            quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", -1)
            quest:SetStateBool("DomeCutsceneStart", false)
            quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("HeroGuildComplexInsideHSP"), false)
            scratchValue = quest:IsLevelLoaded("HeroGuildComplex")
            while not scratchValue do
                if not quest:NewScriptFrame() then goto LAB_00d496bc end
                scratchValue = quest:IsLevelLoaded("HeroGuildComplex")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:FadeScreenOut(0.5, 0.0)
            quest:OpenDoor(theRealGuildmaster4)
            quest:SetThingPersistent(scratchValue12, true)
            quest:SetRegionExitAsActive(scratchValue11, true)
            quest:GiveHeroExperience(math.modf(quest:ReadGlobalGameDataFloat(3868)))
            quest:GiveHeroObject("OBJECT_HERO_BOOTS", -1)
            quest:GiveHeroObject("OBJECT_HERO_TROUSERS", -1)
            quest:GiveHeroObject("OBJECT_HERO_SHIRT", -1)
            quest:GiveHeroObject("OBJECT_HERO_GLOVES", -1)
            if not quest:NewScriptFrame() then goto LAB_00d496bc end
            quest:SetHeroAsWearing("OBJECT_HERO_BOOTS")
            quest:SetHeroAsWearing("OBJECT_HERO_TROUSERS")
            quest:SetHeroAsWearing("OBJECT_HERO_SHIRT")
            quest:SetHeroAsWearing("OBJECT_HERO_GLOVES")
            skillApprentice9 = quest:GetThingWithScriptName("SkillApprentice")
            if skillApprentice9 ~= nil and skillApprentice9:IsAlive() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
                quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
            end
            willApprentice9 = quest:GetThingWithScriptName("WillApprentice")
            if willApprentice9 ~= nil and willApprentice9:IsAlive() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
                quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
            end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
            quest:RemoveThing(quest:GetThingWithScriptName("CombatApprentice"), false, true)
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
            -- TODO(native): NScript::CQ_GuildTrainingScript::RunSaveXPCutscene2__atd496f0(this);
            quest:SetIsPushableByHero(quest:GetThingWithScriptName("TheRealGuildmaster"), false)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_14", "HeroGuildComplexInside", "")
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 10)
            scratchValue = quest:MsgOnLeavingExperienceSpendingScreen()
            while not scratchValue do
                if not quest:NewScriptFrame() then goto LAB_00d496b3 end
                if quest:GetTimer(timerId) == 0 then
                    conversationId = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                    quest:AddLineToConversation(conversationId, "TheRealGuildmaster", quest:GetThingWithScriptName("TEXT_CS_028_LEAVING_TOUR_45"), quest:GetHero(), false)
                    quest:SetTimer(timerId, 10)
                end
                scratchValue = quest:MsgOnLeavingExperienceSpendingScreen()
            end
            if not quest:IsActiveThreadTerminating() then
                -- TODO(native): NScript::CQ_GuildTrainingScript::RunSaveXPCutscene2__atd49a20(this);
                quest:FadeScreenOut(0.0, 0.5)
                quest:EntityTeleportToThing(quest:GetThingWithScriptName("M_GuildmasterMarker"), quest:GetThingWithScriptName("TheRealGuildmaster"), false)
                scratchValue = quest:IsHeroControlledByPlayer()
                while not scratchValue do
                    if not quest:NewScriptFrame() then goto LAB_00d496b3 end
                    scratchValue = quest:IsHeroControlledByPlayer()
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:EntityUnsetAsOpinionSource(quest:GetHero(), false)
                    quest:SetHeroAsApprentice(false)
                    quest:GiveHeroExpression("EXPRESSION_FOLLOW", -1, true)
                    quest:GiveHeroExpression("EXPRESSION_WAIT", -1, true)
                    quest:SetThingAsUsable(scratchValue10, true)
                    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)
                    quest:RemoveThing(quest:GetThingWithScriptName("TheRealGuildmaster"), false, true)
                    quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeMaze"), false, true)
                    quest:NewScriptFrame()
                    if not quest:IsActiveThreadTerminating() then
                        quest:NewScriptFrame()
                        if not quest:IsActiveThreadTerminating() then
                            quest:NewScriptFrame()
                            if not quest:IsActiveThreadTerminating() then
                                quest:NewScriptFrame()
                                if not quest:IsActiveThreadTerminating() then
                                    quest:NewScriptFrame()
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:FadeScreenIn()
                                        quest:SetTimeOfDay(10.0)
                                        quest:SetWeaponOutCrimeEnabled(true)
                                        quest:SetGuardsIgnoreCrimes(false)
                                        quest:EnableGuards(quest:GetNearestWithDefName(quest:GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE"), true)
                                        quest:SetHeroSleepingAsEnabled(true)
                                        while quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", quest:GetHero()) do
                                            if not quest:NewScriptFrame() then goto LAB_00d496aa end
                                            quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            getAllThingsWithDefName = quest:GetAllThingsWithDefName("OBJECT_APPLE_RED_01")
                                            scratchValue5 = 0 - getAllThingsWithDefName >> 31
                                            scratchValue19 = 0
                                            if (0 - getAllThingsWithDefName) / 12 + scratchValue5 ~= scratchValue5 then
                                                scratchValue6 = 0
                                                repeat
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d496a1 end
                                                    quest:RemoveThing(scratchValue18, getAllThingsWithDefName + scratchValue6, false)
                                                    scratchValue19 = scratchValue19 + 1
                                                    scratchValue6 = scratchValue6 + 12
                                                until scratchValue19 >= ((0 - getAllThingsWithDefName) / 12)
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
                                                quest:NewScriptFrame()
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:NewScriptFrame()
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:NewScriptFrame()
                                                        if not quest:IsActiveThreadTerminating() then
                                                            quest:NewScriptFrame()
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:NewScriptFrame()
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_THEGUILD")
                                                                    if quest:DisplayTutorial(30) then
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            scratchValue = quest:MsgIsTutorialClickedPast()
                                                                            while not scratchValue do
                                                                                if not quest:NewScriptFrame() then goto LAB_00d496a1 end
                                                                                scratchValue = quest:MsgIsTutorialClickedPast()
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); goto FLOW_after_lab_00d4967d end
                                                                        end
                                                                    else
                                                                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                                                                    end
                                                                    ::FLOW_after_lab_00d4967d::
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                            ::LAB_00d496a1::
                                        end
                                        ::LAB_00d496aa::
                                    end
                                end
                            end
                        end
                    end
                end
            end
            ::LAB_00d496b3::
            quest:DeregisterTimer(timerId)
            goto LAB_00d496bc
        end
        ::LAB_00d487f4::
        resources:ReleaseResource(scratchValue21)
    end
    ::LAB_00d48800::
    resources:ReleaseResource(scratchValue22)
    ::LAB_00d496bc::
end

-- Q_GuildTraining.CheckFriendlyAttacks (retail 0x00d45060)
function CheckFriendlyAttacks(quest)
    local resources = quest:RetailResources()
    local scratchValue, predicateResult5, predicateResult6, predicateResult19, scratchValue18
    local conversationId, scratchValue19, scratchValue20, scratchValue21, heroWarnings, hero, hero2
    local hero3, scratchValue28, scratchValue29, scratchValue30, scratchValue31, r1_1
    local scratchValue40, scratchValue41, scratchValue42, scratchValue43, conversationId2
    local scratchValue44, scratchValue45, scratchValue46
    r1_1 = quest:GetThingWithScriptName(nil --[[missing]])
    scratchValue19 = r1_1 - 0 >> 31
    scratchValue40 = 0
    if (r1_1 - 0) / 12 + scratchValue19 ~= scratchValue19 then
        scratchValue20 = 0
        repeat
            scratchValue30 = r1_1
            scratchValue28 = 0
            if quest:IsActiveThreadTerminating() then goto LAB_00d452d1 end
            if CCharString__NotEqual((**(*(0 + scratchValue20) + 8))(&xStack_4c),"CREATURE_BIRD_GUILD_SPARROW") == 0 then
                predicateResult19 = false
            else
                if CCharString__NotEqual((**(*(0 + scratchValue20) + 8))(&xStack_38),"CREATURE_RIVAL_HERO_WHISPER_APPRENTICE") == 0 then
                    predicateResult19 = false
                    goto FLOW_after_lab_00d45184
                end
                if CCharString__NotEqual((**(*(0 + scratchValue20) + 8))(&xStack_48),"CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE") == 0 then
                    predicateResult19 = false
                    goto FLOW_after_lab_00d45184
                end
                predicateResult19 = true
                if CCharString__NotEqual((**(*(0 + scratchValue20) + 8))(&xStack_60),"CREATURE_RIVAL_HERO_MAZE") == 0 then
                    predicateResult19 = false
                    goto FLOW_after_lab_00d45184
                end
            end
            ::FLOW_after_lab_00d45184::
            if predicateResult19 then
                if quest:IsActiveThreadTerminating() then return end
                -- TODO(native): (**(code **)(*(int *)((int)xStack_84 + iVar18) + 0x10c))(1);
                quest:EntitySetAsKillable(r1_1, 0 + scratchValue20, false)
            end
            scratchValue40 = scratchValue40 + 1
            scratchValue20 = scratchValue20 + 12
        until scratchValue40 >= ((r1_1 - 0) / 12)
    end
    scratchValue31 = r1_1
    scratchValue29 = 0
    if not quest:IsActiveThreadTerminating() then
        while scratchValue29 ~= scratchValue31 do
            -- TODO(native): (**(code **)*puVar7)(0);
            scratchValue29 = scratchValue29 + 3
        end
        if nil ~= nil then
            -- TODO(native): free(xStack_84);
        end
        repeat
            if quest:IsActiveThreadTerminating() then return end
            if not quest:IsLevelLoaded("HeroGuildComplex") then
                while not quest:IsLevelLoaded("HeroGuildComplex") do
                    if not quest:NewScriptFrame() then goto LAB_00d45322 end
                end
                scratchValue18 = 0
                scratchValue21 = r1_1 - 0 >> 31
                scratchValue41 = 0
                if (r1_1 - 0) / 12 + scratchValue21 ~= scratchValue21 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if CCharString__NotEqual((**(*(scratchValue18 + 0) + 8))(scratchValue42),"CREATURE_BIRD_GUILD_SPARROW") == 0 then
                            predicateResult5 = false
                        else
                            if CCharString__NotEqual((**(*(scratchValue18 + 0) + 8))(conversationId2),"CREATURE_RIVAL_HERO_WHISPER_APPRENTICE") == 0 then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                            if CCharString__NotEqual((**(*(0 + scratchValue18) + 8))(conversationId2),"CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE") == 0 then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                            predicateResult5 = true
                            if CCharString__NotEqual((**(*(0 + scratchValue18) + 8))(conversationId2),"CREATURE_RIVAL_HERO_MAZE") == 0 then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                        end
                        ::FLOW_after_lab_00d4557d::
                        if predicateResult5 then
                            if quest:IsActiveThreadTerminating() then return end
                            -- TODO(native): (**(code **)(*(int *)((int)xStack_84 + iVar13) + 0x10c))(1);
                            quest:EntitySetAsKillable(nil --[[missing]], 0 + scratchValue18, false)
                        end
                        scratchValue41 = scratchValue41 + 1
                        scratchValue18 = scratchValue18 + 12
                    until scratchValue41 >= ((r1_1 - 0) / 12)
                end
                if quest:IsActiveThreadTerminating() then return end
            end
            hero = quest:GetHero()
            -- TODO(native): MsgHitFriendWithBareHands is not a ForgeFSE binding
            if hero:MsgHitFriendWithBareHands() then
                if not quest:IsLevelLoaded("HeroGuildComplex") then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d45782
                end
                if scratchValue46 ~= nil and (**(*scratchValue46 + 84))("SCRIPT_NAME_HERO") ~= 0 then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d45782
                end
                if scratchValue46 ~= nil and (**(*scratchValue46 + 168))("SCRIPT_NAME_HERO") ~= 0 then
                    if scratchValue46 == nil or (**(*scratchValue46 + 164))(14,"SCRIPT_NAME_HERO") == 0 then
                        predicateResult6 = false
                        goto FLOW_after_lab_00d45782
                    end
                end
                predicateResult6 = true
            else
                hero2 = quest:GetHero()
                -- TODO(native): MsgHitFriendWithMeleeWeapon is not a ForgeFSE binding
                if hero2:MsgHitFriendWithMeleeWeapon() then
                    if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
                    if scratchValue46 ~= nil and (**(*scratchValue46 + 84))("SCRIPT_NAME_HERO") ~= 0 then goto LAB_00d45782 end
                    if scratchValue46 ~= nil and (**(*scratchValue46 + 168))("SCRIPT_NAME_HERO") ~= 0 then
                        if scratchValue46 == nil or (**(*scratchValue46 + 164))(14,"SCRIPT_NAME_HERO") == 0 then goto LAB_00d45782 end
                    end
                    predicateResult6 = true
                    goto FLOW_after_lab_00d45782
                end
                hero3 = quest:GetHero()
                -- TODO(native): MsgHitFriendWithRangedWeapon is not a ForgeFSE binding
                if hero3:MsgHitFriendWithRangedWeapon() then
                    if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
                    if scratchValue46 ~= nil and (**(*scratchValue46 + 84))("SCRIPT_NAME_HERO") ~= 0 then goto LAB_00d45782 end
                    if scratchValue46 ~= nil and (**(*scratchValue46 + 168))("SCRIPT_NAME_HERO") ~= 0 then
                        if scratchValue46 == nil or (**(*scratchValue46 + 164))(14,"SCRIPT_NAME_HERO") == 0 then goto LAB_00d45782 end
                    end
                    predicateResult6 = true
                    goto FLOW_after_lab_00d45782
                end
                ::LAB_00d45782::
                predicateResult6 = false
            end
            ::FLOW_after_lab_00d45782::
            if not predicateResult6 then goto LAB_00d45cae end
            if quest:IsActiveThreadTerminating() then return end
            if 2 < quest:GetStateInt("HeroWarnings") then
                quest:SetMasterGameState("GuildWarningOccuring", true)
                if quest:GetMasterGameState("SkillTestOccuring") == 0 and quest:GetMasterGameState("WillTestOccuring") == 0 then
                    scratchValue44 = resources:NewResource()
                    while not resources:TryAcquire(scratchValue44, scratchValue45, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d45db2 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue43 = resources:NewResource()
                        while not resources:TryAcquire(scratchValue43, quest:GetHero(), 4) do
                            if not quest:NewScriptFrame() then goto LAB_00d45da9 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue42 = resources:NewActorMap()
                            resources:SetActor(amStack_1c, "HERO", &scratchValue42)
                            resources:SetActor(amStack_1c, "MAZE", scratchValue44)
                            scratchValue = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            conversationId2 = quest:AddNewConversation(quest:GetHero(), false, false)
                            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", quest:GetHero(), quest:GetHero(), false)
                            quest:Pause(2.0)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro(xStack_58, amStack_1c, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue)
                            resources:DestroyActorMap(amStack_1c)
                            resources:ReleaseResource(amStack_1c)
                            resources:ReleaseResource(conversationId2)
                            goto LAB_00d45c9d
                        end
                        ::LAB_00d45da9::
                        resources:ReleaseResource(scratchValue43)
                    end
                    ::LAB_00d45db2::
                    resources:ReleaseResource(scratchValue44)
                    return
                end
                while quest:GetMasterGameState("SkillTestOccuring") ~= 0 or quest:GetMasterGameState("WillTestOccuring") ~= 0 do
                    if not quest:NewScriptFrame() then return end
                end
                ::LAB_00d45c9d::
                quest:SetStateInt("HeroWarnings", 0)
                quest:SetMasterGameState("GuildWarningOccuring", false)
                goto LAB_00d45cae
            end
            conversationId = quest:AddNewConversation(quest:GetHero(), false, false)
            heroWarnings = quest:GetStateInt("HeroWarnings")
            if heroWarnings == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", quest:GetHero(), quest:GetHero(), false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            elseif heroWarnings == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", quest:GetHero(), quest:GetHero(), false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            elseif heroWarnings ~= 2 then
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", quest:GetHero(), quest:GetHero(), false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            ::LAB_00d45cae::
            quest:NewScriptFrame()
        until false
    end
    while scratchValue29 ~= scratchValue31 do
        -- TODO(native): (**(code **)*puVar7)(0);
        scratchValue29 = scratchValue29 + 3
    end
    if nil ~= nil then
        -- TODO(native): free(xStack_84);
    end
    ::LAB_00d45322::
    do return end
    ::LAB_00d452d1::
    while scratchValue28 ~= scratchValue30 do
        -- TODO(native): (**(code **)*puVar7)(0);
        scratchValue28 = scratchValue28 + 3
    end
    if nil ~= nil then
        -- TODO(native): free(xStack_84);
    end
    goto LAB_00d45322
end

-- Q_GuildTraining.KeepTabsOnWhisper (retail 0x00d3cbd0)
function KeepTabsOnWhisper(quest)
    local scratchValue, scratchValue2, predicateResult, meleeApprentice
    if quest:IsActiveThreadTerminating() then return end
    scratchValue = 0
    repeat
        scratchValue2 = scratchValue | 1
        if quest:IsLevelLoaded("GuildWoods") then
            scratchValue2 = scratchValue | 3
            predicateResult = true
            if not quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                predicateResult = false
                goto FLOW_after_lab_00d3cc48
            end
        else
            predicateResult = false
        end
        ::FLOW_after_lab_00d3cc48::
        if scratchValue2 & true then
            scratchValue2 = scratchValue2 & 0xfffffffd
        end
        if scratchValue2 & true then
            scratchValue2 = scratchValue2 & 0xfffffffe
        end
        scratchValue = scratchValue2
        if predicateResult then
            while not quest:IsLevelLoaded("HeroGuildComplex") do
                if not quest:NewScriptFrame() then return end
            end
            scratchValue = scratchValue2
            if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                scratchValue = scratchValue2
                if meleeApprentice ~= nil and meleeApprentice:IsAlive() then
                    quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                    while quest:IsQuestActive("Q_GuildTrainingWoodsWill") do
                        if not quest:NewScriptFrame() then return end
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    if not quest:NewScriptFrame() then return end
                    if not quest:NewScriptFrame() then return end
                    if not quest:NewScriptFrame() then return end
                    while not quest:IsLevelLoaded("HeroGuildComplex") do
                        if not quest:NewScriptFrame() then return end
                    end
                    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos(), "MeleeApprentice")
                    return
                end
            end
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- Q_GuildTraining.WatchForSparrowKilled (retail 0x00d3c840)
function WatchForSparrowKilled(quest)
    while not quest:GetStateBool("DisplayBirdKilledMessage") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DisplayGameInfo("TEXT_QST_028_HERO_KILL_BIRD")
    while not quest:MsgIsGameInfoClickedPast() do
        if not quest:NewScriptFrame() then return end
    end
end

-- Q_GuildTraining.KeepBookcaseExitRemoved (retail 0x00d3c8f0)
function KeepBookcaseExitRemoved(quest)
    local r2_1 = quest:GetNearestWithDefName(quest:GetThingWithScriptName("SecretBookcase"), "REGION_EXIT_POINT")
    while quest:GetStateInt("GameState") ~= 9 do
        if not quest:NewScriptFrame() then goto LAB_00d3cac6 end
        while quest:IsLevelLoaded("HeroGuildComplex") do
            if not quest:NewScriptFrame() then return end
        end
        while not quest:IsLevelLoaded("HeroGuildComplex") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:GetStateInt("GameState") == 9 then break end
        quest:SetRegionExitAsActive(r2_1, false)
    end
    ::LAB_00d3cac6::
end

-- Q_GuildTraining.RunArrivalCutscene (retail 0x00d44cb0)
function RunArrivalCutscene(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5
    quest:SetTimeOfDay(19.0)
    if not quest:NewScriptFrame() then return end
    scratchValue2 = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", quest:GetThingWithScriptName("MK_GTA_MAZE1"):GetPos(), "CutsceneMaze")
    scratchValue5 = resources:NewResource()
    while not resources:TryAcquire(scratchValue5, scratchValue2, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d44ec4 end
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue4 = resources:NewResource()
        while not resources:TryAcquire(scratchValue4, quest:GetHero(), 4) do
            if not quest:NewScriptFrame() then goto LAB_00d44ebb end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue = resources:NewActorMap()
            resources:SetActor(scratchValue, "MAZE", scratchValue5)
            resources:SetActor(scratchValue, "HERO", scratchValue4)
            scratchValue3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_ARRIVE", scratchValue, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:FadeScreenOut(0.5, 0.0)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:SetRegionTextDisplayAsActive(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue3)
            resources:DestroyActorMap(scratchValue)
            resources:ReleaseResource(scratchValue4)
            resources:DestroyMovie(scratchValue5)
            quest:RemoveThing(scratchValue2, false, true)
            return
        end
        ::LAB_00d44ebb::
        resources:ReleaseResource(scratchValue4)
    end
    ::LAB_00d44ec4::
    resources:ReleaseResource(scratchValue5)
end

-- Q_GuildTraining.GossipSetup (retail 0x00d3b410)
function GossipSetup(quest)
    quest:AddRumourCategory("Sweet Kid")
    quest:AddNewRumourToCategory("Sweet Kid", "TEXT_AI_GOSSIP_SWEET_KID")
    quest:AddGossipVillage("Sweet Kid", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_SERVANTS")
    quest:AddGossipFactionToCategory("Sweet Kid", "FACTION_VILLAGERS")
    quest:AddGossipFactionToCategory("Sweet Kid", "FACTION_GUILD_APPRENTICES_GOOD")
    quest:AddNewRumourToCategory("Young Apprentice", "TEXT_AI_GOSSIP_YOUNG_APPRENTICE")
    quest:AddGossipVillage("Young Apprentice", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Young Apprentice", "FACTION_GUILD_SERVANTS")
    quest:AddNewRumourToCategory("Young Annoyance", "TEXT_AI_GOSSIP_YOUNG_ANNOYANCE")
    quest:AddGossipVillage("Young Annoyance", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Young Annoyance", "FACTION_VILLAGERS")
    quest:AddRumourCategory("Wannabe Hero")
    quest:AddNewRumourToCategory("Wannabe Hero", "TEXT_AI_GOSSIP_WANNABE_HERO")
    quest:AddGossipVillage("Wannabe Hero", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Wannabe Hero", "FACTION_GUILD_APPRENTICES_GOOD")
    quest:AddRumourCategory("Complete Melee")
    quest:AddNewRumourToCategory("Complete Melee", "TEXT_AI_GOSSIP_MELEE_COMPLETE")
    quest:AddGossipVillage("Complete Melee", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Complete Melee", "FACTION_GUILD_APPRENTICES_GOOD")
    quest:AddRumourCategory("Complete Skill")
    quest:AddNewRumourToCategory("Complete Skill", "TEXT_AI_GOSSIP_SKILL_COMPLETE")
    quest:AddGossipVillage("Complete Skill", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Complete Skill", "FACTION_GUILD_APPRENTICES_GOOD")
    quest:AddRumourCategory("Complete Will")
    quest:AddNewRumourToCategory("Complete Will", "TEXT_AI_GOSSIP_WILL_COMPLETE")
    quest:AddGossipVillage("Complete Will", "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:AddGossipFactionToCategory("Complete Will", "FACTION_GUILD_APPRENTICES_GOOD")
end

-- Q_GuildTraining.RunCeremonyCutscene (retail 0x00d49d50)
function RunCeremonyCutscene(quest)
    local resources = quest:RetailResources()
    local predicateResult, hero, scratchValue4, scratchValue5, scratchValue6, scratchValue7
    local scratchValue8, scratchValue9, scratchValue10
    while not quest:IsLevelLoaded("FrescoDome") do
        if not quest:NewScriptFrame() then return end
    end
    predicateResult = quest:IsActiveThreadTerminating()
    CONCAT31(extraout_var_00,predicateResult)
    if not predicateResult then
        scratchValue4 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MK_GTC_WHISSTART"):GetPos(), "Whisper")
        scratchValue5 = quest:CreateCreature("CREATURE_GUILDKEEPER", quest:GetThingWithScriptName("MK_GTC_GMSTART"):GetPos(), "GM")
        scratchValue9 = resources:NewResource()
        while not resources:TryAcquire(scratchValue9, scratchValue4, 4) do
            if not quest:NewScriptFrame() then goto LAB_00d4a246 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:EntitySetInFaction(scratchValue5, "FACTION_HERO")
            scratchValue8 = resources:NewResource()
            while not resources:TryAcquire(scratchValue8, scratchValue5, 4) do
                if not quest:NewScriptFrame() then goto LAB_00d4a23d end
            end
            if not quest:IsActiveThreadTerminating() then
                scratchValue7 = resources:NewResource()
                hero = quest:GetHero()
                while not resources:TryAcquire(scratchValue7, hero, 4) do
                    if not quest:NewScriptFrame() then goto LAB_00d4a234 end
                    hero = quest:GetHero()
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue10 = resources:NewActorMap()
                    resources:SetActor(scratchValue10, "HERO", scratchValue7)
                    resources:SetActor(scratchValue10, "WHIS", scratchValue9)
                    resources:SetActor(scratchValue10, "GM", scratchValue8)
                    scratchValue6 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_CEREMONY", scratchValue10, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:RemoveThing(hero, false, true)
                    quest:RemoveThing(scratchValue4, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue6)
                    resources:DestroyActorMap(scratchValue10)
                end
                ::LAB_00d4a234::
                resources:ReleaseResource(scratchValue7)
            end
            ::LAB_00d4a23d::
            resources:ReleaseResource(scratchValue8)
        end
        ::LAB_00d4a246::
        resources:ReleaseResource(scratchValue9)
    end
end

-- Q_GuildTraining.RunSaveXPCutscene (retail 0x00d496f0)
function RunSaveXPCutscene(quest)
    local resources = quest:RetailResources()
    local theRealGuildmaster, scratchValue, scratchValue2, scratchValue3, scratchValue4
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    scratchValue3 = resources:NewResource()
    while not resources:TryAcquire(scratchValue3, theRealGuildmaster, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(scratchValue3); return end
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue2 = resources:NewResource()
        while not resources:TryAcquire(scratchValue2, quest:GetHero(), 4) do
            if not quest:NewScriptFrame() then goto LAB_00d4997a end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = resources:NewActorMap()
            resources:SetActor(scratchValue4, "GM", scratchValue3)
            resources:SetActor(scratchValue4, "HERO", scratchValue2)
            scratchValue = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SAVEXP", scratchValue4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue)
            resources:DestroyActorMap(scratchValue4)
        end
        ::LAB_00d4997a::
        resources:ReleaseResource(scratchValue2)
    end
    resources:ReleaseResource(scratchValue3)
end

-- Q_GuildTraining.RunSaveXPCutscene2 (retail 0x00d49a20)
function RunSaveXPCutscene2(quest)
    local resources = quest:RetailResources()
    local theRealGuildmaster, scratchValue, scratchValue2, scratchValue3, scratchValue4
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    scratchValue3 = resources:NewResource()
    while not resources:TryAcquire(scratchValue3, theRealGuildmaster, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(scratchValue3); return end
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue2 = resources:NewResource()
        while not resources:TryAcquire(scratchValue2, quest:GetHero(), 4) do
            if not quest:NewScriptFrame() then goto LAB_00d49caa end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = resources:NewActorMap()
            resources:SetActor(scratchValue4, "GM", scratchValue3)
            resources:SetActor(scratchValue4, "HERO", scratchValue2)
            scratchValue = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SAVEXP2", scratchValue4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue)
            resources:DestroyActorMap(scratchValue4)
        end
        ::LAB_00d49caa::
        resources:ReleaseResource(scratchValue2)
    end
    resources:ReleaseResource(scratchValue3)
end

