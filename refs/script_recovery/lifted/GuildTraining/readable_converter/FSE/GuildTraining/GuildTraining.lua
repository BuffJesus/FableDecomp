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
    local isLevelLoaded, guildTrainingHSP
    if quest:GetStateInt("GameState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveAllHeroWeapons()
        local guildArrivalHSP = quest:GetThingWithScriptName("GuildArrivalHSP")
        local hero2 = quest:GetHero()
        quest:EntityTeleportToThing(hero2, guildArrivalHSP, false)
        while not quest:IsLevelLoaded("LookoutPoint") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_MELEE", "Q_GuildTraining", false)
        if not quest:NewScriptFrame() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(getActiveQuestName, false, true)
        RunArrivalCutscene(quest)
        quest:FadeScreenOut(0.5, 0.0)
        if not quest:NewScriptFrame() then return end
    end
    guildTrainingHSP = quest:GetThingWithScriptName("GuildTrainingHSP")
    local hero3 = quest:GetHero()
    quest:EntityTeleportToThing(hero3, guildTrainingHSP, false)
    guildTrainingHSP = nil
    isLevelLoaded = quest:IsLevelLoaded("HeroGuildComplex")
    while true do
        if isLevelLoaded then
            local predicateResult = quest:IsActiveThreadTerminating()
            if predicateResult then return end
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
            local hero = quest:GetHero()
            local getNearestWithDefName = quest:GetNearestWithDefName(hero, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EnableGuards(getNearestWithDefName, false)
            quest:CreateThread("RunTutorials")  -- native thread body 0x00D45DD0: lift it as function RunTutorials(quest)
            if not predicateResult then
            end
            quest:CreateThread("CheckFriendlyAttacks")  -- native thread body 0x00D45060: lift it as function CheckFriendlyAttacks(quest)
            quest:CreateThread("KeepTabsOnWhisper")  -- native thread body KeepTabsOnWhisper: lift it as function KeepTabsOnWhisper(quest)
            if not predicateResult then
            end
            quest:CreateThread("WatchForSparrowKilled")  -- native thread body 0x00D3C840: lift it as function WatchForSparrowKilled(quest)
            if not predicateResult then
            end
            quest:CreateThread("KeepBookcaseExitRemoved")  -- native thread body KeepBookcaseExitRemoved: lift it as function KeepBookcaseExitRemoved(quest)
            if not predicateResult then
            end
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
    local gameState = quest:GetStateInt("GameState") or 0
    gameState = quest:PersistTransferInt(context, "GameState", gameState)
    quest:SetStateInt("GameState", gameState)
    local currentBirdsKilled = quest:GetStateInt("CurrentBirdsKilled") or 0
    currentBirdsKilled = quest:PersistTransferInt(context, "CurrentBirdsKilled", currentBirdsKilled)
    quest:SetStateInt("CurrentBirdsKilled", currentBirdsKilled)
end

-- Q_GuildTraining.RunTutorials (retail 0x00d45dd0)
function RunTutorials(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue3, scratchValue4, theRealGuildmaster, hero16, position3
    local position4, position5, position6, position7, position8, position9, hero17, hero18
    local theRealGuildmaster4, theRealGuildmaster5, getActiveQuestName, getActiveQuestName2
    local secretBookcase, theRealGuildmaster6, getNearestWithDefName, getNearestWithDefName2
    local guildDoors, scratchValue6, thing, scratchValue8, scratchValue9, movie, resource, resource4
    local appleMarker, actorMap, timerId
    secretBookcase = quest:GetThingWithScriptName("SecretBookcase")
    getNearestWithDefName2 = quest:GetNearestWithDefName(secretBookcase, "REGION_EXIT_POINT")
    quest:SetRegionExitAsActive(getNearestWithDefName2, false)
    quest:SetExperienceSpendingAsEnabled(false)
    quest:SetHeroSleepingAsEnabled(false)
    guildDoors = quest:GetThingWithScriptName("GuildDoors")
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:MiniMapAddMarker(theRealGuildmaster, "HUD_ORB_QUEST_CORE")
    theRealGuildmaster = nil
    if quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsQuestActive("Q_GuildTrainingDeparture") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsQuestActive("Q_GuildTrainingMelee") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsQuestActive("Q_GuildTrainingSkill") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
        if not quest:NewScriptFrame() then return end
    end
    if quest:GetStateInt("GameState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingAsUsable(guildDoors, false)
        quest:SetThingPersistent(guildDoors, true)
        local hero = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", hero) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_GUILD_SEAL_1")
        end
        quest:SetStateInt("GameState", 1)
    end
    local heroGuildTeleportMarker = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    quest:SetTeleporterAsActive(heroGuildTeleportMarker, false)
    local meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    if meleeOpponent ~= nil and meleeOpponent:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        local meleeOpponent2 = quest:GetThingWithScriptName("MeleeOpponent")
        quest:RemoveThing(meleeOpponent2, false, true)
    end
    if quest:GetStateInt("GameState") == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetMasterGameState("ScorpionsDestroyed", false)
        quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", false)
        quest:SetMasterGameState("SkillTrainingStarted", false)
        local preMeleeDummy = quest:GetThingWithScriptName("PreMeleeDummy")
        if not (preMeleeDummy ~= nil and preMeleeDummy:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            local position = quest:GetThingWithScriptName("PreMeleeDummyMarker"):GetPos()
            quest:CreateObject("OBJECT_STRAW_DUMMY_01", position, "PreMeleeDummy")
            local angle = quest:GetThingWithScriptName("PreMeleeDummyMarker"):GetAngleXY()
            local preMeleeDummy2 = quest:GetThingWithScriptName("PreMeleeDummy")
            quest:EntitySetFacingAngle(preMeleeDummy2, angle, true)
        end
        local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice ~= nil and meleeApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(meleeApprentice2, false, true)
        end
        local combatApprentice = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice ~= nil and combatApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local combatApprentice2 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(combatApprentice2, false, true)
        end
        local skillApprentice = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice ~= nil and skillApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local skillApprentice2 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(skillApprentice2, false, true)
        end
        local willApprentice = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice ~= nil and willApprentice:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice2 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(willApprentice2, false, true)
        end
        local birdKiller = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller ~= nil and birdKiller:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local birdKiller2 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(birdKiller2, false, true)
        end
        local hero2 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_HERO_STICK", hero2) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_HERO_STICK")
        end
        quest:SetCategoryActivity("Sweet Kid", true)
        if not quest:IsQuestActive("Q_GuildTrainingPreMelee") then
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("Q_GuildTrainingPreMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingPreMelee", false)
            if not quest:NewScriptFrame() then return end
            appleMarker = quest:GetAllThingsWithScriptName("AppleMarker")
            if #appleMarker ~= 0 then
                scratchValue3 = 0
                scratchValue8 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d46c49 end
                    local position2 = appleMarker[scratchValue3 + 1]:GetPos()
                    scratchValue6 = quest:CreateObject("OBJECT_APPLE_RED_01", position2, "")
                    quest:SetThingPersistent(scratchValue6, true)
                    quest:SetThingPersistent(appleMarker[scratchValue3 + 1], true)
                    quest:RemoveThing(appleMarker[scratchValue3 + 1], false, true)
                    scratchValue8 = scratchValue8 + 1
                    scratchValue3 = scratchValue3 + 1
                until not (scratchValue8 < (#appleMarker))
            end
            if not (quest:IsActiveThreadTerminating()) then
                goto FLOW_past_lab_00d46c49
            end
            ::LAB_00d46c49::
            do return end
            ::FLOW_past_lab_00d46c49::
        end
        while quest:IsQuestActive("Q_GuildTrainingPreMelee") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then
            if quest:IsActiveThreadTerminating() then return end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        end
        quest:SetCategoryActivity("Sweet Kid", false)
        quest:SetStateInt("GameState", 3)
        quest:GiveHeroGold(50)
    else
        if quest:IsActiveThreadTerminating() then return end
        local preMeleeDummy3 = quest:GetThingWithScriptName("PreMeleeDummy")
        if preMeleeDummy3 ~= nil and preMeleeDummy3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local preMeleeDummy4 = quest:GetThingWithScriptName("PreMeleeDummy")
            quest:RemoveThing(preMeleeDummy4, false, true)
        end
        local preMeleeWhisper = quest:GetThingWithScriptName("PreMeleeWhisper")
        if preMeleeWhisper ~= nil and preMeleeWhisper:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local preMeleeWhisper2 = quest:GetThingWithScriptName("PreMeleeWhisper")
            quest:RemoveThing(preMeleeWhisper2, false, true)
        end
    end
    if quest:GetStateInt("GameState") == 3 then
        if quest:IsActiveThreadTerminating() then return end
        local hero3 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", hero3) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
        end
        local hero4 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_IRON_KATANA", hero4) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_IRON_KATANA")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        local meleeApprentice3 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice3 ~= nil and meleeApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local meleeApprentice4 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(meleeApprentice4, false, true)
        end
        local combatApprentice3 = quest:GetThingWithScriptName("CombatApprentice")
        if combatApprentice3 ~= nil and combatApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local combatApprentice4 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(combatApprentice4, false, true)
        end
        local skillApprentice3 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice3 ~= nil and skillApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local skillApprentice4 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(skillApprentice4, false, true)
        end
        local willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice3 ~= nil and willApprentice3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice4 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(willApprentice4, false, true)
        end
        local birdKiller3 = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller3 ~= nil and birdKiller3:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local birdKiller4 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(birdKiller4, false, true)
        end
        quest:SetCategoryActivity("Wannabe Hero", true)
        quest:SetCategoryActivity("Young Apprentice", true)
        quest:SetCategoryActivity("Young Annoyance", true)
        local hero5 = quest:GetHero()
        quest:TurnCreatureInto(hero5, "CREATURE_HERO")
        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
        quest:GiveHeroExpression("EXPRESSION_FLIRT", -1, true)
        quest:GiveHeroExpression("EXPRESSION_COSSACK", -1, true)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1, true)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1, true)
        end
        local hero6 = quest:GetHero()
        quest:EntitySetAsOpinionSource(hero6, "OPINION_SOURCE_HERO_AS_APPRENTICE")
        quest:SetHeroAsTeenager(true)
        quest:SetHeroAsApprentice(true)
        quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, false)
        if not quest:IsQuestActive("Q_GuildTrainingMelee") then
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("Q_GuildTrainingMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingMelee", false)
        end
        if not quest:NewScriptFrame() then return end
        while quest:IsQuestActive("Q_GuildTrainingMelee") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveQuestCardFromGuild("Q_GuildTraining")
        quest:SetCategoryActivity("Complete Melee", true)
        quest:SetStateInt("GameState", 5)
    else
        if quest:IsActiveThreadTerminating() then return end
        local combatApprentice5 = quest:GetThingWithScriptName("CombatApprentice")
        if not (combatApprentice5 ~= nil and combatApprentice5:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            position3 = quest:GetThingWithScriptName("CombatApprenticeMarker"):GetPos()
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position3, "CombatApprentice", false)
            quest:GetThingWithScriptName("CombatApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    if quest:GetStateInt("GameState") == 5 then
        if quest:IsActiveThreadTerminating() then return end
        local hero7 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", hero7) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
        end
        local hero8 = quest:GetHero()
        if quest:IsObjectInThingsPossession("OBJECT_YEW_CROSSBOW", hero8) then
            if quest:IsActiveThreadTerminating() then return end
            quest:TakeObjectFromHero("OBJECT_YEW_CROSSBOW")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        quest:SetMasterGameState("MovingDummiesNeeded", false)
        local meleeApprentice5 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice5 ~= nil and meleeApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local meleeApprentice6 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(meleeApprentice6, false, true)
        end
        position4 = quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos()
        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", position4, "MeleeApprentice", false)
        local skillApprentice5 = quest:GetThingWithScriptName("SkillApprentice")
        if skillApprentice5 ~= nil and skillApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local skillApprentice6 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(skillApprentice6, false, true)
        end
        local willApprentice5 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice5 ~= nil and willApprentice5:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local willApprentice6 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(willApprentice6, false, true)
        end
        local birdKiller5 = quest:GetThingWithScriptName("BirdKiller")
        if birdKiller5 ~= nil and birdKiller5:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local birdKiller6 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(birdKiller6, false, true)
        end
        if not quest:IsQuestActive("Q_GuildTrainingSkill") then
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("Q_GuildTrainingSkill")
            quest:SetQuestAsPersistent("Q_GuildTrainingSkill", false)
        end
        if not quest:NewScriptFrame() then return end
        while quest:IsQuestActive("Q_GuildTrainingSkill") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetCategoryActivity("Complete Skill", true)
        quest:SetStateInt("GameState", 7)
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("MovingDummiesNeeded", true)
        local skillApprentice7 = quest:GetThingWithScriptName("SkillApprentice")
        if not (skillApprentice7 ~= nil and skillApprentice7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            position5 = quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos()
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position5, "SkillApprentice", false)
            quest:GetThingWithScriptName("SkillApprentice"):SetToKillOnLevelUnload(false)
        end
        local birdKiller7 = quest:GetThingWithScriptName("BirdKiller")
        if not (birdKiller7 ~= nil and birdKiller7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            position6 = quest:GetThingWithScriptName("BirdKillerMarker"):GetPos()
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position6, "BirdKiller", false)
        end
    end
    if quest:GetStateInt("GameState") == 7 then
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, false)
        local meleeApprentice7 = quest:GetThingWithScriptName("MeleeApprentice")
        if meleeApprentice7 ~= nil and meleeApprentice7:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            local meleeApprentice8 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(meleeApprentice8, false, true)
        end
        position7 = quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos()
        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", position7, "MeleeApprentice", false)
        if not quest:IsQuestActive("Q_GuildTrainingWill") then
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("Q_GuildTrainingWill")
        end
        if not quest:NewScriptFrame() then return end
        while quest:IsQuestActive("Q_GuildTrainingWill") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetCategoryActivity("Complete Will", true)
        quest:SetStateInt("GameState", 9)
    else
        if quest:IsActiveThreadTerminating() then return end
        local willApprentice7 = quest:GetThingWithScriptName("WillApprentice")
        if not (willApprentice7 ~= nil and willApprentice7:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            position8 = quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos()
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position8, "WillApprentice", false)
            quest:GetThingWithScriptName("WillApprentice"):SetToKillOnLevelUnload(false)
        end
    end
    local meleeApprentice9 = quest:GetThingWithScriptName("MeleeApprentice")
    if meleeApprentice9 ~= nil and meleeApprentice9:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        local meleeApprentice10 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(meleeApprentice10, false, true)
    end
    position9 = quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos()
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", position9, "MeleeApprentice", false)
    if not quest:IsQuestActive("Q_GuildTrainingDeparture") then
        if quest:IsActiveThreadTerminating() then return end
        quest:ActivateQuest("Q_GuildTrainingDeparture")
        quest:SetQuestAsPersistent("Q_GuildTrainingDeparture", false)
    end
    if not quest:NewScriptFrame() then return end
    while quest:IsQuestActive("Q_GuildTrainingDeparture") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_13", "HeroGuildComplexInside", "")
    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    theRealGuildmaster6 = quest:GetThingWithScriptName("TheRealGuildmaster")
    resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, theRealGuildmaster6, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource4); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    local hero9 = quest:GetHero()
    scratchValue = resources:TryAcquire(resource, hero9, 4)
    while not scratchValue do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); resources:ReleaseResource(resource4); return end
        local hero10 = quest:GetHero()
        scratchValue = resources:TryAcquire(resource, hero10, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); resources:ReleaseResource(resource4); return end
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
    local frescoDomeHSP = quest:GetThingWithScriptName("FrescoDomeHSP")
    hero17 = quest:GetHero()
    quest:EntityTeleportToThing(hero17, frescoDomeHSP, false)
    quest:SetStateBool("DomeCutsceneStart", true)
    RunCeremonyCutscene(quest)
    quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", -1, true)
    quest:SetStateBool("DomeCutsceneStart", false)
    local heroGuildComplexInsideHSP = quest:GetThingWithScriptName("HeroGuildComplexInsideHSP")
    hero18 = quest:GetHero()
    quest:EntityTeleportToThing(hero18, heroGuildComplexInsideHSP, false)
    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:FadeScreenOut(0.5, 0.0)
    quest:OpenDoor(secretBookcase)
    quest:SetThingPersistent(secretBookcase, true)
    quest:SetRegionExitAsActive(getNearestWithDefName2, true)
    local scratchValue2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_EndGuildXP)))
    quest:GiveHeroExperience(scratchValue2)
    quest:GiveHeroObject("OBJECT_HERO_BOOTS", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_TROUSERS", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_SHIRT", -1, true)
    quest:GiveHeroObject("OBJECT_HERO_GLOVES", -1, true)
    if not quest:NewScriptFrame() then return end
    quest:SetHeroAsWearing("OBJECT_HERO_BOOTS")
    quest:SetHeroAsWearing("OBJECT_HERO_TROUSERS")
    quest:SetHeroAsWearing("OBJECT_HERO_SHIRT")
    quest:SetHeroAsWearing("OBJECT_HERO_GLOVES")
    local skillApprentice9 = quest:GetThingWithScriptName("SkillApprentice")
    if skillApprentice9 ~= nil and skillApprentice9:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        local skillApprentice10 = quest:GetThingWithScriptName("SkillApprentice")
        quest:RemoveThing(skillApprentice10, false, true)
    end
    local willApprentice9 = quest:GetThingWithScriptName("WillApprentice")
    if willApprentice9 ~= nil and willApprentice9:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        local willApprentice10 = quest:GetThingWithScriptName("WillApprentice")
        quest:RemoveThing(willApprentice10, false, true)
    end
    local meleeApprentice11 = quest:GetThingWithScriptName("MeleeApprentice")
    quest:RemoveThing(meleeApprentice11, false, true)
    local combatApprentice7 = quest:GetThingWithScriptName("CombatApprentice")
    quest:RemoveThing(combatApprentice7, false, true)
    local birdKiller8 = quest:GetThingWithScriptName("BirdKiller")
    quest:RemoveThing(birdKiller8, false, true)
    RunSaveXPCutscene(quest)
    thing = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:SetIsPushableByHero(thing, false)
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_14", "HeroGuildComplexInside", "")
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 10)
    scratchValue = quest:MsgOnLeavingExperienceSpendingScreen()
    while not scratchValue do
        if not quest:NewScriptFrame() then goto LAB_00d496b3 end
        if quest:GetTimer(timerId) ~= 0 then
            scratchValue = quest:MsgOnLeavingExperienceSpendingScreen()
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d496b3 end
            local theRealGuildmaster2 = quest:GetThingWithScriptName("TheRealGuildmaster")
            local conversationId = quest:AddNewConversation(theRealGuildmaster2, false, false)
            local hero11 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero11)
            local hero12 = quest:GetHero()
            theRealGuildmaster4 = quest:GetThingWithScriptName("TheRealGuildmaster")
            quest:AddLineToConversation(conversationId, "TEXT_CS_028_LEAVING_TOUR_45", theRealGuildmaster4, hero12, false)
            quest:SetTimer(timerId, 10)
            scratchValue = quest:MsgOnLeavingExperienceSpendingScreen()
        end
    end
    if not quest:IsActiveThreadTerminating() then
        RunSaveXPCutscene2(quest)
        quest:FadeScreenOut(0.0, 0.5)
        local guildmasterMarker = quest:GetThingWithScriptName("M_GuildmasterMarker")
        theRealGuildmaster5 = quest:GetThingWithScriptName("TheRealGuildmaster")
        quest:EntityTeleportToThing(theRealGuildmaster5, guildmasterMarker, false)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00d496b3 end
        end
        if not quest:IsActiveThreadTerminating() then
            local hero13 = quest:GetHero()
            quest:EntityUnsetAsOpinionSource(hero13, false)
            quest:SetHeroAsApprentice(false)
            quest:GiveHeroExpression("EXPRESSION_FOLLOW", -1, true)
            quest:GiveHeroExpression("EXPRESSION_WAIT", -1, true)
            quest:SetThingAsUsable(guildDoors, true)
            quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)
            local theRealGuildmaster3 = quest:GetThingWithScriptName("TheRealGuildmaster")
            quest:RemoveThing(theRealGuildmaster3, false, true)
            local preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
            quest:RemoveThing(preMeleeMaze, false, true)
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
                                local hero14 = quest:GetHero()
                                getNearestWithDefName = quest:GetNearestWithDefName(hero14, "VILLAGE_GUILD_COMPLEX_INSIDE")
                                quest:EnableGuards(getNearestWithDefName, true)
                                quest:SetHeroSleepingAsEnabled(true)
                                local hero15 = quest:GetHero()
                                if quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero15) then
                                    repeat
                                        if not quest:NewScriptFrame() then goto LAB_00d496aa end
                                        quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                                        hero16 = quest:GetHero()
                                    until not quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero16)
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
                                        until not (scratchValue9 < (#appleRed01))
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        getActiveQuestName = quest:GetActiveQuestName()
                                        quest:SetQuestAsCompleted(getActiveQuestName, true, false, false)
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
                                                            if not (not quest:DisplayTutorial(TUTORIAL_CATEGORY_TAKING_QUESTS)) then
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    while not quest:MsgIsTutorialClickedPast() do
                                                                        if not quest:NewScriptFrame() then goto LAB_00d496a1 end
                                                                    end
                                                                    if not quest:IsActiveThreadTerminating() then goto LAB_00d4967d end
                                                                end
                                                                goto FLOW_past_lab_00d4967d
                                                            end
                                                            ::LAB_00d4967d::
                                                            getActiveQuestName2 = quest:GetActiveQuestName()
                                                            quest:DeactivateQuestLater(getActiveQuestName2, 0)
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
    do return end
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource4)
end

-- Q_GuildTraining.CheckFriendlyAttacks (retail 0x00d45060)
function CheckFriendlyAttacks(quest)
    local resources = quest:RetailResources()
    local movie, actorMap, predicateResult2, predicateResult5, predicateResult6, controlAcquired
    local predicateResult, scratchValue, conversationId, scratchValue11, heroWarnings
    local conversationId2, hero4, hero6, hero7, hero8, hero12, resource, preMeleeMaze
    local scratchValue22, scratchValue23, resource2
    preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
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
                if quest:IsActiveThreadTerminating() then return end
                creatures[scratchValue11 + 1]:SetFriendsWithEverythingFlag(true)
                quest:EntitySetAsKillable(creatures[scratchValue11 + 1], false, true)
            end
            scratchValue22 = scratchValue22 + 1
            scratchValue11 = scratchValue11 + 1
        until not (scratchValue22 < (#creatures))
    end
    if not quest:IsActiveThreadTerminating() then
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                preMeleeMaze = nil
                return
            end
            if not quest:IsLevelLoaded("HeroGuildComplex") then
                if quest:IsActiveThreadTerminating() then preMeleeMaze = nil; goto LAB_00d45322 end
                while not quest:IsLevelLoaded("HeroGuildComplex") do
                    if not quest:NewScriptFrame() then preMeleeMaze = nil; goto LAB_00d45322 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
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
                            if quest:IsActiveThreadTerminating() then return end
                            creatures2[scratchValue + 1]:SetFriendsWithEverythingFlag(true)
                            quest:EntitySetAsKillable(creatures2[scratchValue + 1], false, true)
                        end
                        scratchValue23 = scratchValue23 + 1
                        scratchValue = scratchValue + 1
                    until not (scratchValue23 < (#creatures2))
                end
                if quest:IsActiveThreadTerminating() then return end
            end
            local hero = quest:GetHero()
            if hero ~= nil and hero:MsgHitFriendWithBareHands() then
                goto LAB_00d456af
            else
                local hero2 = quest:GetHero()
                if hero2 ~= nil and hero2:MsgHitFriendWithMeleeWeapon() then goto LAB_00d456af end
                local hero3 = quest:GetHero()
                if hero3 ~= nil and hero3:MsgHitFriendWithRangedWeapon() then goto LAB_00d456af end
                goto LAB_00d45782
            end
            goto FLOW_past_lab_00d456af
            ::LAB_00d456af::
            if not quest:IsLevelLoaded("HeroGuildComplex") then goto LAB_00d45782 end
            if (preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) and preMeleeMaze:MsgIsHitByHero() then goto LAB_00d45782 end
            if ((preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) and preMeleeMaze:MsgIsHitByAnySpecialAbilityFromHero()) and (not (preMeleeMaze ~= nil and not preMeleeMaze:IsNull()) or not preMeleeMaze:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)) then goto LAB_00d45782 end
            predicateResult6 = true
            ::FLOW_past_lab_00d456af::
            goto FLOW_past_lab_00d45782
            ::LAB_00d45782::
            predicateResult6 = false
            ::FLOW_past_lab_00d45782::
            if not predicateResult6 then goto FLOW_past_lab_00d45930 end
            if quest:IsActiveThreadTerminating() then preMeleeMaze = nil; return end
            if 2 < quest:GetStateInt("HeroWarnings") then
                if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
                quest:SetMasterGameState("GuildWarningOccuring", true)
                if not quest:GetMasterGameState("SkillTestOccuring") and not quest:GetMasterGameState("WillTestOccuring") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, preMeleeMaze, 4) do
                        if not quest:NewScriptFrame() then goto LAB_00d45db2 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d45db2 end
                    resource2 = resources:NewResource()
                    resources:PrepareResource(resource2)
                    resource = resource2
                    hero4 = quest:GetHero()
                    controlAcquired = resources:TryAcquire(resource, hero4, 4)
                    while not controlAcquired do
                        if not quest:NewScriptFrame() then goto LAB_00d45da9 end
                        resource = resource2
                        local hero5 = quest:GetHero()
                        controlAcquired = resources:TryAcquire(resource, hero5, 4)
                    end
                    if not (quest:IsActiveThreadTerminating()) then
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource2)
                        resources:SetActor(actorMap, "MAZE", resource3)
                        movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        hero6 = quest:GetHero()
                        conversationId2 = quest:AddNewConversation(hero6, false, false)
                        hero7 = quest:GetHero()
                        hero12 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", hero12, hero7, false)
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
                    end
                    ::LAB_00d45da9::
                    resources:ReleaseResource(resource2)
                    ::LAB_00d45db2::
                    resources:ReleaseResource(resource3)
                    return
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
                while quest:GetMasterGameState("SkillTestOccuring") or quest:GetMasterGameState("WillTestOccuring") do
                    if not quest:NewScriptFrame() then return end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
                ::LAB_00d45c9d::
                quest:SetStateInt("HeroWarnings", 0)
                quest:SetMasterGameState("GuildWarningOccuring", false)
                goto FLOW_past_lab_00d45930
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d45322 end
            hero8 = quest:GetHero()
            conversationId = quest:AddNewConversation(hero8, false, false)
            heroWarnings = quest:GetStateInt("HeroWarnings")
            if heroWarnings == 0 then
                local hero9 = quest:GetHero()
                local hero13 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", hero13, hero9, false)
                goto LAB_00d45930
            elseif heroWarnings == 1 then
                local hero10 = quest:GetHero()
                local hero14 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", hero14, hero10, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                if heroWarnings ~= 2 then goto LAB_00d45930 end
                local hero11 = quest:GetHero()
                local hero15 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", hero15, hero11, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            goto FLOW_past_lab_00d45930
            ::LAB_00d45930::
            quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            ::FLOW_past_lab_00d45930::
            quest:NewScriptFrame()
            predicateResult2 = quest:IsActiveThreadTerminating()
        until false
    end
    preMeleeMaze = nil
    ::LAB_00d45322::
    do return end
    ::LAB_00d452d1::
    preMeleeMaze = nil
    goto LAB_00d45322
end

-- Q_GuildTraining.KeepTabsOnWhisper (retail 0x00d3cbd0)
function KeepTabsOnWhisper(quest)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not ((not quest:IsLevelLoaded("GuildWoods")) or (not quest:IsQuestActive("Q_GuildTrainingWoodsWill"))) then
            if quest:IsActiveThreadTerminating() then return end
            while not quest:IsLevelLoaded("HeroGuildComplex") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                if quest:IsActiveThreadTerminating() then return end
                local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                if meleeApprentice ~= nil and meleeApprentice:IsAlive() then
                    if quest:IsActiveThreadTerminating() then return end
                    local meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
                    quest:RemoveThing(meleeApprentice2, false, true)
                    while quest:IsQuestActive("Q_GuildTrainingWoodsWill") do
                        if not quest:NewScriptFrame() then return end
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:IsActiveThreadTerminating() then return end
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then return end
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then return end
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then return end
                    while not quest:IsLevelLoaded("HeroGuildComplex") do
                        if not quest:NewScriptFrame() then return end
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    local pPosition = quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos()
                    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pPosition, "MeleeApprentice", false)
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
    quest:IsActiveThreadTerminating()
end

-- Q_GuildTraining.KeepBookcaseExitRemoved (retail 0x00d3c8f0)
function KeepBookcaseExitRemoved(quest)
    local secretBookcase, getNearestWithDefName
    secretBookcase = quest:GetThingWithScriptName("SecretBookcase")
    getNearestWithDefName = quest:GetNearestWithDefName(secretBookcase, "REGION_EXIT_POINT")
    while quest:GetStateInt("GameState") ~= 9 do
        if not quest:NewScriptFrame() then return end
        while quest:IsLevelLoaded("HeroGuildComplex") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                getNearestWithDefName = nil
                secretBookcase = nil
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsLevelLoaded("HeroGuildComplex") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                getNearestWithDefName = nil
                secretBookcase = nil
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetStateInt("GameState") == 9 then break end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetRegionExitAsActive(getNearestWithDefName, false)
    end
    quest:IsActiveThreadTerminating()
end

-- Q_GuildTraining.RunArrivalCutscene (retail 0x00d44cb0)
function RunArrivalCutscene(quest)
    local resources = quest:RetailResources()
    local controlAcquired
    quest:SetTimeOfDay(19.0)
    if not quest:NewScriptFrame() then return end
    local pPosition = quest:GetThingWithScriptName("MK_GTA_MAZE1"):GetPos()
    local rivalHeroMazeCutscene = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", pPosition, "CutsceneMaze", false)
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, rivalHeroMazeCutscene, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    local hero = quest:GetHero()
    controlAcquired = resources:TryAcquire(resource, hero, 4)
    while not controlAcquired do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "MAZE", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
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
    resources:ReleaseResource(resource)
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
    local resources = quest:RetailResources()
    local controlAcquired, movie, actorMap
    while not quest:IsLevelLoaded("FrescoDome") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local position = quest:GetThingWithScriptName("MK_GTC_WHISSTART"):GetPos()
    local rivalHeroWhisperApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", position, "Whisper", false)
    local position2 = quest:GetThingWithScriptName("MK_GTC_GMSTART"):GetPos()
    local guildkeeper = quest:CreateCreature("CREATURE_GUILDKEEPER", position2, "GM", false)
    local resource5 = resources:NewResource()
    resources:PrepareResource(resource5)
    while not resources:TryAcquire(resource5, rivalHeroWhisperApprentice, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource5); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource5); return end
    quest:EntitySetInFaction(rivalHeroWhisperApprentice, "FACTION_HERO")
    local resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, guildkeeper, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource4); resources:ReleaseResource(resource5); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); resources:ReleaseResource(resource5); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    local hero = quest:GetHero()
    controlAcquired = resources:TryAcquire(resource, hero, 4)
    while not controlAcquired do
        if not quest:NewScriptFrame() then goto LAB_00d4a234 end
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
    end
    if not (quest:IsActiveThreadTerminating()) then
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
    end
    ::LAB_00d4a234::
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource4)
    resources:ReleaseResource(resource5)
end

-- Q_GuildTraining.RunSaveXPCutscene (retail 0x00d496f0)
function RunSaveXPCutscene(quest)
    local resources = quest:RetailResources()
    local controlAcquired, theRealGuildmaster
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            theRealGuildmaster = nil
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    local hero = quest:GetHero()
    controlAcquired = resources:TryAcquire(resource, hero, 4)
    while not controlAcquired do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "GM", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_SAVEXP", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
end

-- Q_GuildTraining.RunSaveXPCutscene2 (retail 0x00d49a20)
function RunSaveXPCutscene2(quest)
    local resources = quest:RetailResources()
    local controlAcquired, theRealGuildmaster
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, theRealGuildmaster, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            theRealGuildmaster = nil
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    local hero = quest:GetHero()
    controlAcquired = resources:TryAcquire(resource, hero, 4)
    while not controlAcquired do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "GM", resource2)
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_SAVEXP2", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
end

