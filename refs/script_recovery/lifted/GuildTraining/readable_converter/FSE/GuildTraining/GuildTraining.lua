-- Readable native conversion: Q_GuildTraining. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local TUTORIAL_CATEGORY_TAKING_QUESTS = 30  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_EndGuildXP = 3868,  -- 600.0
}

-- Q_GuildTraining.Main (retail 0x00d3bc60)
function Main(quest)
    local isLevelLoaded
    local hero = quest:GetHero()
    if quest:GetStateInt("GameState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveAllHeroWeapons()
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("GuildArrivalHSP"), false)
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
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("GuildTrainingHSP"), false)
    isLevelLoaded = quest:IsLevelLoaded("HeroGuildComplex")
    while true do
        if isLevelLoaded then
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
            quest:EnableGuards(quest:GetNearestWithDefName(hero, "VILLAGE_GUILD_COMPLEX_INSIDE"), false)
            quest:CreateThread("RunTutorials")  -- native thread body 0x00D45DD0: lift it as function RunTutorials(quest)
            quest:CreateThread("CheckFriendlyAttacks")  -- native thread body 0x00D45060: lift it as function CheckFriendlyAttacks(quest)
            quest:CreateThread("KeepTabsOnWhisper")  -- native thread body KeepTabsOnWhisper: lift it as function KeepTabsOnWhisper(quest)
            quest:CreateThread("WatchForSparrowKilled")  -- native thread body 0x00D3C840: lift it as function WatchForSparrowKilled(quest)
            quest:CreateThread("KeepBookcaseExitRemoved")  -- native thread body KeepBookcaseExitRemoved: lift it as function KeepBookcaseExitRemoved(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded = quest:IsLevelLoaded("HeroGuildComplex")
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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue5, meleeOpponent, meleeApprentice9, skillApprentice9
    local willApprentice9, actorMap, theRealGuildmaster, scratchValue6, scratchValue9, scratchValue
    local movie, resource, resource4, appleMarker, timerId
    local secretBookcase = quest:GetThingWithScriptName("SecretBookcase")
    local getNearestWithDefName = quest:GetNearestWithDefName(secretBookcase, "REGION_EXIT_POINT")
    quest:SetRegionExitAsActive(getNearestWithDefName, false)
    quest:SetExperienceSpendingAsEnabled(false)
    quest:SetHeroSleepingAsEnabled(false)
    local guildDoors = quest:GetThingWithScriptName("GuildDoors")
    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
    appleMarker = nil
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
        quest:SetThingPersistent(guildDoors, true)
        if quest:IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", hero) then
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
        local preMeleeDummy = quest:GetThingWithScriptName("PreMeleeDummy")
        if not (preMeleeDummy ~= nil and preMeleeDummy:IsAlive()) then
            scratchValue6 = quest:CreateObject("PreMeleeDummy", nil --[[missing]], "PreMeleeDummyMarker")
            quest:EntitySetFacingAngle(quest:GetThingWithScriptName("PreMeleeDummyMarker"), quest:GetThingWithScriptName("PreMeleeDummy"):GetAngleXY(), true)
        end
        local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice ~= nil and meleeApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        local combatApprentice = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice ~= nil and combatApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("CombatApprentice"), false, true)
        end
        local skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice ~= nil and skillApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        local willApprentice = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice ~= nil and willApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        local birdKiller = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller ~= nil and birdKiller:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
        end
        if quest:IsObjectInThingsPossession("OBJECT_HERO_STICK", hero) then
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
            appleMarker = quest:GetAllThingsWithScriptName("AppleMarker")
            local scratchValue3 = (appleMarker._4_4_ - appleMarker._0_4_) >> 31
            if (appleMarker._4_4_ - appleMarker._0_4_) / 12 + scratchValue3 ~= scratchValue3 then
                scratchValue4 = 0
                scratchValue9 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
                    -- TODO(native): pCVar4 = (**(*(xStack_54._0_4_ + iVar9) + 0x18))()
    --[[unresolved native value]]
                    quest:SetThingPersistent(quest:CreateObject(nil, scratchValue9, ""), true)
                    quest:SetThingPersistent(scratchValue6, appleMarker._0_4_ + scratchValue4)
                    quest:RemoveThing(nil --[[missing]], appleMarker._0_4_ + scratchValue4, false)
                    scratchValue9 = scratchValue9 + 1
                    scratchValue4 = scratchValue4 + 12
                until scratchValue9 >= ((appleMarker._4_4_ - appleMarker._0_4_) / 12)
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
        local preMeleeDummy3 = quest:GetThingWithScriptName("PreMeleeDummy")
        if preMeleeDummy3 ~= nil and preMeleeDummy3:IsAlive() then
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeDummy"), false, true)
        end
        local preMeleeWhisper = quest:GetThingWithScriptName("PreMeleeWhisper")
        if preMeleeWhisper ~= nil and preMeleeWhisper:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
        end
    end
    if quest:GetStateInt("GameState") == 3 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", hero) then
            quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
        end
        if quest:IsObjectInThingsPossession("OBJECT_IRON_KATANA", hero) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_KATANA")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        local meleeApprentice3 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice3 ~= nil and meleeApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        local combatApprentice3 = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice3 ~= nil and combatApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("CombatApprentice"), false, true)
        end
        local skillApprentice3 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice3 ~= nil and skillApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        local willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice3 ~= nil and willApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        local birdKiller3 = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller3 ~= nil and birdKiller3:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("BirdKiller"), false, true)
        end
        quest:SetCategoryActivity("Wannabe Hero", true)
        quest:SetCategoryActivity("Young Apprentice", true)
        quest:SetCategoryActivity("Young Annoyance", true)
        quest:TurnCreatureInto(hero, "CREATURE_HERO")
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
        -- TODO(native): EntitySetAsOpinionSource is not a ForgeFSE binding
        quest:EntitySetAsOpinionSource(hero, "OPINION_SOURCE_HERO_AS_APPRENTICE")
        quest:SetHeroAsTeenager(true)
        quest:SetHeroAsApprentice(true)
        quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, false)
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
        local combatApprentice5 = quest:GetThingWithScriptName("CombatApprentice")
        if not (combatApprentice5 ~= nil and combatApprentice5:IsAlive()) then
            quest:CreateCreature("CombatApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "CombatApprenticeMarker")
            quest:GetThingWithScriptName("CombatApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    if quest:GetStateInt("GameState") == 5 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", hero) then
            quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
        end
        if quest:IsObjectInThingsPossession("OBJECT_YEW_CROSSBOW", hero) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_CROSSBOW")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        quest:SetMasterGameState("MovingDummiesNeeded", false)
        local meleeApprentice5 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice5 ~= nil and meleeApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        quest:CreateCreature("MeleeApprentice", quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"):GetPos(), "M_MeleeOpponentStand")
        local skillApprentice5 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice5 ~= nil and skillApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("SkillApprentice"), false, true)
        end
        local willApprentice5 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice5 ~= nil and willApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("WillApprentice"), false, true)
        end
        local birdKiller5 = quest:GetThingWithScriptName("BirdKiller")
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
        local skillApprentice7 = quest:GetThingWithScriptName("SkillApprentice")
        if not (skillApprentice7 ~= nil and skillApprentice7:IsAlive()) then
            quest:CreateCreature("SkillApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "SkillApprenticeMarker")
            quest:GetThingWithScriptName("SkillApprentice"):SetToKillOnLevelUnload(false)
        end
        local birdKiller7 = quest:GetThingWithScriptName("BirdKiller")
        if not (birdKiller7 ~= nil and birdKiller7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:CreateCreature("BirdKiller", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "BirdKillerMarker")
        end
    end
    if quest:GetStateInt("GameState") == 7 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, false)
        local meleeApprentice7 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice7 ~= nil and meleeApprentice7:IsAlive() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        end
        quest:CreateCreature("MeleeApprentice", quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"):GetPos(), "M_MeleeOpponentStand")
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
        local willApprentice7 = quest:GetThingWithScriptName("WillApprentice")
        if not (willApprentice7 ~= nil and willApprentice7:IsAlive()) then
            quest:CreateCreature("WillApprentice", quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE"):GetPos(), "WillApprenticeMarker")
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
    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    resource4 = resources:NewResource()
    while not resources:TryAcquire(resource4, appleMarker[0 + 1], 4) do
        if not quest:NewScriptFrame() then goto LAB_00d48800 end
    end
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d487f4 end
    end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "GM", resource4)
    movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_EXIT_WOODS", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource4)
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("FrescoDomeHSP"), false)
    quest:SetStateBool("DomeCutsceneStart", true)
    RunCeremonyCutscene(quest)
    quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", -1)
    quest:SetStateBool("DomeCutsceneStart", false)
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("HeroGuildComplexInsideHSP"), false)
    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then goto LAB_00d496bc end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
    quest:FadeScreenOut(0.5, 0.0)
    quest:OpenDoor(secretBookcase)
    quest:SetThingPersistent(secretBookcase, true)
    quest:SetRegionExitAsActive(getNearestWithDefName, true)
    quest:GiveHeroExperience(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_EndGuildXP))))
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
    while not quest:MsgOnLeavingExperienceSpendingScreen() do
        if not quest:NewScriptFrame() then goto LAB_00d496b3 end
        if quest:GetTimer(timerId) == 0 then
            local conversationId = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TheRealGuildmaster", quest:GetThingWithScriptName("TEXT_CS_028_LEAVING_TOUR_45"), hero, false)
            quest:SetTimer(timerId, 10)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        -- TODO(native): NScript::CQ_GuildTrainingScript::RunSaveXPCutscene2__atd49a20(this);
        quest:FadeScreenOut(0.0, 0.5)
        quest:EntityTeleportToThing(quest:GetThingWithScriptName("M_GuildmasterMarker"), quest:GetThingWithScriptName("TheRealGuildmaster"), false)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00d496b3 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:EntityUnsetAsOpinionSource(hero, false)
            quest:SetHeroAsApprentice(false)
            quest:GiveHeroExpression("EXPRESSION_FOLLOW", -1, true)
            quest:GiveHeroExpression("EXPRESSION_WAIT", -1, true)
            quest:SetThingAsUsable(guildDoors, true)
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
                                quest:EnableGuards(quest:GetNearestWithDefName(hero, "VILLAGE_GUILD_COMPLEX_INSIDE"), true)
                                quest:SetHeroSleepingAsEnabled(true)
                                while quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero) do
                                    if not quest:NewScriptFrame() then goto LAB_00d496aa end
                                    quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    local appleRed01 = quest:GetAllThingsWithDefName("OBJECT_APPLE_RED_01")
                                    scratchValue = 0
                                    if #appleRed01 ~= 0 then
                                        scratchValue5 = 0
                                        repeat
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d496a1 end
                                            quest:RemoveThing(theRealGuildmaster, appleRed01 + scratchValue5, false)
                                            scratchValue = scratchValue + 1
                                            scratchValue5 = scratchValue5 + 12
                                        until scratchValue >= #appleRed01
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
                                                            if quest:DisplayTutorial(TUTORIAL_CATEGORY_TAKING_QUESTS) then
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    while not quest:MsgIsTutorialClickedPast() do
                                                                        if not quest:NewScriptFrame() then goto LAB_00d496a1 end
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
    ::LAB_00d487f4::
    resources:ReleaseResource(resource)
    ::LAB_00d48800::
    resources:ReleaseResource(resource4)
    ::LAB_00d496bc::
