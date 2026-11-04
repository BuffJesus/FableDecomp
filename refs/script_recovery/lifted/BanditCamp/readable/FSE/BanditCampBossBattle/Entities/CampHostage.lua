-- Readable native conversion: CampHostage. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CampHostage.Main (retail 0x00d07e50)
function Main(quest, me)
    local resources = quest:RetailResources()
    local hostagesRescued, conversationId, switch1, hero, hero2, hero3, scratchValue
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
        hero = quest:GetHero()
        if not quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then goto LAB_00d08172 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        hero2 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, hero2, false)
        conversationId = quest:AddNewConversation(me, false, false)
        hero3 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero3)
        resources:PlayAnimationWithNativeArgument5(resource, "CS_SHOUT_FOR_HELP", false, false, false, true, false, false)
        switch1 = scratchValue
        repeat
            if switch1 == 0 then
                local hero4 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_FIRST", me, hero4, false)
                scratchValue = 1
                break
            else
                if switch1 == 1 then
                    local hero5 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_SECOND", me, hero5, false)
                    goto LAB_00d08150
                elseif switch1 == 2 then
                    local hero6 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_THIRD", me, hero6, false)
                    scratchValue = 3
                    break
                elseif switch1 == 3 then
                    local hero7 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SHOUT_FOURTH", me, hero7, false)
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
            local scratchValue6 = resources:ScriptThing(resource)
            local fret_0 = quest:GetHealth(scratchValue6)
            if 0.0 < fret_0 then
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, "TEXT_QST_009_HOSTAGE_HELP", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
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
            local scratchValue5 = resources:ScriptThing(resource)
            local fret_00 = quest:GetHealth(scratchValue5)
            if 0.0 < fret_00 then
                local hero9 = quest:GetHero()
                if not me:Speak(hero9, "TEXT_QST_009_HOSTAGE_CLOSE", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
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

