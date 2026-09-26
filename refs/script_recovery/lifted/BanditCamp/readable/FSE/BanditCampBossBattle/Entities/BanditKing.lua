-- Readable native conversion: BanditKing. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local healthBarIndex

-- BanditKing.Main (retail 0x00d0a830)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, predicateResult3, predicateResult7, predicateResult8, predicateResult
    local conversationId3, switch1, movie, scratchValue11, timerId3
    if not quest:NewScriptFrame(me) then return end
    quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_07", "", "BanditCampMain")
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:EntitySetAbleToBeEngagedInCombat(me, false)
    local scratchValue = math.tointeger(math.modf(quest:GetHealth(me)))
    predicateResult3 = false
    predicateResult = false
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while not quest:GetStateBool("ItsAllOver") do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
        quest:SetStateInt("KingHealth", math.tointeger(math.modf(quest:GetHealth(me))))
        if (quest:GetStateInt("KingHealth") < math.tointeger(math.modf((scratchValue * 3) / 4)) and not predicateResult3) and quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_009_TWINBLADE_TAUNT_10", me, hero, false)
            predicateResult3 = true
            quest:SetTimer(timerId, 8)
        end
        if (quest:GetStateInt("KingHealth") < math.tointeger(math.modf(scratchValue / 2)) and not predicateResult) and quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local conversationId2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId2, hero)
            quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_TAUNT_20", me, hero, false)
            predicateResult = true
            quest:SetTimer(timerId, 8)
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
    scratchValue2 = 0
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d0b2a0 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d0b2a0 end
    timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 0)
    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
    quest:EntitySetFacingAngleTowardsThing(hero, me, false)
    me:PlayLoopingAnimation("DEFEATED_POSE", -1, false, false, false)
    predicateResult7 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult7 then
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            return
        end
        if 0 < quest:GetTimer(timerId3) then goto LAB_00d0ae81 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d0b297 end
        conversationId3 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId3, hero)
        switch1 = scratchValue2
        repeat
            if switch1 == 0 then
                quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_10", me, hero, false)
                scratchValue2 = 1
                scratchValue11 = 1
                break
            elseif switch1 == 1 then
                quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_20", me, hero, false)
                scratchValue2 = 2
                scratchValue11 = 2
                break
            else
                if switch1 == 2 then
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_30", me, hero, false)
                    goto LAB_00d0ae5e
                elseif switch1 == 3 then
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_40", me, hero, false)
                    scratchValue2 = 4
                    scratchValue11 = 4
                    break
                elseif switch1 == 4 then
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_50", me, hero, false)
                    goto LAB_00d0ae5e
                end
                goto FLOW_past_lab_00d0ae5e
                ::LAB_00d0ae5e::
                scratchValue2 = 3
                scratchValue11 = 3
                ::FLOW_past_lab_00d0ae5e::
            end
        until true
        quest:SetTimer(timerId3, 10)
        ::LAB_00d0ae81::
        if me:MsgIsHitByHero() then
            goto LAB_00d0af0f
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0af0f end
            end
            predicateResult8 = false
        end
        goto FLOW_past_lab_00d0af0f
        ::LAB_00d0af0f::
        predicateResult8 = true
        ::FLOW_past_lab_00d0af0f::
        if predicateResult8 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0b297 end
            goto FLOW_hoist_lab_00d0b297_1
        end
        goto FLOW_past_lab_00d0b297
        ::LAB_00d0b297::
        quest:DeregisterTimer(timerId3)
        break
        ::FLOW_hoist_lab_00d0b297_1::
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        me:ClearAllActionsIncludingLoopingAnimations()
        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
            me:Speak(hero, "TEXT_QST_009_TWINBLADE_OVER", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0b27a end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0b27a end
            goto FLOW_past_lab_00d0b27a
            ::LAB_00d0b27a::
            resources:DestroyMovie(movie)
            goto LAB_00d0b297
            ::FLOW_past_lab_00d0b27a::
        end
        quest:SetStateBool("TwinBladeAttacked", true)
        quest:SetStateInt("AngryBanditNeeded", 4)
        resources:PrepareResource(resource)
        quest:GiveThingBestEnemyTarget(me, hero)
        quest:EntitySetAsKillable(me, true, true)
        quest:EntitySetAbleToBeEngagedInCombat(me, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:DisplayQuestInfo(true)
        healthBarIndex = quest:AddQuestInfoBar(scratchValue, 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
        scratchValue2 = scratchValue11
        while me:IsAlive() do
            if not quest:NewScriptFrame(me) then goto LAB_00d0b297 end
            quest:UpdateQuestInfoBar(healthBarIndex, quest:GetHealth(me), -1.0, -1.0)
            scratchValue2 = scratchValue11
        end
        ::FLOW_past_lab_00d0b297::
        quest:NewScriptFrame(me)
        predicateResult7 = quest:IsActiveThreadTerminating()
    until false
    ::LAB_00d0b2a0::
    resources:ReleaseResource(resource)
    quest:DeregisterTimer(timerId)
end

-- BanditKing.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- BanditKing.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BanditKing.OnPredicateFail (retail 0x00d0a7b0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateBool("TwinBladeKilled", true)
        quest:SetMasterGameState("BanditCampTwinbladeKilled", true)
    end
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(healthBarIndex)
end