end

-- Q_GuildTraining.CheckFriendlyAttacks (retail 0x00d45060)
function CheckFriendlyAttacks(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local movie, scratchValue4, predicateResult5, predicateResult6, predicateResult, scratchValue
    local conversationId, scratchValue17, heroWarnings, scratchValue23, scratchValue24, actorMap
    local conversationId2, thing, scratchValue25
    local getThingWithScriptName = quest:GetThingWithScriptName(nil --[[missing]])
    local creatures = quest:GetAllCreaturesExcludingHero()
    local scratchValue16 = getThingWithScriptName - creatures >> 31
    scratchValue23 = 0
    if (getThingWithScriptName - creatures) / 12 + scratchValue16 ~= scratchValue16 then
        scratchValue17 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00d452d1 end
            -- TODO(native): pvVar12 = (**(*xStack_84[(iVar18) / 0xc + 1] + 8))(&xStack_70)
    --[[unresolved native value]]
            if nil == "CREATURE_BIRD_GUILD_SPARROW" then
                predicateResult = false
            else
                -- TODO(native): pvVar12 = (**(*xStack_84[(iVar18) / 0xc + 1] + 8))(&xStack_74)
    --[[unresolved native value]]
                if nil == "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE" then
                    predicateResult = false
                    goto FLOW_after_lab_00d45184
                end
                -- TODO(native): pvVar12 = (**(*xStack_84[(iVar18) / 0xc + 1] + 8))(&xStack_68)
    --[[unresolved native value]]
                if nil == "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" then
                    predicateResult = false
                    goto FLOW_after_lab_00d45184
                end
                -- TODO(native): pvVar12 = (**(*xStack_84[(iVar18) / 0xc + 1] + 8))(&xStack_6c)
    --[[unresolved native value]]
                predicateResult = true
                if nil == "CREATURE_RIVAL_HERO_MAZE" then
                    predicateResult = false
                    goto FLOW_after_lab_00d45184
                end
            end
            ::FLOW_after_lab_00d45184::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then return end
                -- TODO(native): (**(code **)(*(int *)xStack_84[(iVar18) / 0xc + 1] + 0x10c))(1);
                quest:EntitySetAsKillable(creatures[scratchValue17 + 1], false, true)
            end
            scratchValue23 = scratchValue23 + 1
            scratchValue17 = scratchValue17 + 1
        until scratchValue23 >= ((getThingWithScriptName - creatures) / 12)
    end
    if not quest:IsActiveThreadTerminating() then
        repeat
            if quest:IsActiveThreadTerminating() then return end
            if not quest:IsLevelLoaded("HeroGuildComplex") then
                while not quest:IsLevelLoaded("HeroGuildComplex") do
                    if not quest:NewScriptFrame() then goto LAB_00d45322 end
                end
                scratchValue = 0
                local creatures2 = quest:GetAllCreaturesExcludingHero()
                local scratchValue18 = getThingWithScriptName - creatures2 >> 31
                scratchValue24 = 0
                if (getThingWithScriptName - creatures2) / 12 + scratchValue18 ~= scratchValue18 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_BIRD_GUILD_SPARROW" then
                            predicateResult5 = false
                        else
                            if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE" then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                            -- TODO(native): pvVar12 = (**(*xStack_84[(iVar13) / 0xc + 1] + 8))(aCStack_10)
    --[[unresolved native value]]
                            if nil == "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                            -- TODO(native): pvVar12 = (**(*xStack_84[(iVar13) / 0xc + 1] + 8))(xStack_30)
    --[[unresolved native value]]
                            predicateResult5 = true
                            if nil == "CREATURE_RIVAL_HERO_MAZE" then
                                predicateResult5 = false
                                goto FLOW_after_lab_00d4557d
                            end
                        end
                        ::FLOW_after_lab_00d4557d::
                        if predicateResult5 then
                            if quest:IsActiveThreadTerminating() then return end
                            -- TODO(native): (**(code **)(*(int *)xStack_84[(iVar13) / 0xc + 1] + 0x10c))(1);
                            quest:EntitySetAsKillable(creatures2[scratchValue + 1], false, true)
                        end
                        scratchValue24 = scratchValue24 + 1
                        scratchValue = scratchValue + 1
                    until scratchValue24 >= ((getThingWithScriptName - creatures2) / 12)
                end
                if quest:IsActiveThreadTerminating() then return end
            end
            -- TODO(native): MsgHitFriendWithBareHands is not a ForgeFSE binding
            if hero:MsgHitFriendWithBareHands() then
                if not quest:IsLevelLoaded("HeroGuildComplex") then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d45782
                end
                if scratchValue25 ~= nil and scratchValue25:MsgIsHitByHero() then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d45782
                end
                if scratchValue25 ~= nil and scratchValue25:MsgIsHitByAnySpecialAbilityFromHero() then
                    if scratchValue25 == nil or not scratchValue25:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then
                        predicateResult6 = false
                        goto FLOW_after_lab_00d45782
                    end
                end
                predicateResult6 = true
            else
                -- TODO(native): MsgHitFriendWithMeleeWeapon is not a ForgeFSE binding
                if hero:MsgHitFriendWithMeleeWeapon() then
                    if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
                    if scratchValue25 ~= nil and scratchValue25:MsgIsHitByHero() then goto LAB_00d45782 end
                    if scratchValue25 ~= nil and scratchValue25:MsgIsHitByAnySpecialAbilityFromHero() then
                        if scratchValue25 == nil or not scratchValue25:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d45782 end
                    end
                    predicateResult6 = true
                    goto FLOW_after_lab_00d45782
                end
                -- TODO(native): MsgHitFriendWithRangedWeapon is not a ForgeFSE binding
                if hero:MsgHitFriendWithRangedWeapon() then
                    if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
                    if scratchValue25 ~= nil and scratchValue25:MsgIsHitByHero() then goto LAB_00d45782 end
                    if scratchValue25 ~= nil and scratchValue25:MsgIsHitByAnySpecialAbilityFromHero() then
                        if scratchValue25 == nil or not scratchValue25:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d45782 end
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
                    local resource = resources:NewResource()
                    while not resources:TryAcquire(resource, thing, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d45db2 end
                    end
                    if not hero:AcquireControl(4) then hero:ReleaseControl(); goto LAB_00d45db2 end
                    actorMap = resources:NewActorMap()
                    -- TODO(native): resources:SetActor(amStack_1c, "HERO", &xStack_20)
                    resources:SetActor(scratchValue4, "MAZE", resource)
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    conversationId2 = quest:AddNewConversation(hero, false, false)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", hero, hero, false)
                    quest:Pause(2.0)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_BADHERO", scratchValue4, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(scratchValue4)
                    resources:ReleaseResource(conversationId2)
                    goto LAB_00d45c9d
                    hero:ReleaseControl()
                    ::LAB_00d45db2::
                    resources:ReleaseResource(resource)
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
            conversationId = quest:AddNewConversation(hero, false, false)
            heroWarnings = quest:GetStateInt("HeroWarnings")
            if heroWarnings == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", hero, hero, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            elseif heroWarnings == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", hero, hero, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            elseif heroWarnings ~= 2 then
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", hero, hero, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            ::LAB_00d45cae::
            quest:NewScriptFrame()
        until false
    end
    ::LAB_00d45322::
    do return end
    ::LAB_00d452d1::
    goto LAB_00d45322
end

-- Q_GuildTraining.KeepTabsOnWhisper (retail 0x00d3cbd0)
function KeepTabsOnWhisper(quest)
    local scratchValue, scratchValue2, predicateResult, scratchValue4
    if quest:IsActiveThreadTerminating() then return end
    scratchValue = 0
    repeat
        scratchValue2 = scratchValue | 1
        scratchValue4 = scratchValue2
        if quest:IsLevelLoaded("GuildWoods") then
            scratchValue2 = scratchValue | 3
            scratchValue4 = scratchValue2
            predicateResult = true
            if not quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                predicateResult = false
                goto FLOW_after_lab_00d3cc48
            end
        else
            predicateResult = false
        end
        ::FLOW_after_lab_00d3cc48::
        if scratchValue2 & 2 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffd
            scratchValue4 = scratchValue2
        end
        if scratchValue2 & 1 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffe
            scratchValue4 = scratchValue2
        end
        scratchValue = scratchValue2
        if predicateResult then
            while not quest:IsLevelLoaded("HeroGuildComplex") do
                if not quest:NewScriptFrame() then return end
            end
            scratchValue = scratchValue4
            if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                scratchValue = scratchValue4
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
    local getNearestWithDefName = quest:GetNearestWithDefName(quest:GetThingWithScriptName("SecretBookcase"), "REGION_EXIT_POINT")
    while quest:GetStateInt("GameState") ~= 9 do
        if not quest:NewScriptFrame() then goto LAB_00d3cac6 end
        while quest:IsLevelLoaded("HeroGuildComplex") do
            if not quest:NewScriptFrame() then return end
        end
        while not quest:IsLevelLoaded("HeroGuildComplex") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:GetStateInt("GameState") == 9 then break end
        quest:SetRegionExitAsActive(getNearestWithDefName, false)
    end
    ::LAB_00d3cac6::
end

-- Q_GuildTraining.RunArrivalCutscene (retail 0x00d44cb0)
function RunArrivalCutscene(quest)
    local hero = quest:GetHero()
    quest:SetTimeOfDay(19.0)
    if not quest:NewScriptFrame() then return end
    local rivalHeroMazeCutscene = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", quest:GetThingWithScriptName("MK_GTA_MAZE1"):GetPos(), "CutsceneMaze")
    if not rivalHeroMazeCutscene:AcquireControl(4) then goto LAB_00d44ec4 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d44ec4 end
    if not hero:AcquireControl(4) then hero:ReleaseControl(); goto LAB_00d44ec4 end
    quest:StartCutscene({MAZE = rivalHeroMazeCutscene, HERO = hero}, {}, true)
    quest:RunCutscene("CS_GUILD_ARRIVE", true, false)
    quest:FixMovieSequenceCamera(false)
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetAllowScreenFadingOnNextRegionChange(false)
    quest:SetRegionTextDisplayAsActive(false)
    quest:EndCutscene()
    hero:ReleaseControl()
    rivalHeroMazeCutscene:ReleaseControl()
    quest:RemoveThing(rivalHeroMazeCutscene, false, true)
    do return end
    hero:ReleaseControl()
    ::LAB_00d44ec4::
    rivalHeroMazeCutscene:ReleaseControl()
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
    local hero = quest:GetHero()
    while not quest:IsLevelLoaded("FrescoDome") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local rivalHeroWhisperApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MK_GTC_WHISSTART"):GetPos(), "Whisper")
    local guildkeeper = quest:CreateCreature("CREATURE_GUILDKEEPER", quest:GetThingWithScriptName("MK_GTC_GMSTART"):GetPos(), "GM")
    if not rivalHeroWhisperApprentice:AcquireControl(4) then goto LAB_00d4a246 end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4a246 end
    quest:EntitySetInFaction(rivalHeroWhisperApprentice, "FACTION_HERO")
    if not guildkeeper:AcquireControl(4) then goto LAB_00d4a23d end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4a23d end
    if not hero:AcquireControl(4) then hero:ReleaseControl(); goto LAB_00d4a23d end
    quest:StartCutscene({HERO = hero, WHIS = rivalHeroWhisperApprentice, GM = guildkeeper}, {}, true)
    quest:RunCutscene("CS_GUILD_CEREMONY", true, false)
    quest:FixMovieSequenceCamera(false)
    quest:RemoveThing(guildkeeper, false, true)
    quest:RemoveThing(rivalHeroWhisperApprentice, false, true)
    quest:EndCutscene()
    hero:ReleaseControl()
    ::LAB_00d4a23d::
    guildkeeper:ReleaseControl()
    ::LAB_00d4a246::
    rivalHeroWhisperApprentice:ReleaseControl()
end

-- Q_GuildTraining.RunSaveXPCutscene (retail 0x00d496f0)
function RunSaveXPCutscene(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if hero:AcquireControl(4) then
        quest:StartCutscene({GM = theRealGuildmaster, HERO = hero}, {}, true)
        quest:RunCutscene("CS_GUILD_SAVEXP", true, false)
        quest:EndCutscene()
    end
    hero:ReleaseControl()
    resources:ReleaseResource(resource)
end

-- Q_GuildTraining.RunSaveXPCutscene2 (retail 0x00d49a20)
function RunSaveXPCutscene2(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if hero:AcquireControl(4) then
        quest:StartCutscene({GM = theRealGuildmaster, HERO = hero}, {}, true)
        quest:RunCutscene("CS_GUILD_SAVEXP2", true, false)
        quest:EndCutscene()
    end
    hero:ReleaseControl()
    resources:ReleaseResource(resource)
end

