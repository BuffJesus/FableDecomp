local MY_SCRIPT_NAME = "Fisherman"

local F_FishermanHelpDelay = 10
local F_FishermanIdleChatDelay = 15
local F_WaspType = "CREATURE_HORNET_01"
local FishermanSaveHimMorality = 0.01

local function RunAwaySequence(quest, me, hero)
    quest:Log("Fisherman: RunAwaySequence triggered.")

    -- He is no longer a person of interest
    quest:ClearThingHasInformation(me)
    quest:MiniMapRemoveMarker(me)
    quest:EntitySetTargetingType(me, 0)

    -- Ensure we have control of the entity
    me:AcquireControl()

    quest:PauseAllNonScriptedEntities(true)
    quest:SheatheHeroWeapons()

    local npcStand = quest:GetNearestWithScriptName(me, "NPC_NPCStand")
    if npcStand and quest:IsDistanceBetweenThingsUnder(me, npcStand, 10.0) then
        quest:EntityTeleportToThing(me, npcStand)
        quest:EntitySetFacingAngle(me, npcStand:GetAngleXY())
    end

    local heroStand = quest:GetNearestWithScriptName(me, "NPC_HeroStand")
    if heroStand and hero and quest:IsDistanceBetweenThingsUnder(hero, heroStand, 10.0) then
        quest:EntityTeleportToThing(hero, heroStand)
        quest:EntitySetFacingAngle(hero, heroStand:GetAngleXY())
    end

    local npcCamera = quest:GetNearestWithScriptName(me, "NPC_Camera")
    if npcCamera and npcCamera:IsAlive() then
        quest:CameraUseCameraPoint(npcCamera, {}, {}, -1.0, 0, -1)
    end

    if hero then
        quest:EntitySetFacingAngleTowardsThing(me, hero)
    end

    if quest:GetHealth(me) > 0.0 then
        me:SpeakAndWait("TEXT_QST_023_IDLE_03_HATE")
    end

    local fleeMarker = quest:GetThingWithScriptName("MK_FI_HERO")
    if fleeMarker then
        me:MoveToThing(fleeMarker, 3.0, 1) -- 1 = ENTITY_MOVE_RUN (Non-blocking)
    end

    quest:Pause(1.5)
    quest:FadeScreenOut(1.5, 1.5)
    quest:Pause(2.0)

    quest:RemoveThing(me)
    quest:CameraDefault()
    quest:PauseAllNonScriptedEntities(false)
    quest:FadeScreenIn()
    me:ReleaseControl()
    quest:Log("Fisherman: RunAwaySequence complete. Fisherman removed.")
end

local function CheckHeroAttack(quest, me, hero)
    local wasHit = me:MsgIsHitByHero()
        or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

    if wasHit and quest:GetHealth(me) <= 1.0 then
        quest:SetStateInt("Attitude", 2)
        quest:SetStateBool("FishermanMustRunAway", true)
        RunAwaySequence(quest, me, hero)
        return true
    end
    return false
end

function Init(quest, me)
    quest:Log("Fisherman: Init phase started.")
    quest:SetStateBool("FishermanMustRunAway", false)
    quest:EntitySetAsKillable(me, false)

    if not quest:GetStateBool("HeroHasSavedFishermanFromWasps") then
        local fisherMarker = quest:GetThingWithScriptName("MK_FI_FISHER")
        if fisherMarker then
            quest:EntityTeleportToThing(me, fisherMarker)
            local angle = fisherMarker:GetAngleXY()
            quest:EntitySetFacingAngle(me, angle)
        end

        local door = quest:GetThingWithScriptName("FishermanHutDoor")
        if door then
            quest:CloseDoor(door)
        end
    end
end

