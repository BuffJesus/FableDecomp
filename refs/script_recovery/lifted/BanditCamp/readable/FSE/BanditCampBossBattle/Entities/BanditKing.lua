-- Readable native conversion: BanditKing. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local healthBarIndex

-- BanditKing.Main (retail 0x00d0a830)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, predicateResult3, predicateResult7, predicateResult8
    local predicateResult, conversationId3, switch1, hero6, hero13, movie, scratchValue11
    local scratchValue12
    if not quest:NewScriptFrame(me) then return end
    quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_07", "", "BanditCampMain")
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:EntitySetAbleToBeEngagedInCombat(me, false)
    local fret_v0 = quest:GetHealth(me)
    scratchValue = math.tointeger(math.modf(fret_v0))
    predicateResult3 = false
    predicateResult = false
    local scratchValue10 = scratchValue
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while not quest:GetStateBool("ItsAllOver") do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
        local fret_v00 = quest:GetHealth(me)
        local scratchValue6 = math.tointeger(math.modf(fret_v00))
        quest:SetStateInt("KingHealth", scratchValue6)
        if (quest:GetStateInt("KingHealth") < math.tointeger(math.modf((scratchValue * 3) / 4)) and not predicateResult3) and quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local conversationId = quest:AddNewConversation(me, false, false)
            local hero = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero)
            local hero2 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_009_TWINBLADE_TAUNT_10", me, hero2, false)
            predicateResult3 = true
            quest:SetTimer(timerId, 8)
            scratchValue = scratchValue10
        end
        if (quest:GetStateInt("KingHealth") < math.tointeger(math.modf(scratchValue / 2)) and not predicateResult) and quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local conversationId2 = quest:AddNewConversation(me, false, false)
            local hero3 = quest:GetHero()
            quest:AddPersonToConversation(conversationId2, hero3)
            local hero4 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_TAUNT_20", me, hero4, false)
            predicateResult = true
            quest:SetTimer(timerId, 8)
            scratchValue = scratchValue10
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
    scratchValue2 = 0
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); quest:DeregisterTimer(timerId); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); quest:DeregisterTimer(timerId); return end
    local timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 0)
    local hero5 = quest:GetHero()
    quest:EntitySetFacingAngleTowardsThing(me, hero5, false)
    local pThing = quest:GetHero()
    quest:EntitySetFacingAngleTowardsThing(pThing, me, false)
    resources:PlayLoopingAnimation(resource, "DEFEATED_POSE", -1, false, false, false, true, true, false, false)
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
        hero6 = quest:GetHero()
        quest:AddPersonToConversation(conversationId3, hero6)
        switch1 = scratchValue2
        repeat
            if switch1 == 0 then
                local hero7 = quest:GetHero()
                quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_10", me, hero7, false)
                scratchValue2 = 1
                scratchValue11 = 1
                break
            elseif switch1 == 1 then
                local hero8 = quest:GetHero()
                quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_20", me, hero8, false)
                scratchValue2 = 2
                scratchValue11 = 2
                break
            else
                if switch1 == 2 then
                    local hero9 = quest:GetHero()
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_30", me, hero9, false)
                    goto LAB_00d0ae5e
                elseif switch1 == 3 then
                    local hero10 = quest:GetHero()
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_40", me, hero10, false)
                    scratchValue2 = 4
                    scratchValue11 = 4
                    break
                elseif switch1 == 4 then
                    local hero11 = quest:GetHero()
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_009_TWINBLADE_BEGGING_50", me, hero11, false)
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
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0af0f end
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
        scratchValue12 = resources:ScriptThing(resource)
        if 0.0 < quest:GetHealth(scratchValue12) then
            local p1 = "TEXT_QST_009_TWINBLADE_OVER"
            local hero12 = quest:GetHero()
            if not me:Speak(hero12, p1, 0, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0b27a end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0b27a end
                goto FLOW_past_lab_00d0b27a
            end
            ::LAB_00d0b27a::
            resources:DestroyMovie(movie)
            goto LAB_00d0b297
            ::FLOW_past_lab_00d0b27a::
        end
        quest:SetStateBool("TwinBladeAttacked", true)
        quest:SetStateInt("AngryBanditNeeded", 4)
        resources:PrepareResource(resource)
        hero13 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(me, hero13)
        quest:EntitySetAsKillable(me, true, true)
        quest:EntitySetAbleToBeEngagedInCombat(me, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:DisplayQuestInfo(true)
        healthBarIndex = quest:AddQuestInfoBar(scratchValue10, 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
        scratchValue2 = scratchValue11
        while me:IsAlive() do
            if not quest:NewScriptFrame(me) then goto LAB_00d0b297 end
            local fret_00 = quest:GetHealth(me)
            quest:UpdateQuestInfoBar(healthBarIndex, fret_00, -1.0, -1.0)
            scratchValue2 = scratchValue11
        end
        ::FLOW_past_lab_00d0b297::
        quest:NewScriptFrame(me)
        predicateResult7 = quest:IsActiveThreadTerminating()
    until false
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

