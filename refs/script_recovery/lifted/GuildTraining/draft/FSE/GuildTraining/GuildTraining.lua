-- Generated native draft: Q_GuildTraining. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar3, pCVar4, pCVar5, pCVar6, r1, uVar10
    local alive = true
    if quest:GetStateInt("GameState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:RemoveAllHeroWeapons()
        uVar10 = 0
        pCVar4 = quest:GetThingWithScriptName("GuildArrivalHSP")
        pCVar5 = quest:GetHero()
        quest:EntityTeleportToThing(pCVar5, pCVar4, (uVar10 ~= 0))
        bVar3 = quest:IsLevelLoaded("LookoutPoint")
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            bVar3 = quest:IsLevelLoaded("LookoutPoint")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_MELEE", "Q_GuildTraining", false)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = false
        pCVar6 = quest:GetActiveQuestName()
        quest:KickOffQuestStartScreen(pCVar6, bVar3, true)
        RunArrivalCutscene(quest)
        quest:FadeScreenOut(0.5, 0.0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
    end
    uVar10 = 0
    pCVar4 = quest:GetThingWithScriptName("GuildTrainingHSP")
    pCVar5 = quest:GetHero()
    quest:EntityTeleportToThing(pCVar5, pCVar4, (uVar10 ~= 0))
    pCVar4 = nil
    bVar3 = quest:IsLevelLoaded("HeroGuildComplex")
    while true do
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
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
                pCVar4 = quest:GetHero()
                r1 = quest:GetNearestWithDefName(pCVar4, "VILLAGE_GUILD_COMPLEX_INSIDE")
                quest:EnableGuards(r1, false)
                quest:CreateThread("RunTutorials")  -- native thread body 0x00D45DD0: lift it as function RunTutorials(quest)
                if not bVar3 then
                end
                quest:CreateThread("CheckFriendlyAttacks")  -- native thread body 0x00D45060: lift it as function CheckFriendlyAttacks(quest)
                quest:CreateThread("KeepTabsOnWhisper")  -- native thread body KeepTabsOnWhisper: lift it as function KeepTabsOnWhisper(quest)
                if not bVar3 then
                end
                quest:CreateThread("WatchForSparrowKilled")  -- native thread body 0x00D3C840: lift it as function WatchForSparrowKilled(quest)
                if not bVar3 then
                end
                quest:CreateThread("KeepBookcaseExitRemoved")  -- native thread body KeepBookcaseExitRemoved: lift it as function KeepBookcaseExitRemoved(quest)
                if not bVar3 then
                end
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        bVar3 = quest:IsLevelLoaded("HeroGuildComplex")
    end
end

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

function OnPersist(quest, context)
    local gameState = quest:GetStateInt("GameState") or 0
    gameState = quest:PersistTransferInt(context, "GameState", gameState)
    quest:SetStateInt("GameState", gameState)
    local currentBirdsKilled = quest:GetStateInt("CurrentBirdsKilled") or 0
    currentBirdsKilled = quest:PersistTransferInt(context, "CurrentBirdsKilled", currentBirdsKilled)
    quest:SetStateInt("CurrentBirdsKilled", currentBirdsKilled)
end

function RunTutorials(quest)
    local resources = quest:RetailResources()
    local angle, b3, bVar14, bVar2, elem_1, fret_0, iVar16, iVar9, pCVar12, pCVar3, pCVar4, pCVar6, pCVar7, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, thing, uVar15, uVar8, xStack_10, xStack_20, xStack_30, xStack_48, xStack_54, xStack_88
    local alive = true
    r1 = quest:GetThingWithScriptName("SecretBookcase")
    r2 = quest:GetNearestWithDefName(r1, "REGION_EXIT_POINT")
    quest:SetRegionExitAsActive(r2, false)
    quest:SetExperienceSpendingAsEnabled(false)
    quest:SetHeroSleepingAsEnabled(false)
    r3 = quest:GetThingWithScriptName("GuildDoors")
    pCVar3 = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:MiniMapAddMarker(pCVar3, "HUD_ORB_QUEST_CORE")
    pCVar3 = nil
    bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsDeparture", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
    end
    if quest:GetStateInt("GameState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:SetThingAsUsable(r3, false)
        quest:SetThingPersistent(r3, true)
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_GUILD_SEAL_1")
        end
        quest:SetStateInt("GameState", 1)
    end
    bVar2 = false
    pCVar3 = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    quest:SetTeleporterAsActive(pCVar3, bVar2)
    pCVar3 = quest:GetThingWithScriptName("MeleeOpponent")
    bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar14 = true
        bVar2 = false
        pCVar3 = quest:GetThingWithScriptName("MeleeOpponent")
        quest:RemoveThing(pCVar3, bVar2, bVar14)
    end
    if quest:GetStateInt("GameState") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:SetMasterGameState("ScorpionsDestroyed", false)
        quest:SetMasterGameState("ScorpionsDestroyedCutscenePlayed", false)
        quest:SetMasterGameState("SkillTrainingStarted", false)
        pCVar3 = quest:GetThingWithScriptName("PreMeleeDummy")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            pCVar3 = quest:GetThingWithScriptName("PreMeleeDummyMarker")
            pCVar4 = pCVar3:GetPos()
            r4 = quest:CreateObject("OBJECT_STRAW_DUMMY_01", pCVar4, "PreMeleeDummy")
            pCVar3 = quest:GetThingWithScriptName("PreMeleeDummyMarker")
            uVar15 = 1
            fret_0 = pCVar3:GetAngleXY()
            angle = fret_0
            pCVar3 = quest:GetThingWithScriptName("PreMeleeDummy")
            quest:EntitySetFacingAngle(pCVar3, angle, (uVar15 ~= 0))
        end
        pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("WillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("BirdKiller")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_HERO_STICK", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_HERO_STICK")
        end
        quest:SetCategoryActivity("Sweet Kid", true)
        bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingPreMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingPreMelee", false)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            xStack_54 = quest:GetAllThingsWithScriptName("AppleMarker")
            if #xStack_54 ~= 0 then
                iVar9 = 0
                uVar8 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d46c49 end
                    elem_1 = xStack_54[(iVar9) / 0xc + 1]
                    pCVar4 = elem_1:GetPos()
                    r5 = quest:CreateObject("OBJECT_APPLE_RED_01", pCVar4, "")
                    quest:SetThingPersistent(r5, true)
                    quest:SetThingPersistent(xStack_54[(iVar9) / 0xc + 1], true)
                    quest:RemoveThing(xStack_54[(iVar9) / 0xc + 1], false, true)
                    uVar8 = uVar8 + 1
                    iVar9 = iVar9 + 0xc
                until not (uVar8 < (#xStack_54))
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                goto LAB_00d46c49
            end
            goto FLOW_past_lab_00d46c49
            ::LAB_00d46c49::
            goto LAB_00d496bc
            ::FLOW_past_lab_00d46c49::
        end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
        end
        quest:SetCategoryActivity("Sweet Kid", false)
        quest:SetStateInt("GameState", 3)
        quest:GiveHeroGold(0x32)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        pCVar3 = quest:GetThingWithScriptName("PreMeleeDummy")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("PreMeleeDummy")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("PreMeleeWhisper")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("PreMeleeWhisper")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
    end
    if quest:GetStateInt("GameState") == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
        end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_IRON_KATANA", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_IRON_KATANA")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("WillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("BirdKiller")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        quest:SetCategoryActivity("Wannabe Hero", true)
        quest:SetCategoryActivity("Young Apprentice", true)
        quest:SetCategoryActivity("Young Annoyance", true)
        pCVar3 = quest:GetHero()
        r6 = quest:TurnCreatureInto(pCVar3, "CREATURE_HERO")
        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
        quest:GiveHeroExpression("EXPRESSION_FLIRT", -1, true)
        quest:GiveHeroExpression("EXPRESSION_COSSACK", -1, true)
        bVar2 = quest:IsXbox()
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1)
            quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1)
        end
        pCVar3 = quest:GetHero()
        quest:EntitySetAsOpinionSource(pCVar3, "OPINION_SOURCE_HERO_AS_APPRENTICE")
        quest:SetHeroAsTeenager(true)
        quest:SetHeroAsApprentice(true)
        quest:GiveHeroAbility(0xb, false)
        bVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingMelee")
            quest:SetQuestAsPersistent("Q_GuildTrainingMelee", false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar2 = quest:IsQuestActive("Q_GuildTrainingMelee")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:RemoveQuestCardFromGuild("Q_GuildTraining")
        quest:SetCategoryActivity("Complete Melee", true)
        quest:SetStateInt("GameState", 5)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            pCVar3 = quest:GetThingWithScriptName("CombatApprenticeMarker")
            bVar2 = false
            pCVar4 = pCVar3:GetPos()
            r7 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar4, "CombatApprentice")
            pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
            pCVar3:SetToKillOnLevelUnload(false)
        end
    end
    if quest:GetStateInt("GameState") == 5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
        end
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_YEW_CROSSBOW", pCVar3)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:TakeObjectFromHero("OBJECT_YEW_CROSSBOW")
        end
        quest:SetMasterGameState("SkillTrainingStarted", false)
        quest:SetMasterGameState("MovingDummiesNeeded", false)
        pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
        bVar2 = false
        pCVar4 = pCVar3:GetPos()
        r8 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pCVar4, "MeleeApprentice")
        pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("WillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("WillApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("BirdKiller")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingSkill")
            quest:SetQuestAsPersistent("Q_GuildTrainingSkill", false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar2 = quest:IsQuestActive("Q_GuildTrainingSkill")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:SetCategoryActivity("Complete Skill", true)
        quest:SetStateInt("GameState", 7)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("MovingDummiesNeeded", true)
        pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            pCVar3 = quest:GetThingWithScriptName("SkillApprenticeMarker")
            bVar2 = false
            pCVar4 = pCVar3:GetPos()
            r9 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar4, "SkillApprentice")
            pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
            pCVar3:SetToKillOnLevelUnload(false)
        end
        pCVar3 = quest:GetThingWithScriptName("BirdKiller")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            pCVar3 = quest:GetThingWithScriptName("BirdKillerMarker")
            bVar2 = false
            pCVar4 = pCVar3:GetPos()
            r10 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar4, "BirdKiller")
        end
    end
    if quest:GetStateInt("GameState") == 7 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:GiveHeroAbility(0xb, false)
        pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
        end
        pCVar3 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
        bVar2 = false
        pCVar4 = pCVar3:GetPos()
        r11 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pCVar4, "MeleeApprentice")
        bVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:ActivateQuest("Q_GuildTrainingWill")
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar2 = quest:IsQuestActive("Q_GuildTrainingWill")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:SetCategoryActivity("Complete Will", true)
        quest:SetStateInt("GameState", 9)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        pCVar3 = quest:GetThingWithScriptName("WillApprentice")
        bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            pCVar3 = quest:GetThingWithScriptName("WillApprenticeMarker")
            bVar2 = false
            pCVar4 = pCVar3:GetPos()
            r12 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar4, "WillApprentice")
            pCVar3 = quest:GetThingWithScriptName("WillApprentice")
            pCVar3:SetToKillOnLevelUnload(false)
        end
    end
    pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
    bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar14 = true
        bVar2 = false
        pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(pCVar3, bVar2, bVar14)
    end
    pCVar3 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    bVar2 = false
    pCVar4 = pCVar3:GetPos()
    r13 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", pCVar4, "MeleeApprentice")
    bVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        quest:ActivateQuest("Q_GuildTrainingDeparture")
        quest:SetQuestAsPersistent("Q_GuildTrainingDeparture", false)
    end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d496bc end
    bVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
    if bVar2 then
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            bVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        until not (bVar2)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d496bc end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_13", "HeroGuildComplexInside", "")
    bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d496bc end
        bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d496bc end
    r14 = quest:GetThingWithScriptName("TheRealGuildmaster")
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar2 = resources:TryAcquire(xStack_30, r14, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d48800 end
        bVar2 = resources:TryAcquire(xStack_30, r14, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        iVar16 = 4
        pCVar12 = xStack_20
        pCVar3 = quest:GetHero()
        bVar2 = resources:TryAcquire(pCVar12, pCVar3, iVar16)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d487f4 end
            iVar16 = 4
            pCVar12 = xStack_20
            pCVar3 = quest:GetHero()
            bVar2 = resources:TryAcquire(pCVar12, pCVar3, iVar16)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            r13 = resources:NewActorMap()
            resources:SetActor(r13, "HERO", xStack_20)
            resources:SetActor(r13, "GM", xStack_30)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_DEPARTURE_EXIT_WOODS", r13, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(r13)
            resources:ReleaseResource(xStack_20)
            resources:ReleaseResource(xStack_30)
            uVar15 = 0
            pCVar3 = quest:GetThingWithScriptName("FrescoDomeHSP")
            pCVar6 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar6, pCVar3, (uVar15 ~= 0))
            quest:SetStateBool("DomeCutsceneStart", true)
            RunCeremonyCutscene(quest)
            quest:GiveHeroObject("OBJECT_GUILD_SEAL_1", -1)
            quest:SetStateBool("DomeCutsceneStart", false)
            uVar15 = 0
            pCVar3 = quest:GetThingWithScriptName("HeroGuildComplexInsideHSP")
            pCVar6 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar6, pCVar3, (uVar15 ~= 0))
            bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:FadeScreenOut(0.5, 0.0)
            quest:OpenDoor(r1)
            quest:SetThingPersistent(r1, true)
            quest:SetRegionExitAsActive(r2, true)
            iVar16 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf1c)))
            quest:GiveHeroExperience(iVar16)
            quest:GiveHeroObject("OBJECT_HERO_BOOTS", -1)
            quest:GiveHeroObject("OBJECT_HERO_TROUSERS", -1)
            quest:GiveHeroObject("OBJECT_HERO_SHIRT", -1)
            quest:GiveHeroObject("OBJECT_HERO_GLOVES", -1)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d496bc end
            quest:SetHeroAsWearing("OBJECT_HERO_BOOTS")
            quest:SetHeroAsWearing("OBJECT_HERO_TROUSERS")
            quest:SetHeroAsWearing("OBJECT_HERO_SHIRT")
            quest:SetHeroAsWearing("OBJECT_HERO_GLOVES")
            pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
            bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar14 = true
                bVar2 = false
                pCVar3 = quest:GetThingWithScriptName("SkillApprentice")
                quest:RemoveThing(pCVar3, bVar2, bVar14)
            end
            pCVar3 = quest:GetThingWithScriptName("WillApprentice")
            bVar2 = (pCVar3 ~= nil and pCVar3:IsAlive())
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496bc end
                bVar14 = true
                bVar2 = false
                pCVar3 = quest:GetThingWithScriptName("WillApprentice")
                quest:RemoveThing(pCVar3, bVar2, bVar14)
            end
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("MeleeApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("CombatApprentice")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
            bVar14 = true
            bVar2 = false
            pCVar3 = quest:GetThingWithScriptName("BirdKiller")
            quest:RemoveThing(pCVar3, bVar2, bVar14)
            -- TODO(native): NScript::CQ_GuildTrainingScript::RunSaveXPCutscene2__atd496f0(this);
            bVar2 = false
            thing = quest:GetThingWithScriptName("TheRealGuildmaster")
            quest:SetIsPushableByHero(thing, bVar2)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_14", "HeroGuildComplexInside", "")
            xStack_88 = quest:RegisterTimer()
            quest:SetTimer(xStack_88, 10)
            bVar2 = quest:MsgOnLeavingExperienceSpendingScreen()
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d496b3 end
                iVar9 = quest:GetTimer(xStack_88)
                if iVar9 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d496b3 end
                    bVar14 = false
                    bVar2 = false
                    pCVar3 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    iVar16 = quest:AddNewConversation(pCVar3, bVar2, bVar14)
                    pCVar3 = quest:GetHero()
                    quest:AddPersonToConversation(iVar16, pCVar3)
                    pCVar3 = quest:GetHero()
                    pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    quest:AddLineToConversation(iVar16, "TEXT_CS_028_LEAVING_TOUR_45", pCVar6, pCVar3, false)
                    quest:SetTimer(xStack_88, 10)
                end
                bVar2 = quest:MsgOnLeavingExperienceSpendingScreen()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                -- TODO(native): NScript::CQ_GuildTrainingScript::RunSaveXPCutscene2__atd49a20(this);
                quest:FadeScreenOut(0.0, 0.5)
                uVar15 = 0
                pCVar3 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                quest:EntityTeleportToThing(pCVar6, pCVar3, (uVar15 ~= 0))
                bVar2 = quest:IsHeroControlledByPlayer()
                while not bVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d496b3 end
                    bVar2 = quest:IsHeroControlledByPlayer()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    bVar2 = false
                    pCVar3 = quest:GetHero()
                    quest:EntityUnsetAsOpinionSource(pCVar3, bVar2)
                    quest:SetHeroAsApprentice(false)
                    quest:GiveHeroExpression("EXPRESSION_FOLLOW", -1, true)
                    quest:GiveHeroExpression("EXPRESSION_WAIT", -1, true)
                    quest:SetThingAsUsable(r3, true)
                    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)
                    bVar14 = true
                    bVar2 = false
                    pCVar3 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    quest:RemoveThing(pCVar3, bVar2, bVar14)
                    bVar14 = true
                    bVar2 = false
                    pCVar3 = quest:GetThingWithScriptName("PreMeleeMaze")
                    quest:RemoveThing(pCVar3, bVar2, bVar14)
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        quest:FadeScreenIn()
                                        quest:SetTimeOfDay(10.0)
                                        quest:SetWeaponOutCrimeEnabled(true)
                                        quest:SetGuardsIgnoreCrimes(false)
                                        pCVar3 = quest:GetHero()
                                        r15 = quest:GetNearestWithDefName(pCVar3, "VILLAGE_GUILD_COMPLEX_INSIDE")
                                        quest:EnableGuards(r15, true)
                                        quest:SetHeroSleepingAsEnabled(true)
                                        pCVar3 = quest:GetHero()
                                        bVar2 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", pCVar3)
                                        if bVar2 then
                                            repeat
                                                alive = quest:NewScriptFrame()
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if bVar2 then goto LAB_00d496aa end
                                                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                                                pCVar3 = quest:GetHero()
                                                bVar2 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", pCVar3)
                                            until not (bVar2)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if not bVar2 then
                                            xStack_48 = quest:GetAllThingsWithDefName("OBJECT_APPLE_RED_01")
                                            uVar8 = 0
                                            if #xStack_48 ~= 0 then
                                                iVar9 = 0
                                                repeat
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar2 = not alive
                                                    if bVar2 then goto LAB_00d496a1 end
                                                    quest:RemoveThing(xStack_48[(iVar9) / 0xc + 1], false, true)
                                                    uVar8 = uVar8 + 1
                                                    iVar9 = iVar9 + 0xc
                                                until not (uVar8 < (#xStack_48))
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if not bVar2 then
                                                b3 = false
                                                bVar14 = false
                                                bVar2 = true
                                                pCVar7 = quest:GetActiveQuestName()
                                                quest:SetQuestAsCompleted(pCVar7, bVar2, bVar14, b3)
                                                alive = quest:NewScriptFrame()
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if not bVar2 then
                                                    alive = quest:NewScriptFrame()
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar2 = not alive
                                                    if not bVar2 then
                                                        alive = quest:NewScriptFrame()
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar2 = not alive
                                                        if not bVar2 then
                                                            alive = quest:NewScriptFrame()
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar2 = not alive
                                                            if not bVar2 then
                                                                alive = quest:NewScriptFrame()
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar2 = not alive
                                                                if not bVar2 then
                                                                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_THEGUILD")
                                                                    bVar2 = quest:DisplayTutorial(0x1e)
                                                                    if bVar2 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar2 = not alive
                                                                        if not bVar2 then
                                                                            bVar2 = quest:MsgIsTutorialClickedPast()
                                                                            while not bVar2 do
                                                                                alive = quest:NewScriptFrame()
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar2 = not alive
                                                                                if bVar2 then goto LAB_00d496a1 end
                                                                                bVar2 = quest:MsgIsTutorialClickedPast()
                                                                            end
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar2 = not alive
                                                                            if not bVar2 then goto LAB_00d4967d end
                                                                        end
                                                                    else
                                                                        goto LAB_00d4967d
                                                                    end
                                                                    goto FLOW_past_lab_00d4967d
                                                                    ::LAB_00d4967d::
                                                                    uVar8 = 0
                                                                    pCVar7 = quest:GetActiveQuestName()
                                                                    quest:DeactivateQuestLater(pCVar7, uVar8)
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
            quest:DeregisterTimer(xStack_88)
            goto LAB_00d496bc
        end
        ::LAB_00d487f4::
        resources:ReleaseResource(xStack_20)
    end
    ::LAB_00d48800::
    resources:ReleaseResource(xStack_30)
    ::LAB_00d496bc::
end

function CheckFriendlyAttacks(quest)
    local resources = quest:RetailResources()
    local aCStack_10, amStack_1c, bVar10, bVar2, bVar20, bVar3, bVar4, bVar5, bVar6, bVar9, cVar11, elem_1, elem_2, iVar13, iVar18, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, pCVar14, pCVar15, pCVar19, pcVar21, pvVar12, r1, uStack_9c, uVar17, xStack_20, xStack_30, xStack_80, xStack_84, x_stk_a0
    local alive = true
    bVar4 = false
    bVar3 = false
    bVar2 = false
    r1 = quest:GetThingWithScriptName("PreMeleeMaze")
    xStack_84 = quest:GetAllCreaturesExcludingHero()
    uVar17 = 0
    if #xStack_84 ~= 0 then
        iVar18 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then goto LAB_00d452d1 end
            pcVar21 = "CREATURE_BIRD_GUILD_SPARROW"
            pvVar12 = xStack_84[(iVar18) / 0xc + 1]:GetDefName()
            iVar13 = ((pvVar12 ~= pcVar21) and 1 or 0)
            if iVar13 == 0 then
                goto LAB_00d45184
            else
                pcVar21 = "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                bVar4 = true
                pvVar12 = xStack_84[(iVar18) / 0xc + 1]:GetDefName()
                iVar13 = ((pvVar12 ~= pcVar21) and 1 or 0)
                if iVar13 == 0 then goto LAB_00d45184 end
                pcVar21 = "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                bVar4 = true
                bVar3 = true
                pvVar12 = xStack_84[(iVar18) / 0xc + 1]:GetDefName()
                iVar13 = ((pvVar12 ~= pcVar21) and 1 or 0)
                if iVar13 == 0 then goto LAB_00d45184 end
                pcVar21 = "CREATURE_RIVAL_HERO_MAZE"
                bVar4 = true
                bVar3 = true
                bVar2 = true
                pvVar12 = xStack_84[(iVar18) / 0xc + 1]:GetDefName()
                iVar13 = ((pvVar12 ~= pcVar21) and 1 or 0)
                bVar9 = true
                if iVar13 == 0 then goto LAB_00d45184 end
            end
            goto FLOW_past_lab_00d45184
            ::LAB_00d45184::
            bVar9 = false
            ::FLOW_past_lab_00d45184::
            if bVar2 then
                bVar2 = false
            end
            if bVar3 then
                bVar3 = false
            end
            if bVar4 then
                bVar4 = false
            end
            if bVar9 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then return end
                xStack_84[(iVar18) / 0xc + 1]:SetFriendsWithEverythingFlag(1)
                quest:EntitySetAsKillable(xStack_84[(iVar18) / 0xc + 1], false, true)
            end
            uVar17 = uVar17 + 1
            iVar18 = iVar18 + 0xc
        until not (uVar17 < (#xStack_84))
    end
    bVar6 = false
    bVar5 = false
    bVar9 = false
    bVar4 = false
    bVar3 = false
    bVar2 = false
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if not bVar10 then
        alive = not quest:IsActiveThreadTerminating()
        bVar10 = not alive
        repeat
            if bVar10 then
                r1 = nil
                -- LAB_00d45d42: (native jump target)
                return
            end
            bVar10 = quest:IsLevelLoaded("HeroGuildComplex")
            if not bVar10 then
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then
                    r1 = nil
                    goto LAB_00d45322
                end
                bVar10 = quest:IsLevelLoaded("HeroGuildComplex")
                while not bVar10 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then
                        r1 = nil
                        goto LAB_00d45322
                    end
                    bVar10 = quest:IsLevelLoaded("HeroGuildComplex")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00d45322 end
                iVar13 = 0
                xStack_84 = quest:GetAllCreaturesExcludingHero()
                uVar17 = 0
                if #xStack_84 ~= 0 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then
                            return
                        end
                        pcVar21 = "CREATURE_BIRD_GUILD_SPARROW"
                        elem_1 = xStack_84[(iVar13) / 0xc + 1]
                        pvVar12 = elem_1:GetDefName()
                        iVar18 = ((pvVar12 ~= pcVar21) and 1 or 0)
                        if iVar18 == 0 then
                            goto LAB_00d4557d
                        else
                            pcVar21 = "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
                            bVar3 = true
                            elem_2 = xStack_84[(iVar13) / 0xc + 1]
                            pvVar12 = elem_2:GetDefName()
                            iVar18 = ((pvVar12 ~= pcVar21) and 1 or 0)
                            if iVar18 == 0 then goto LAB_00d4557d end
                            pcVar21 = "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE"
                            bVar3 = true
                            bVar2 = true
                            pvVar12 = xStack_84[(iVar13) / 0xc + 1]:GetDefName()
                            iVar18 = ((pvVar12 ~= pcVar21) and 1 or 0)
                            if iVar18 == 0 then goto LAB_00d4557d end
                            pcVar21 = "CREATURE_RIVAL_HERO_MAZE"
                            bVar3 = true
                            bVar2 = true
                            pvVar12 = xStack_84[(iVar13) / 0xc + 1]:GetDefName()
                            iVar18 = ((pvVar12 ~= pcVar21) and 1 or 0)
                            bVar10 = true
                            if iVar18 == 0 then goto LAB_00d4557d end
                        end
                        goto FLOW_past_lab_00d4557d
                        ::LAB_00d4557d::
                        bVar10 = false
                        ::FLOW_past_lab_00d4557d::
                        if bVar2 then
                            bVar2 = false
                        end
                        if bVar3 then
                            bVar3 = false
                        end
                        if bVar10 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then
                                return
                            end
                            xStack_84[(iVar13) / 0xc + 1]:SetFriendsWithEverythingFlag(1)
                            quest:EntitySetAsKillable(xStack_84[(iVar13) / 0xc + 1], false, true)
                        end
                        uVar17 = uVar17 + 1
                        iVar13 = iVar13 + 0xc
                    until not (uVar17 < (#xStack_84))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then
                    return
                end
            end
            pCVar14 = quest:GetHero()
            bVar10 = pCVar14:MsgHitFriendWithBareHands()
            if bVar10 then
                goto LAB_00d456af
            else
                pCVar14 = quest:GetHero()
                bVar10 = pCVar14:MsgHitFriendWithMeleeWeapon()
                if bVar10 then goto LAB_00d456af end
                pCVar14 = quest:GetHero()
                bVar10 = pCVar14:MsgHitFriendWithRangedWeapon()
                if bVar10 then goto LAB_00d456af end
                goto LAB_00d45782
            end
            goto FLOW_past_lab_00d456af
            ::LAB_00d456af::
            bVar6 = true
            bVar10 = quest:IsLevelLoaded("HeroGuildComplex")
            if not bVar10 then goto LAB_00d45782 end
            bVar6 = true
            bVar5 = true
            native_arg_sequence_1 = false
            if uStack_9c ~= nil then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                -- TODO(native): cVar11 = (**(*uStack_9c + 0x54))("SCRIPT_NAME_HERO")
                cVar11 = nil --[[unresolved native value]]
                if cVar11 ~= 0 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d45782 end
            native_arg_sequence_2 = false
            if uStack_9c ~= nil then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                -- TODO(native): cVar11 = (**(*uStack_9c + 0xa8))("SCRIPT_NAME_HERO")
                cVar11 = nil --[[unresolved native value]]
                if cVar11 ~= 0 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
            if native_arg_sequence_2 then
                bVar6 = true
                bVar5 = true
                bVar9 = true
                bVar4 = true
                native_arg_sequence_3 = false
                if uStack_9c == nil then
                    native_arg_sequence_3 = true
                else
                    native_arg_sequence_3 = false
                end
                if not native_arg_sequence_3 then
                    -- TODO(native): cVar11 = (**(*uStack_9c + 0xa4))(0xe,"SCRIPT_NAME_HERO")
                    cVar11 = nil --[[unresolved native value]]
                    if cVar11 == 0 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                end
                if native_arg_sequence_3 then goto LAB_00d45782 end
            end
            bVar6 = true
            bVar5 = true
            bVar9 = true
            bVar10 = true
            ::FLOW_past_lab_00d456af::
            goto FLOW_past_lab_00d45782
            ::LAB_00d45782::
            bVar10 = false
            ::FLOW_past_lab_00d45782::
            if bVar4 then
                bVar4 = false
            end
            if bVar9 then
                bVar9 = false
            end
            if bVar5 then
                bVar5 = false
            end
            if bVar6 then
                bVar6 = false
            end
            if not bVar10 then goto LAB_00d45cae end
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then
                xStack_80 = nil
                return
            end
            if 2 < quest:GetStateInt("HeroWarnings") then
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00d45322 end
                quest:SetMasterGameState("GuildWarningOccuring", true)
                if (not quest:GetMasterGameState("SkillTestOccuring")) and (not quest:GetMasterGameState("WillTestOccuring")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then goto LAB_00d45322 end
                    xStack_30 = resources:NewResource()
                    resources:PrepareResource(xStack_30)
                    bVar10 = resources:TryAcquire(xStack_30, xStack_80, 4)
                    while not bVar10 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00d45db2 end
                        bVar10 = resources:TryAcquire(xStack_30, xStack_80, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if not bVar10 then
                        xStack_20 = resources:NewResource()
                        resources:PrepareResource(xStack_20)
                        iVar13 = 4
                        pCVar19 = xStack_20
                        pCVar14 = quest:GetHero()
                        bVar10 = resources:TryAcquire(pCVar19, pCVar14, iVar13)
                        while not bVar10 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then goto LAB_00d45da9 end
                            iVar13 = 4
                            pCVar19 = xStack_20
                            pCVar14 = quest:GetHero()
                            bVar10 = resources:TryAcquire(pCVar19, pCVar14, iVar13)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if not bVar10 then
                            amStack_1c = resources:NewActorMap()
                            -- TODO(native): resources:SetActor(amStack_1c, "HERO", &xStack_20)
                            resources:SetActor(amStack_1c, "MAZE", xStack_30)
                            aCStack_10 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            bVar20 = false
                            bVar10 = false
                            pCVar14 = quest:GetHero()
                            xStack_30 = quest:AddNewConversation(pCVar14, bVar10, bVar20)
                            pCVar14 = quest:GetHero()
                            pCVar15 = quest:GetHero()
                            quest:AddLineToConversation(xStack_30, "TEXT_QST_028_GUILD_SEAL_FOURTH_WARNING", pCVar15, pCVar14, false)
                            quest:Pause(2.0)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_BADHERO", amStack_1c, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(aCStack_10)
                            resources:DestroyActorMap(amStack_1c)
                            resources:ReleaseResource(amStack_1c)
                            resources:ReleaseResource(xStack_30)
                            goto LAB_00d45c9d
                        end
                        ::LAB_00d45da9::
                        resources:ReleaseResource(xStack_20)
                    end
                    ::LAB_00d45db2::
                    resources:ReleaseResource(xStack_30)
                    -- LAB_00d45dbb: (native jump target)
                    return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00d45322 end
                while (quest:GetMasterGameState("SkillTestOccuring") or (quest:GetMasterGameState("WillTestOccuring"))) do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then
                        return
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00d45322 end
                ::LAB_00d45c9d::
                quest:SetStateInt("HeroWarnings", 0)
                quest:SetMasterGameState("GuildWarningOccuring", false)
                goto LAB_00d45cae
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then goto LAB_00d45322 end
            bVar20 = false
            bVar10 = false
            pCVar14 = quest:GetHero()
            iVar13 = quest:AddNewConversation(pCVar14, bVar10, bVar20)
            iVar18 = quest:GetStateInt("HeroWarnings")
            if iVar18 == 0 then
                pCVar14 = quest:GetHero()
                pCVar15 = quest:GetHero()
                quest:AddLineToConversation(iVar13, "TEXT_QST_028_GUILD_SEAL_FIRST_WARNING", pCVar15, pCVar14, false)
                goto LAB_00d45930
            elseif iVar18 == 1 then
                pCVar14 = quest:GetHero()
                pCVar15 = quest:GetHero()
                quest:AddLineToConversation(iVar13, "TEXT_QST_028_GUILD_SEAL_SECOND_WARNING", pCVar15, pCVar14, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            else
                if iVar18 ~= 2 then goto LAB_00d45930 end
                pCVar14 = quest:GetHero()
                pCVar15 = quest:GetHero()
                quest:AddLineToConversation(iVar13, "TEXT_QST_028_GUILD_SEAL_THIRD_WARNING", pCVar15, pCVar14, false)
                quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            end
            goto FLOW_past_lab_00d45930
            ::LAB_00d45930::
            quest:SetStateInt("HeroWarnings", quest:GetStateInt("HeroWarnings") + 1)
            ::FLOW_past_lab_00d45930::
            ::LAB_00d45cae::
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
        until false
    end
    r1 = nil
    ::LAB_00d45322::
    do return end
    ::LAB_00d452d1::
    x_stk_a0 = nil
    goto LAB_00d45322
end

function KeepTabsOnWhisper(quest)
    local CVar5, CVar6, bVar2, bVar3, pCVar4, pPosition, r1, xStack_2c
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    CVar5 = 0
    repeat
        CVar6 = CVar5 | 1
        xStack_2c = CVar6
        bVar2 = quest:IsLevelLoaded("GuildWoods")
        if bVar2 then
            CVar6 = CVar5 | 3
            xStack_2c = CVar6
            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
            bVar2 = true
            if not bVar3 then goto LAB_00d3cc48 end
        else
            goto LAB_00d3cc48
        end
        goto FLOW_past_lab_00d3cc48
        ::LAB_00d3cc48::
        bVar2 = false
        ::FLOW_past_lab_00d3cc48::
        if (CVar6 & 2) ~= 0 then
            CVar6 = CVar6 & 0xfffffffd
            xStack_2c = CVar6
        end
        if (CVar6 & 1) ~= 0 then
            CVar6 = CVar6 & 0xfffffffe
            xStack_2c = CVar6
        end
        CVar5 = CVar6
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
            CVar5 = xStack_2c
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pCVar4 = quest:GetThingWithScriptName("MeleeApprentice")
                bVar2 = (pCVar4 ~= nil and pCVar4:IsAlive())
                CVar5 = xStack_2c
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    bVar3 = true
                    bVar2 = false
                    pCVar4 = quest:GetThingWithScriptName("MeleeApprentice")
                    quest:RemoveThing(pCVar4, bVar2, bVar3)
                    bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                    if bVar2 then
                        repeat
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                            bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                        until not (bVar2)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        bVar2 = quest:IsQuestActive("Q_GuildTrainingDeparture")
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    alive = quest:NewScriptFrame()
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
                                        while not bVar2 do
                                            alive = quest:NewScriptFrame()
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then
                                                return
                                            end
                                            bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if not bVar2 then
                                            pCVar4 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
                                            bVar2 = false
                                            pPosition = pCVar4:GetPos()
                                            r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", pPosition, "MeleeApprentice")
                                        end
                                    end
                                end
                            end
                        end
                    end
                    return
                end
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    until false
end

function WatchForSparrowKilled(quest)
    local bVar2
    local alive = true
    local cVar1 = quest:GetStateBool("DisplayBirdKilledMessage")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar1 = quest:GetStateBool("DisplayBirdKilledMessage")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:DisplayGameInfo("TEXT_QST_028_HERO_KILL_BIRD")
        bVar2 = quest:MsgIsGameInfoClickedPast()
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
    end
end

function KeepBookcaseExitRemoved(quest)
    local bVar2, iVar1, r1, r2
    local alive = true
    r1 = quest:GetThingWithScriptName("SecretBookcase")
    r2 = quest:GetNearestWithDefName(r1, "REGION_EXIT_POINT")
    iVar1 = quest:GetStateInt("GameState")
    while iVar1 ~= 9 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    r2 = nil
                    r1 = nil
                    return
                end
                bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                r2 = nil
                r1 = nil
                -- LAB_00d3cb49: (native jump target)
                return
            end
            bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        if quest:GetStateInt("GameState") == 9 then break end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3cac6 end
        quest:SetRegionExitAsActive(r2, false)
        iVar1 = quest:GetStateInt("GameState")
    end
    alive = not quest:IsActiveThreadTerminating()
    ::LAB_00d3cac6::
end

function RunArrivalCutscene(quest)
    local resources = quest:RetailResources()
    local bVar3, iVar7, pCVar4, pPosition, pppuVar6, r1, xStack_10, xStack_20, xStack_30
    local alive = true
    quest:SetTimeOfDay(19.0)
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    pCVar4 = quest:GetThingWithScriptName("MK_GTA_MAZE1")
    bVar3 = false
    pPosition = pCVar4:GetPos()
    r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", pPosition, "CutsceneMaze")
    pCVar4 = nil
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar3 = resources:TryAcquire(xStack_30, r1, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d44ec4 end
        bVar3 = resources:TryAcquire(xStack_30, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        iVar7 = 4
        pppuVar6 = xStack_20
        pCVar4 = quest:GetHero()
        bVar3 = resources:TryAcquire(pppuVar6, pCVar4, iVar7)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d44ebb end
            iVar7 = 4
            pppuVar6 = xStack_20
            pCVar4 = quest:GetHero()
            bVar3 = resources:TryAcquire(pppuVar6, pCVar4, iVar7)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pCVar4 = resources:NewActorMap()
            resources:SetActor(pCVar4, "MAZE", xStack_30)
            resources:SetActor(pCVar4, "HERO", xStack_20)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_ARRIVE", pCVar4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:FadeScreenOut(0.5, 0.0)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:SetRegionTextDisplayAsActive(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(pCVar4)
            resources:ReleaseResource(xStack_20)
            resources:ReleaseResource(xStack_30)
            quest:RemoveThing(r1, false, true)
            return
        end
        ::LAB_00d44ebb::
        resources:ReleaseResource(xStack_20)
    end
    ::LAB_00d44ec4::
    resources:ReleaseResource(xStack_30)
end

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

function RunCeremonyCutscene(quest)
    local resources = quest:RetailResources()
    local bVar2, iVar9, pCVar4, pCVar5, pCVar8, r1, r2, xStack_10, xStack_20, xStack_30, xStack_40, xStack_58
    local alive = true
    bVar2 = quest:IsLevelLoaded("FrescoDome")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsLevelLoaded("FrescoDome")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        pCVar4 = quest:GetThingWithScriptName("MK_GTC_WHISSTART")
        bVar2 = false
        pCVar5 = pCVar4:GetPos()
        r1 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", pCVar5, "Whisper")
        pCVar4 = quest:GetThingWithScriptName("MK_GTC_GMSTART")
        bVar2 = false
        pCVar5 = pCVar4:GetPos()
        r2 = quest:CreateCreature("CREATURE_GUILDKEEPER", pCVar5, "GM")
        xStack_40 = resources:NewResource()
        resources:PrepareResource(xStack_40)
        bVar2 = resources:TryAcquire(xStack_40, r1, 4)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d4a246 end
            bVar2 = resources:TryAcquire(xStack_40, r1, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:EntitySetInFaction(r1, "FACTION_HERO")
            xStack_30 = resources:NewResource()
            resources:PrepareResource(xStack_30)
            bVar2 = resources:TryAcquire(xStack_30, r2, 4)
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d4a23d end
                bVar2 = resources:TryAcquire(xStack_30, r2, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                xStack_20 = resources:NewResource()
                resources:PrepareResource(xStack_20)
                iVar9 = 4
                pCVar8 = xStack_20
                pCVar4 = quest:GetHero()
                bVar2 = resources:TryAcquire(pCVar8, pCVar4, iVar9)
                while not bVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d4a234 end
                    iVar9 = 4
                    pCVar8 = xStack_20
                    pCVar4 = quest:GetHero()
                    bVar2 = resources:TryAcquire(pCVar8, pCVar4, iVar9)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    xStack_58 = resources:NewActorMap()
                    resources:SetActor(xStack_58, "HERO", xStack_20)
                    resources:SetActor(xStack_58, "WHIS", xStack_40)
                    resources:SetActor(xStack_58, "GM", xStack_30)
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_CEREMONY", xStack_58, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:RemoveThing(r2, false, true)
                    quest:RemoveThing(r1, false, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                    resources:DestroyActorMap(xStack_58)
                end
                ::LAB_00d4a234::
                resources:ReleaseResource(xStack_20)
            end
            ::LAB_00d4a23d::
            resources:ReleaseResource(xStack_30)
        end
        ::LAB_00d4a246::
        resources:ReleaseResource(xStack_40)
    end
end

function RunSaveXPCutscene(quest)
    local resources = quest:RetailResources()
    local bVar2, iVar7, pCVar3, pppuVar6, r1, xStack_10, xStack_20, xStack_30, xStack_3c
    local alive = true
    r1 = quest:GetThingWithScriptName("TheRealGuildmaster")
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar2 = resources:TryAcquire(xStack_30, r1, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_30)
            r1 = nil
            return
        end
        bVar2 = resources:TryAcquire(xStack_30, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        iVar7 = 4
        pppuVar6 = xStack_20
        pCVar3 = quest:GetHero()
        bVar2 = resources:TryAcquire(pppuVar6, pCVar3, iVar7)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d4997a end
            iVar7 = 4
            pppuVar6 = xStack_20
            pCVar3 = quest:GetHero()
            bVar2 = resources:TryAcquire(pppuVar6, pCVar3, iVar7)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            xStack_3c = resources:NewActorMap()
            resources:SetActor(xStack_3c, "GM", xStack_30)
            resources:SetActor(xStack_3c, "HERO", xStack_20)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SAVEXP", xStack_3c, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_3c)
        end
        ::LAB_00d4997a::
        resources:ReleaseResource(xStack_20)
    end
    resources:ReleaseResource(xStack_30)
end

function RunSaveXPCutscene2(quest)
    local resources = quest:RetailResources()
    local bVar2, iVar7, pCVar3, pppuVar6, r1, xStack_10, xStack_20, xStack_30, xStack_3c
    local alive = true
    r1 = quest:GetThingWithScriptName("TheRealGuildmaster")
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar2 = resources:TryAcquire(xStack_30, r1, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_30)
            r1 = nil
            return
        end
        bVar2 = resources:TryAcquire(xStack_30, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        iVar7 = 4
        pppuVar6 = xStack_20
        pCVar3 = quest:GetHero()
        bVar2 = resources:TryAcquire(pppuVar6, pCVar3, iVar7)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d49caa end
            iVar7 = 4
            pppuVar6 = xStack_20
            pCVar3 = quest:GetHero()
            bVar2 = resources:TryAcquire(pppuVar6, pCVar3, iVar7)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            xStack_3c = resources:NewActorMap()
            resources:SetActor(xStack_3c, "GM", xStack_30)
            resources:SetActor(xStack_3c, "HERO", xStack_20)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SAVEXP2", xStack_3c, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_3c)
        end
        ::LAB_00d49caa::
        resources:ReleaseResource(xStack_20)
    end
    resources:ReleaseResource(xStack_30)
end

