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
            quest:AddEntityBinding("AppleGirl", "GuildTraining/Entities/AppleGirl", 1)
            quest:AddEntityBinding("ApprenticeSpeedTest", "GuildTraining/Entities/ApprenticeSpeedTest", 1)
            quest:AddEntityBinding("SpeedFriend", "GuildTraining/Entities/SpeedFriend", 1)
            quest:AddEntityBinding("RaceMarker", "GuildTraining/Entities/RaceMarker", 1)
            quest:AddEntityBinding("MeleeApprentice", "GuildTraining/Entities/MeleeApprentice", 1)
            quest:AddEntityBinding("CombatApprentice", "GuildTraining/Entities/CombatApprentice", 1)
            quest:AddEntityBinding("SkillApprentice", "GuildTraining/Entities/SkillApprentice", 1)
            quest:AddEntityBinding("WillApprentice", "GuildTraining/Entities/WillApprentice", 1)
            quest:AddEntityBinding("HeroBed", "GuildTraining/Entities/HeroBed", 1)
            quest:AddEntityBinding("SkillTarget", "GuildTraining/Entities/SkillTarget", 1)
            quest:AddEntityBinding("WillDummy", "GuildTraining/Entities/WillDummy", 1)
            quest:AddEntityBinding("BirdKiller", "GuildTraining/Entities/BirdKiller", 1)
            quest:AddEntityBinding("KillBird", "GuildTraining/Entities/KillBird", 1)
            quest:AddEntityBinding("PreMeleeMaze", "GuildTraining/Entities/PreMeleeMaze", 1)
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
    quest:SetStateInt("GameState", quest:PersistTransferInt(context, "GameState", quest:GetStateInt("GameState") or 0))
    quest:SetStateInt("CurrentBirdsKilled", quest:PersistTransferInt(context, "CurrentBirdsKilled", quest:GetStateInt("CurrentBirdsKilled") or 0))
end

