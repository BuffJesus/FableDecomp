-- Readable native conversion: MazeAtTavern. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local hitCount, talkCounter, wavedOver

-- MazeAtTavern.Main (retail 0x00e26e30)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local b3, predicateResult, scratchValue, isPerformingScriptTask, p4, p5, scratchValue4
    local scratchValue5, scratchValue6
    scratchValue6 = 0
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
    predicateResult = quest:IsActiveThreadTerminating()
    scratchValue5 = 0
    isPerformingScriptTask = quest:RegisterTimer()
    repeat
        local i_stk_98_2 = isPerformingScriptTask
        if predicateResult then
            quest:DeregisterTimer(isPerformingScriptTask)
            resources:ReleaseResource(resource3)
            return
        end
        if quest:GetTimer(isPerformingScriptTask) == 0 then
            if quest:IsDistanceBetweenThingsUnder(hero, me, 30.0) then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(isPerformingScriptTask)
                    resources:ReleaseResource(resource3)
                    return
                end
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                quest:AddLineToConversation(conversationId, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, hero, false)
                me:PlayAnimation("ST_HELLO", false, false, false, true, true, false, false)
                quest:SetTimer(i_stk_98_2, 5)
                scratchValue5 = scratchValue6
            end
        end
        if not quest:GetStateBool("GuardianSpokeToHero") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        end
        scratchValue4 = scratchValue5 | 1
        scratchValue6 = scratchValue4
        if me:MsgIsHitByHero() then
            goto LAB_00e270f6
        else
            scratchValue4 = scratchValue5 | 3
            scratchValue6 = scratchValue4
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue4 = scratchValue5 | 7
                scratchValue6 = scratchValue4
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e270f6 end
            end
            scratchValue = 0
        end
        goto FLOW_past_lab_00e270f6
        ::LAB_00e270f6::
        scratchValue = 1
        ::FLOW_past_lab_00e270f6::
        if scratchValue4 & 4 ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffb
            scratchValue6 = scratchValue4
        end
        if scratchValue4 & 2 ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffd
            scratchValue6 = scratchValue4
        end
        if scratchValue4 & 1 ~= 0 then
            scratchValue6 = scratchValue4 & 0xfffffffe
        end
        if scratchValue ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(i_stk_98_2)
                resources:ReleaseResource(resource3)
                return
            end
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)
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
            local fret_0 = quest:GetHealth(resources:ScriptThing(resource3))
            if 0.0 < fret_0 then
                p5 = 0
                p4 = 1
                me:Speak(hero, "TEXT_QST_076_MAZE_ON_HIT_10", GROUP_SELECT_FIRST, false, true, false)
                isPerformingScriptTask = me:IsPerformingScriptTask()
                while isPerformingScriptTask do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(i_stk_98_2)
                        resources:ReleaseResource(resource3)
                        do return end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
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
            hitCount = hitCount + 1
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
                local resource = resources:NewResource()
                resources:TryAcquire(resource, hero, 4)
                local actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "MAZE", resource3)
                resources:SetActor(actorMap, "HERO", resource)
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:RunMacro("CS_GUARDIAN_SISTER_2", actorMap, false, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
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
        if not quest:GetStateBool("GuardianSpokeToHero") then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); scratchValue5 = scratchValue6; goto continue_6 end
        if not quest:IsActiveThreadTerminating() then
            me:ClearCommands()
            me:PlayAnimation("ST_TELEPORT_OUT", false, false, false, true, true, false, false)
            local pPosition = me:GetPos()
            -- TODO(native): CreateEffect is not a ForgeFSE binding
            quest:CreateEffect("MAZE_TELEPORT_OUT_01", pPosition, "", 0.0, false, false)
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
            b3 = false
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        end
        quest:DeregisterTimer(i_stk_98_2)
        resources:ReleaseResource(resource3)
        do return end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        scratchValue5 = scratchValue6
        ::continue_6::
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

