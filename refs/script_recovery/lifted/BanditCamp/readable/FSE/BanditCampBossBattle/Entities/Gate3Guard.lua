-- Readable native conversion: Gate3Guard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- Gate3Guard.Main (retail 0x00d07490)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult3, predicateResult, predicateResult20, timerId
    local scratchValue, scratchValue6
    scratchValue6 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d07d05 end
    end
    predicateResult20 = quest:IsActiveThreadTerminating()
    if predicateResult20 then goto LAB_00d07d05 end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain")
    predicateResult2 = quest:IsActiveThreadTerminating()
    predicateResult = predicateResult20
    repeat
        if predicateResult2 then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        local scratchValue5 = scratchValue6
        scratchValue6 = scratchValue6 | 1
        if me:MsgIsHitByHero() then
            goto LAB_00d076a9
        else
            scratchValue = scratchValue5 | 3
            scratchValue6 = scratchValue
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = scratchValue5 | 7
                scratchValue6 = scratchValue
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d076a9 end
            end
            predicateResult3 = false
        end
        goto FLOW_past_lab_00d076a9
        ::LAB_00d076a9::
        predicateResult3 = true
        ::FLOW_past_lab_00d076a9::
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
            scratchValue6 = scratchValue
        end
        if scratchValue & 2 ~= 0 then
            scratchValue = scratchValue & 0xfffffffd
            scratchValue6 = scratchValue
        end
        if scratchValue & 1 ~= 0 then
            scratchValue6 = scratchValue & 0xfffffffe
        end
        if predicateResult3 then
            if not quest:IsActiveThreadTerminating() then
                quest:GiveThingBestEnemyTarget(me, hero)
                resources:PrepareResource(resource)
                quest:ClearThingHasInformation(me)
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            -- TODO(native): override_prt_d07c09_8df7e253:
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:GetStateBool("Gate3Open") then
            -- TODO(native): if (bVar4) goto override_prt_d07c09_8df7e253;
            if quest:GetTimer(timerId) < 1 then
                if not quest:IsDistanceBetweenThingsUnder(hero, me, 8.0) then goto LAB_00d0783a end
                if not quest:IsActiveThreadTerminating() then
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    if not predicateResult20 then
                        if not quest:IsActiveThreadTerminating() then
                            predicateResult20 = true
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", me, hero, false)
                            goto LAB_00d07820
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", me, hero, false)
                        goto LAB_00d07820
                    end
                    goto FLOW_past_lab_00d07820
                    ::LAB_00d07820::
                    quest:SetTimer(timerId, 10)
                    goto LAB_00d0783a
                    ::FLOW_past_lab_00d07820::
                end
            else
                goto LAB_00d0783a
            end
            goto FLOW_past_lab_00d0783a
            ::LAB_00d0783a::
            if not me:IsTalkedToByHero() then goto LAB_00d07bbe end
            if not quest:IsActiveThreadTerminating() then
                if predicateResult == false then
                    if not quest:IsActiveThreadTerminating() then
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d07cfc end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                            if 0.0 < fret_0 then
                                me:Speak(hero, "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07caf end
                                end
                                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07caf end
                                goto FLOW_past_lab_00d07caf
                                ::LAB_00d07caf::
                                goto LAB_00d07cf7
                            end
                            goto FLOW_hoist_lab_00d07cf7_1
                        end
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d07cfc end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_00 then
                            me:Speak(hero, "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07ce2 end
                            end
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07ce2 end
                            goto FLOW_past_lab_00d07ce2
                            ::LAB_00d07ce2::
                            goto LAB_00d07cf7
                            ::FLOW_past_lab_00d07ce2::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00d07bb5
                    end
                end
                goto FLOW_past_lab_00d07cf7
                ::LAB_00d07cf7::
                goto LAB_00d07cfc
                ::FLOW_past_lab_00d07caf::
                ::FLOW_hoist_lab_00d07cf7_1::
                predicateResult = true
                quest:PauseAllNonScriptedEntities(false)
                ::LAB_00d07bb5::
                goto LAB_00d07bbe
                ::FLOW_past_lab_00d07cf7::
            end
            ::FLOW_past_lab_00d0783a::
            ::LAB_00d07cfc::
            quest:DeregisterTimer(timerId)
            break
        end
        ::LAB_00d07bbe::
        quest:NewScriptFrame(me)
        predicateResult2 = quest:IsActiveThreadTerminating()
    until false
    ::LAB_00d07d05::
    resources:ReleaseResource(resource)
end

-- Gate3Guard.Init (retail 0x00d07410)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

-- Gate3Guard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Gate3Guard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