-- Q_GuildTraining.RunTutorials (retail 0x00d45dd0)
function RunTutorials(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, scratchValue4, meleeOpponent, meleeApprentice9, skillApprentice9
    local willApprentice9, actorMap, theRealGuildmaster, scratchValue8, scratchValue9, movie
    local resource, resource4, timerId
    local secretBookcase = quest:GetThingWithScriptName("SecretBookcase")
    local getNearestWithDefName = quest:GetNearestWithDefName(secretBookcase, "REGION_EXIT_POINT")
    quest:SetRegionExitAsActive(getNearestWithDefName, false)
    quest:SetExperienceSpendingAsEnabled(false)
    quest:SetHeroSleepingAsEnabled(false)
    local guildDoors = quest:GetThingWithScriptName("GuildDoors")
    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
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
            quest:CreateObject("OBJECT_STRAW_DUMMY_01", quest:GetThingWithScriptName("PreMeleeDummyMarker"):GetPos(), "PreMeleeDummy")
            quest:EntitySetFacingAngle(quest:GetThingWithScriptName("PreMeleeDummy"), quest:GetThingWithScriptName("PreMeleeDummyMarker"):GetAngleXY(), true)
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
            local appleMarker = quest:GetAllThingsWithScriptName("AppleMarker")
            if #appleMarker ~= 0 then
                scratchValue = 0
                scratchValue8 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d46c49 end
                    quest:SetThingPersistent(quest:CreateObject("OBJECT_APPLE_RED_01", appleMarker[scratchValue + 1]:GetPos(), ""), true)
                    quest:SetThingPersistent(appleMarker[scratchValue + 1], true)
                    quest:RemoveThing(appleMarker[scratchValue + 1], false, true)
                    scratchValue8 = scratchValue8 + 1
                    scratchValue = scratchValue + 1
                until scratchValue8 >= #appleMarker
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d46c49 end
            goto FLOW_past_lab_00d46c49
            ::LAB_00d46c49::
            goto LAB_00d496bc
            ::FLOW_past_lab_00d46c49::
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
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1, true)
        end
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
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("CombatApprenticeMarker"):GetPos(), "CombatApprentice")
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
        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos(), "MeleeApprentice")
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
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "SkillApprentice")
            quest:GetThingWithScriptName("SkillApprentice"):SetToKillOnLevelUnload(false)
        end
        local birdKiller7 = quest:GetThingWithScriptName("BirdKiller")
        if not (birdKiller7 ~= nil and birdKiller7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("BirdKillerMarker"):GetPos(), "BirdKiller")
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
        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos(), "MeleeApprentice")
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
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
            quest:GetThingWithScriptName("WillApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    meleeApprentice9 = quest:GetThingWithScriptName("MeleeApprentice")
    if meleeApprentice9 ~= nil and meleeApprentice9:IsAlive() then
        if quest:IsActiveThreadTerminating() then goto LAB_00d496bc end
        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
    end
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
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
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, theRealGuildmaster, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d48800 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d48800 end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d487f4 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d487f4 end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "GM", resource4)
    movie = resources:StartMovie("")
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
    quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", -1, true)
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
    quest:GiveHeroObject("OBJECT_HERO_BOOTS", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_TROUSERS", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_SHIRT", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_GLOVES", -1, true)
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
    RunSaveXPCutscene(quest)
    quest:SetIsPushableByHero(quest:GetThingWithScriptName("TheRealGuildmaster"), false)
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_14", "HeroGuildComplexInside", "")
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 10)
    while not quest:MsgOnLeavingExperienceSpendingScreen() do
        if not quest:NewScriptFrame() then goto LAB_00d496b3 end
        if quest:GetTimer(timerId) == 0 then
            local conversationId = quest:AddNewConversation(quest:GetThingWithScriptName("TheRealGuildmaster"), false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_CS_028_LEAVING_TOUR_45", quest:GetThingWithScriptName("TheRealGuildmaster"), hero, false)
            quest:SetTimer(timerId, 10)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        RunSaveXPCutscene2(quest)
        quest:FadeScreenOut(0.0, 0.5)
        quest:EntityTeleportToThing(quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetThingWithScriptName("M_GuildmasterMarker"), false)
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
                                    scratchValue9 = 0
                                    if #appleRed01 ~= 0 then
                                        scratchValue4 = 0
                                        repeat
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d496a1 end
                                            quest:RemoveThing(appleRed01[scratchValue4 + 1], false, true)
                                            scratchValue9 = scratchValue9 + 1
                                            scratchValue4 = scratchValue4 + 1
                                        until scratchValue9 >= #appleRed01
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
                                                            if not quest:DisplayTutorial(TUTORIAL_CATEGORY_TAKING_QUESTS) then goto LAB_00d4967d end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                while not quest:MsgIsTutorialClickedPast() do
                                                                    if not quest:NewScriptFrame() then goto LAB_00d496a1 end
                                                                end
                                                                if not quest:IsActiveThreadTerminating() then goto LAB_00d4967d end
                                                            end
                                                            goto FLOW_past_lab_00d4967d
                                                            ::LAB_00d4967d::
                                                            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                                                            ::FLOW_past_lab_00d4967d::
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
    local movie, actorMap, predicateResult5, predicateResult6, predicateResult, scratchValue
    local conversationId, scratchValue11, heroWarnings, resource, scratchValue22, scratchValue23
    local resource2
    local preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
    local creatures = quest:GetAllCreaturesExcludingHero()
    scratchValue22 = 0
    if #creatures ~= 0 then
        scratchValue11 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00d452d1 end
            if creatures[scratchValue11 + 1]:GetDefName() == "CREATURE_BIRD_GUILD_SPARROW" then
                goto LAB_00d45184
            else
                if creatures[scratchValue11 + 1]:GetDefName() == "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE" then goto LAB_00d45184 end
                if creatures[scratchValue11 + 1]:GetDefName() == "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" then goto LAB_00d45184 end
                predicateResult = true
                if creatures[scratchValue11 + 1]:GetDefName() == "CREATURE_RIVAL_HERO_MAZE" then goto LAB_00d45184 end
            end
            goto FLOW_past_lab_00d45184
            ::LAB_00d45184::
            predicateResult = false
            ::FLOW_past_lab_00d45184::
            if predicateResult then
                creatures[scratchValue11 + 1]:SetFriendsWithEverythingFlag(true)
                quest:EntitySetAsKillable(creatures[scratchValue11 + 1], false, true)
            end
            scratchValue22 = scratchValue22 + 1
            scratchValue11 = scratchValue11 + 1
        until scratchValue22 >= #creatures
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
                scratchValue23 = 0
                if #creatures2 ~= 0 then
                    repeat
                        if quest:IsActiveThreadTerminating() then return end
                        if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_BIRD_GUILD_SPARROW" then
                            goto LAB_00d4557d
                        else
                            if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE" then goto LAB_00d4557d end
                            if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" then goto LAB_00d4557d end
                            predicateResult5 = true
                            if creatures2[scratchValue + 1]:GetDefName() == "CREATURE_RIVAL_HERO_MAZE" then goto LAB_00d4557d end
                        end
                        goto FLOW_past_lab_00d4557d
                        ::LAB_00d4557d::
                        predicateResult5 = false
                        ::FLOW_past_lab_00d4557d::
                        if predicateResult5 then
                            creatures2[scratchValue + 1]:SetFriendsWithEverythingFlag(true)
                            quest:EntitySetAsKillable(creatures2[scratchValue + 1], false, true)
                        end
                        scratchValue23 = scratchValue23 + 1
                        scratchValue = scratchValue + 1
                    until scratchValue23 >= #creatures2
                end
                if quest:IsActiveThreadTerminating() then return end
            end
            if hero ~= nil and hero:MsgHitFriendWithBareHands() then
                goto LAB_00d456af
            else
                if hero ~= nil and hero:MsgHitFriendWithMeleeWeapon() then goto LAB_00d456af end
                if hero ~= nil and hero:MsgHitFriendWithRangedWeapon() then goto LAB_00d456af end
                goto LAB_00d45782
            end
            goto FLOW_past_lab_00d456af
            ::LAB_00d456af::
            if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
            if (preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) and preMeleeMaze:MsgIsHitByHero() then goto LAB_00d45782 end
            if (preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) and preMeleeMaze:MsgIsHitByAnySpecialAbilityFromHero() then
                if not (preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) or not preMeleeMaze:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d45782 end
            end
            predicateResult6 = true
            ::FLOW_past_lab_00d456af::
            goto FLOW_past_lab_00d45782
            ::LAB_00d45782::
            predicateResult6 = false
            ::FLOW_past_lab_00d45782::
            if not predicateResult6 then goto LAB_00d45cae end
            if quest:IsActiveThreadTerminating() then return end
            if 2 < quest:GetStateInt("HeroWarnings") then
                quest:SetMasterGameState("GuildWarningOccuring", true)
                if not quest:GetMasterGameState("SkillTestOccuring") and not quest:GetMasterGameState("WillTestOccuring") then
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, preMeleeMaze, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d45db2 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d45db2 end
                    resource2 = resources:NewResource()
                    resources:PrepareResource(resource2)
                    resource = resource2
                    while not resources:TryAcquire(resource, hero, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d45da9 end
                        resource = resource2
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d45da9 end
                    actorMap = resources:NewActorMap()
                    resources:SetActor(actorMap, "HERO", resource2)
                    resources:SetActor(actorMap, "MAZE", resource3)
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", hero, hero, false)
                    quest:Pause(2.0)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_BADHERO", actorMap, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource3)
                    goto LAB_00d45c9d
                    ::LAB_00d45da9::
                    resources:ReleaseResource(resource2)
                    ::LAB_00d45db2::
                    resources:ReleaseResource(resource3)
                    return
                end
                while quest:GetMasterGameState("SkillTestOccuring") or quest:GetMasterGameState("WillTestOccuring") do
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
                goto LAB_00d45930
            elseif heroWarnings == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", hero, hero, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                if heroWarnings ~= 2 then goto LAB_00d45930 end
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", hero, hero, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            goto FLOW_past_lab_00d45930
            ::LAB_00d45930::
            quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            ::FLOW_past_lab_00d45930::
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
        if not quest:IsLevelLoaded("GuildWoods") then goto LAB_00d3cc48 end
        scratchValue2 = scratchValue | 3
        scratchValue4 = scratchValue2
        predicateResult = true
        if not quest:IsQuestActive("Q_GuildTrainingWoodsWill") then goto LAB_00d3cc48 end
        goto FLOW_past_lab_00d3cc48
        ::LAB_00d3cc48::
        predicateResult = false
        ::FLOW_past_lab_00d3cc48::
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
    local resources = quest:RetailResources()
    local actorMap, movie, resource
    quest:SetTimeOfDay(19.0)
    if not quest:NewScriptFrame() then return end
    local rivalHeroMazeCutscene = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", quest:GetThingWithScriptName("MK_GTA_MAZE1"):GetPos(), "CutsceneMaze")
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, rivalHeroMazeCutscene, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d44ec4 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d44ec4 end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d44ebb end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d44ebb end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "MAZE", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_ARRIVE", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetAllowScreenFadingOnNextRegionChange(false)
    quest:SetRegionTextDisplayAsActive(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
    quest:RemoveThing(rivalHeroMazeCutscene, false, true)
    do return end
    ::LAB_00d44ebb::
    resources:ReleaseResource(resource)
    ::LAB_00d44ec4::
    resources:ReleaseResource(resource2)
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
    local resources = quest:RetailResources()
    local movie, resource, resource4, actorMap
    while not quest:IsLevelLoaded("FrescoDome") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local rivalHeroWhisperApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MK_GTC_WHISSTART"):GetPos(), "Whisper")
    local guildkeeper = quest:CreateCreature("CREATURE_GUILDKEEPER", quest:GetThingWithScriptName("MK_GTC_GMSTART"):GetPos(), "GM")
    local resource5 = resources:NewResource()
    resources:PrepareResource(resource5)
    while not resources:TryAcquire(resource5, rivalHeroWhisperApprentice, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d4a246 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4a246 end
    quest:EntitySetInFaction(rivalHeroWhisperApprentice, "FACTION_HERO")
    resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, guildkeeper, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d4a23d end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4a23d end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d4a234 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4a234 end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "WHIS", resource5)
    resources:SetActor(actorMap, "GM", resource4)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_CEREMONY", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:RemoveThing(guildkeeper, false, true)
    quest:RemoveThing(rivalHeroWhisperApprentice, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    ::LAB_00d4a234::
    resources:ReleaseResource(resource)
    ::LAB_00d4a23d::
    resources:ReleaseResource(resource4)
    ::LAB_00d4a246::
    resources:ReleaseResource(resource5)
end

-- Q_GuildTraining.RunSaveXPCutscene (retail 0x00d496f0)
function RunSaveXPCutscene(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local movie, actorMap
    local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d4997a end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4997a end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "GM", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_SAVEXP", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    ::LAB_00d4997a::
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
end

-- Q_GuildTraining.RunSaveXPCutscene2 (retail 0x00d49a20)
function RunSaveXPCutscene2(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local movie, actorMap
    local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00d49caa end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d49caa end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "GM", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_SAVEXP2", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    ::LAB_00d49caa::
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
end

