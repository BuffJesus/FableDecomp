-- Readable native conversion: CampHostage. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CampHostage.Main (retail 0x00d07e50)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local hostagesRescued, conversationId, switch1, scratchValue
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    hostagesRescued = quest:GetStateBool("HostagesRescued")
    scratchValue = 0
    while not hostagesRescued and not quest:GetStateBool("HostageKilled") do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        if 0 < quest:GetTimer(timerId) then goto LAB_00d08172 end
        if not quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then goto LAB_00d08172 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        me:PlayAnimation("CS_SHOUT_FOR_HELP", false, false, false, true, true, false, false)
        switch1 = scratchValue
        repeat
            if switch1 == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_FIRST", me, hero, false)
                scratchValue = 1
                break
            else
                if switch1 == 1 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_SECOND", me, hero, false)
                    goto LAB_00d08150
                elseif switch1 == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_THIRD", me, hero, false)
                    scratchValue = 3
                    break
                elseif switch1 == 3 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_FOURTH", me, hero, false)
                    goto LAB_00d08150
                end
                goto FLOW_past_lab_00d08150
                ::LAB_00d08150::
                scratchValue = 2
                ::FLOW_past_lab_00d08150::
            end
        until true
        quest:SetTimer(timerId, 10)
        ::LAB_00d08172::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_0 then
                me:Speak(hero, "TEXT_QST_009_HOSTAGE_HELP", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        if not quest:GetStateBool("PlayGuardTooCloseCutscene") then
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        else
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_00 then
                me:Speak(hero, "TEXT_QST_009_HOSTAGE_CLOSE", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            quest:SetStateBool("PlayGuardTooCloseCutscene", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        end
    end
    if not quest:IsActiveThreadTerminating() then
        resources:PrepareResource(resource)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- CampHostage.Init (retail 0x00d07dd0)
function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_NEUTRALS")
    quest:EntitySetOpinionReactionEnabled(me, 34, false)
    quest:SetThingHasInformation(me, false, false, false)
end

-- CampHostage.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CampHostage.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

