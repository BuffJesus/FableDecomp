-- Readable native conversion: Gate3Guard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- Gate3Guard.Main (retail 0x00d07490)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult3, predicateResult, predicateResult20, movie2
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    predicateResult20 = quest:IsActiveThreadTerminating()
    if predicateResult20 then resources:ReleaseResource(resource); return end
    local timerId = quest:RegisterTimer()
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
        if me:MsgIsHitByHero() then
            goto LAB_00d076a9
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d076a9 end
            predicateResult3 = false
        end
        goto FLOW_past_lab_00d076a9
        ::LAB_00d076a9::
        predicateResult3 = true
        ::FLOW_past_lab_00d076a9::
        if predicateResult3 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local hero = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, hero)
            resources:PrepareResource(resource)
            quest:ClearThingHasInformation(me)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:GetStateBool("Gate3Open") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if quest:GetTimer(timerId) < 1 then
                local hero2 = quest:GetHero()
                if not quest:IsDistanceBetweenThingsUnder(hero2, me, 8.0) then goto LAB_00d0783a end
                if not quest:IsActiveThreadTerminating() then
                    local conversationId = quest:AddNewConversation(me, false, false)
                    local hero3 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId, hero3)
                    if not predicateResult20 then
                        if not quest:IsActiveThreadTerminating() then
                            predicateResult20 = true
                            local hero4 = quest:GetHero()
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", me, hero4, false)
                            goto LAB_00d07820
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        local hero5 = quest:GetHero()
                        quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", me, hero5, false)
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
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d0783a end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            movie2 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            local scratchValue6 = resources:ScriptThing(resource)
                            local fret_0 = quest:GetHealth(scratchValue6)
                            if 0.0 < fret_0 then
                                local hero6 = quest:GetHero()
                                if not me:Speak(hero6, "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                else
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07caf end
                                    end
                                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07caf end
                                    goto FLOW_past_lab_00d07caf
                                end
                                ::LAB_00d07caf::
                                resources:DestroyMovie(movie2)
                                goto LAB_00d07cf7
                            end
                            goto FLOW_past_lab_00d07caf
                        end
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d0783a end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue = resources:ScriptThing(resource)
                        local fret_00 = quest:GetHealth(scratchValue)
                        if 0.0 < fret_00 then
                            local hero7 = quest:GetHero()
                            if not me:Speak(hero7, "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07ce2 end
                                end
                                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d07ce2 end
                                goto FLOW_past_lab_00d07ce2
                            end
                            ::LAB_00d07ce2::
                            resources:DestroyMovie(movie)
                            goto LAB_00d07cf7
                            ::FLOW_past_lab_00d07ce2::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d07bb5
                    end
                end
                goto FLOW_past_lab_00d07cf7
                ::LAB_00d07cf7::
                goto FLOW_past_lab_00d0783a
                ::FLOW_past_lab_00d07caf::
                predicateResult = true
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                ::LAB_00d07bb5::
                goto LAB_00d07bbe
                ::FLOW_past_lab_00d07cf7::
            end
            ::FLOW_past_lab_00d0783a::
            quest:DeregisterTimer(timerId)
            break
        end
        ::LAB_00d07bbe::
        quest:NewScriptFrame(me)
        predicateResult2 = quest:IsActiveThreadTerminating()
    until false
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

