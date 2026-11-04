-- Readable native conversion: MazeAtTavern. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local hitCount, talkCounter, wavedOver

-- MazeAtTavern.Main (retail 0x00e26e30)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, timerId, scratchValue5, scratchValue6, scratchValue7
    scratchValue7 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, me, 3) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    quest:SetThingHasInformation(me, false, true, false)
    local i_stk_98_1 = quest:RegisterTimer()
    predicateResult = quest:IsActiveThreadTerminating()
    scratchValue6 = 0
    timerId = i_stk_98_1
    repeat
        local i_stk_98_2 = timerId
        if predicateResult then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource3)
            return
        end
        if quest:GetTimer(timerId) == 0 then
            local hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(hero, me, 30.0) then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource3)
                    return
                end
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero2 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero2)
                local hero3 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, hero3, false)
                resources:PlayAnimationWithNativeArgument5(resource3, "ST_HELLO", false, false, false, true, false, false)
                quest:SetTimer(i_stk_98_2, 5)
                scratchValue6 = scratchValue7
            end
        end
        if not quest:GetStateBool("GuardianSpokeToHero") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            local hero4 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
        end
        scratchValue5 = scratchValue6 | 1
        scratchValue7 = scratchValue5
        if me:MsgIsHitByHero() then
            goto LAB_00e270f6
        else
            scratchValue5 = scratchValue6 | 3
            scratchValue7 = scratchValue5
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue5 = scratchValue6 | 7
                scratchValue7 = scratchValue5
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e270f6 end
            end
            scratchValue = 0
        end
        goto FLOW_past_lab_00e270f6
        ::LAB_00e270f6::
        scratchValue = 1
        ::FLOW_past_lab_00e270f6::
        if (scratchValue5 & 4) ~= 0 then
            scratchValue5 = scratchValue5 & 0xfffffffb
            scratchValue7 = scratchValue5
        end
        if (scratchValue5 & 2) ~= 0 then
            scratchValue5 = scratchValue5 & 0xfffffffd
            scratchValue7 = scratchValue5
        end
        if (scratchValue5 & 1) ~= 0 then
            scratchValue7 = scratchValue5 & 0xfffffffe
        end
        if scratchValue ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            local hero5 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero5)
            local hero8 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(hero8, me)
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue8 = resources:ScriptThing(resource3)
            local fret_0 = quest:GetHealth(scratchValue8)
            if 0.0 < fret_0 then
                local p1 = "TEXT_QST_076_MAZE_ON_HIT_10"
                local hero6 = quest:GetHero()
                if not me:Speak(hero6, p1, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(i_stk_98_2)
                        resources:ReleaseResource(resource3)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    return
                end
            end
            hitCount = (hitCount + 1 + 2147483648) % 4294967296 - 2147483648
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            if talkCounter == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    return
                end
                quest:ForceShipsVisible()
                local resource2 = resources:NewResource()
                local resource = resource2
                local hero7 = quest:GetHero()
                resources:TryAcquire(resource, hero7, 4)
                local actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "MAZE", resource3)
                resources:SetActor(actorMap, "HERO", resource2)
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:RunMacro("CS_GUARDIAN_SISTER_2", actorMap, false, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource2)
                quest:SetStateBool("GuardianSpokeToHero", true)
                quest:AddQuestCard("OBJECT_QUEST_CARD_BANDIT_CAMP", "Q_BanditCamp", false, false)
            end
            talkCounter = talkCounter + 1
            if hitCount < 1 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    return
                end
                resources:PrepareResource(resource3)
                while not resources:TryAcquire(resource3, me, 3) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(i_stk_98_2)
                        resources:ReleaseResource(resource3)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(i_stk_98_2)
                    resources:ReleaseResource(resource3)
                    return
                end
            end
            quest:ClearThingHasInformation(me)
        end
        if quest:GetStateBool("GuardianSpokeToHero") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(i_stk_98_2); resources:ReleaseResource(resource3); return end
            me:ClearCommands()
            resources:PlayAnimationWithNativeArgument5(resource3, "ST_TELEPORT_OUT", false, false, false, true, false, false)
            local pPosition = me:GetPos()
            quest:CreateEffectAtPos("MAZE_TELEPORT_OUT_01", pPosition, 0.0, false)
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
            local getActiveQuestName2 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName2, 0)
            quest:DeregisterTimer(i_stk_98_2)
            resources:ReleaseResource(resource3)
            return
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        scratchValue6 = scratchValue7
        timerId = i_stk_98_2
    until false
end

-- MazeAtTavern.Init (retail 0x00e26bd0)
function Init(quest, me)
    talkCounter = 0
    wavedOver = false
    hitCount = 0
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:SetIsPushableByHero(me, false)
end

-- MazeAtTavern.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MazeAtTavern.OnPredicateFail (retail 0x00e26ba0)
function OnPredicateFail(quest, me)
end