function Main(quest, me)
    quest:Log("Fisherman: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: Fisherman Main - could not get Hero.")
        return
    end

    -- =========================================================================
    -- TASK 1: Wasp Plague
    -- =========================================================================
    if not quest:GetStateBool("HeroHasSavedFishermanFromWasps") then
        quest:SetStateInt("FishermanTasks", 1)

        -- Spawn cutscene wasps
        local wasp1Marker = quest:GetThingWithScriptName("MK_INTRO_WASP1")
        local wasp2Marker = quest:GetThingWithScriptName("MK_INTRO_WASP2")
        local wasp3Marker = quest:GetThingWithScriptName("MK_INTRO_WASP3")

        local wasp1 = nil
        local wasp2 = nil
        local wasp3 = nil

        if wasp1Marker then
            wasp1 = quest:CreateCreature(F_WaspType, wasp1Marker:GetPos(), "CutsceneWasp")
        end
        if wasp2Marker then
            wasp2 = quest:CreateCreature(F_WaspType, wasp2Marker:GetPos(), "CutsceneWasp")
        end
        if wasp3Marker then
            wasp3 = quest:CreateCreature(F_WaspType, wasp3Marker:GetPos(), "CutsceneWasp")
        end

        -- Intro cutscene
        me:ReleaseControl()
        quest:StartCutscene({
            HERO = hero,
            FISHER = me,
            WASP1 = wasp1,
            WASP2 = wasp2,
            WASP3 = wasp3
        }, {}, true)
        quest:RunCutscene("CS_FISHERMAN_INTRO", true, false)
        quest:EndCutscene()

        -- Clean up cutscene wasps
        if wasp1 then quest:RemoveThing(wasp1) end
        if wasp2 then quest:RemoveThing(wasp2) end
        if wasp3 then quest:RemoveThing(wasp3) end

        -- Signal M_FishermanWaspSpawn to spawn real combat wasps
        quest:SetStateBool("IntroPlayed", true)

        -- Move inside hut to hiding spot
        me:AcquireControl()
        local hidingMarker = quest:GetThingWithScriptName("M_FishermanHiding")
        if hidingMarker then
            local hidingPos = hidingMarker:GetPos()
            while not quest:IsDistanceBetweenThingsUnder(me, hidingMarker, 3.0) do
                me:MoveToPosition(hidingPos, 1.0, 1)
                if CheckHeroAttack(quest, me, hero) then return end
                if not quest:NewScriptFrame(me) then
                    me:ReleaseControl()
                    return
                end
            end
        end

        -- Help cries loop while wasps are alive
        local windowPos = quest:GetThingWithScriptName("M_FishermanWindowPos")
        local helpTimer = quest:RegisterTimer()
        quest:SetTimer(helpTimer, F_FishermanHelpDelay)

        while (quest:GetStateInt("WaspsAlive") or 0) > 0 do
            if CheckHeroAttack(quest, me, hero) then
                quest:DeregisterTimer(helpTimer)
                me:ReleaseControl()
                return
            end

            if quest:GetTimer(helpTimer) <= 0 then
                if windowPos and hero and quest:IsDistanceBetweenThingsUnder(windowPos, hero, 10.0) then
                    quest:SetTimer(helpTimer, F_FishermanHelpDelay)
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    if convoID ~= -1 then
                        quest:AddLineToConversation(convoID, "TEXT_QST_023_HELP", me, hero, false)
                    end
                end
            end

            if not quest:NewScriptFrame(me) then
                quest:DeregisterTimer(helpTimer)
                me:ReleaseControl()
                return
            end
        end

        quest:DeregisterTimer(helpTimer)

        -- Wasps defeated
        quest:EntitySetAsScared(me, false)
        quest:SetStateBool("HeroHasSavedFishermanFromWasps", true)
        quest:GiveHeroMorality(FishermanSaveHimMorality)

        -- Thanks cutscene session (movie mode remains active through quest card & tutorial info)
        me:ReleaseControl()
        quest:StartCutscene({
            HERO = hero,
            FISHER = me
        }, {}, true)
        quest:RunCutscene("CS_FISHERMAN_THANKS", true, false)
        quest:FixMovieSequenceCamera(false)

        -- Quest card award
        if not quest:GetStateBool("QuestCardGiven") then
            local activeQuestName = quest:GetActiveQuestName()
            quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_FISHERMAN", activeQuestName, false)
            quest:SetStateBool("QuestCardGiven", true)
            quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_FISHERMAN_OBJECTIVE_01", "", "")
        end

        -- Tutorial info box
        if quest:IsXbox() then
            quest:DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO")
        else
            quest:DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO_PC")
        end
        while not quest:MsgIsGameInfoClickedPast() do
            if CheckHeroAttack(quest, me, hero) then
                quest:EndCutscene()
                return
            end
            if not quest:NewScriptFrame(me) then
                quest:EndCutscene()
                return
            end
        end

        -- End continuous cutscene session
        quest:EndCutscene()

        quest:SetStateInt("FishermanTasks", 2)
    end

    -- Open the door now that wasps are gone
    local door = quest:GetThingWithScriptName("FishermanHutDoor")
    if door then
        quest:OpenDoor(door)
    end

    me:TakeExclusiveControl()
    me:AcquireControl()

    local chatterTimer = quest:RegisterTimer()
    quest:SetTimer(chatterTimer, F_FishermanIdleChatDelay)

    -- =========================================================================
    -- TASK 2: Fishing Tutorial
    -- =========================================================================
    if quest:GetStateInt("FishermanTasks") == 2 then
        quest:SetThingHasInformation(me, false, true, false)
        local fishingMarker = quest:GetNearestWithScriptName(me, "FishermanTutorialMain")

        if fishingMarker and fishingMarker:IsAlive() and quest:IsFishingSpotEnabled(fishingMarker) then
            while quest:IsFishingSpotEnabled(fishingMarker) do
                if CheckHeroAttack(quest, me, hero) then
                    quest:DeregisterTimer(chatterTimer)
                    me:ReleaseControl()
                    return
                end

                -- Idle tutorial chatter
                if hero and quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) and quest:GetTimer(chatterTimer) <= 0 then
                    quest:SetTimer(chatterTimer, F_FishermanIdleChatDelay)
                    local line = quest:GetValidTextEntryNameWithAttitude(me, "TEXT_QST_023_TUTORIAL_IDLE")
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    if convoID ~= -1 then
                        quest:AddLineToConversation(convoID, line, me, hero, false)
                    end
                end

                -- Direct interaction with Hero
                if me:IsTalkedToByHero() then
                    local convoID = quest:StartAmbientConversation(me, hero, false, false)
                    if convoID ~= -1 then
                        quest:AddLineToConversation(convoID, "TEXT_QST_023_TUTORIAL_10", me, hero, false)
                    end
                end

                if not quest:NewScriptFrame(me) then
                    quest:DeregisterTimer(chatterTimer)
                    me:ReleaseControl()
                    return
                end
            end

            quest:SetStateInt("FishermanTasks", 3)
        else
            -- Hero already knows how to fish / spot is disabled
            local attitudeSuffix = ""
            if quest:GetStateInt("Attitude") == 2 then
                attitudeSuffix = "_HATE"
            end
            local textEntry = quest:GetValidTextEntryNameWithAttitude(me, "TEXT_QST_023_KNOWS_FISHING" .. attitudeSuffix)
            local convoID = quest:StartAmbientConversation(me, hero, false, false)
            if convoID ~= -1 then
                quest:AddLineToConversation(convoID, textEntry, me, hero, false)
            end

            me:PlayAnimation("SCRIPT_CHEER_1", false, false, true)
            quest:NewScriptFrame(me)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then
                    quest:DeregisterTimer(chatterTimer)
                    me:ReleaseControl()
                    return
                end
            end

            quest:SetStateInt("FishermanTasks", 4)
        end
    end

    quest:ClearThingHasInformation(me)

    -- =========================================================================
    -- TASK 3: Caught Fish Cutscene & Quest Completion
    -- =========================================================================
    if quest:GetStateInt("FishermanTasks") == 3 then
        me:ReleaseControl()
        quest:StartCutscene({
            HERO = hero,
            FISHER = me
        }, {}, true)

        if quest:GetStateInt("Attitude") == 2 then
            quest:RunCutscene("CS_FISHERMAN_CAUGHTFISH_NASTY", true, false)
        else
            quest:RunCutscene("CS_FISHERMAN_CAUGHTFISH", true, false)
        end
        quest:EndCutscene()

        local activeQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(activeQuestName, false, true, false)
        quest:SetStateInt("FishermanTasks", 4)
    end

    -- =========================================================================
    -- TASK 4: Post-Quest Idle Looking at River
    -- =========================================================================
    if quest:GetStateInt("FishermanTasks") == 4 then
        me:TakeExclusiveControl()
        me:AcquireControl()

        local lookAtRiver = quest:GetThingWithScriptName("M_FishermanLookAtRiver")
        if lookAtRiver then
            quest:EntityForceToLookAtThing(me, lookAtRiver)
        end

        quest:SetTimer(chatterTimer, F_FishermanIdleChatDelay)

        while true do
            if CheckHeroAttack(quest, me, hero) then
                quest:DeregisterTimer(chatterTimer)
                me:ReleaseControl()
                return
            end

            -- Idle chatter
            if hero and quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) and quest:GetTimer(chatterTimer) <= 0 then
                quest:SetTimer(chatterTimer, F_FishermanIdleChatDelay)
                local line = quest:GetValidTextEntryNameWithAttitude(me, "TEXT_QST_023_IDLE")
                local convoID = quest:StartAmbientConversation(me, hero, false, false)
                if convoID ~= -1 then
                    quest:AddLineToConversation(convoID, line, me, hero, false)
                end
            end

            if not quest:NewScriptFrame(me) then
                quest:DeregisterTimer(chatterTimer)
                me:ReleaseControl()
                return
            end
        end
    end

    quest:DeregisterTimer(chatterTimer)
    me:ReleaseControl()
    quest:Log("Fisherman: Main() terminated.")
end
