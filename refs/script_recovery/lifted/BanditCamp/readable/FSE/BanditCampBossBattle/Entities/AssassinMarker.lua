-- Readable native conversion: AssassinMarker. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- AssassinMarker.Main (retail 0x00d11930)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, ctr_c, addNewConversation, assassin1, assassin, assassin3
    if not quest:NewScriptFrame(me) then return end
    assassin1 = quest:GetThingWithScriptName("Assassin1")
    assassin = quest:GetThingWithScriptName("Assassin2")
    assassin3 = quest:GetThingWithScriptName("Assassin3")
    quest:EntitySetAsKillable(assassin1, false, true)
    quest:EntitySetAsKillable(assassin, false, true)
    quest:EntitySetAsKillable(assassin3, false, true)
    local resource7 = resources:NewResource()
    local resource5 = resources:NewResource()
    local resource6 = resources:NewResource()
    predicateResult = false
    addNewConversation = 0
    ctr_c = 0
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    while not quest:GetStateBool("AssassinCutsceneTriggered") and not predicateResult do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); goto LAB_00d128bf end
        if not quest:GetStateBool("AssassinsUnderAttack") then
            local hero = quest:GetHero()
            local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 4.0)
            predicateResult = false
            if not isDistanceBetweenThingsUnder or quest:IsConversationActive(addNewConversation) or quest:GetStateBool("TalkedToAssassin") then goto LAB_00d12226 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d12245 end
            local switch = ctr_c
            repeat
                if switch == 0 then
                    addNewConversation = quest:AddNewConversation(assassin1, false, false)
                    quest:AddPersonToConversation(addNewConversation, assassin)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_ONE", assassin1, assassin, false)
                    break
                elseif switch == 1 then
                    addNewConversation = quest:AddNewConversation(assassin, false, false)
                    quest:AddPersonToConversation(addNewConversation, assassin1)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN2_LISTENED_ONE", assassin, assassin1, false)
                    break
                elseif switch == 2 then
                    addNewConversation = quest:AddNewConversation(assassin1, false, false)
                    quest:AddPersonToConversation(addNewConversation, assassin)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_TWO", assassin1, assassin, false)
                    break
                elseif switch == 3 then
                    addNewConversation = quest:AddNewConversation(assassin, false, false)
                    quest:AddPersonToConversation(addNewConversation, assassin1)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN2_LISTENED_TWO", assassin, assassin1, false)
                    break
                elseif switch == 4 then
                    addNewConversation = quest:AddNewConversation(assassin1, false, false)
                    local hero2 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero2)
                    local hero3 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_THREE", assassin1, hero3, false)
                    break
                elseif switch == 5 then
                    addNewConversation = quest:AddNewConversation(assassin1, false, false)
                    local hero4 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero4)
                    local hero5 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_FOUR", assassin1, hero5, false)
                    break
                else
                    break
                end
            until true
            ctr_c = ctr_c + 1
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d12245 end
            resources:TryAcquire(resource7, assassin1, 4)
            resources:TryAcquire(resource5, assassin, 4)
            resources:TryAcquire(resource6, assassin3, 4)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue = resources:ScriptThing(resource7)
            local fret_0 = quest:GetHealth(scratchValue)
            if 0.0 < fret_0 then
                local hero6 = quest:GetHero()
                resources:Speak(resource7, hero6, "TEXT_QST_009_ASSASSIN1_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource7) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource6)
                        resources:ReleaseResource(resource5)
                        resources:ReleaseResource(resource7)
                        assassin3 = nil
                        assassin = nil
                        assassin1 = nil
                        do return end
                    end
                end
                if not quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d1244c end
            else
                goto FLOW_past_lab_00d1244c
            end
            ::LAB_00d1244c::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DeregisterTimer(timerId)
            goto LAB_00d128bf
            ::FLOW_past_lab_00d1244c::
            local scratchValue3 = resources:ScriptThing(resource5)
            local fret_00 = quest:GetHealth(scratchValue3)
            if 0.0 < fret_00 then
                local hero7 = quest:GetHero()
                resources:Speak(resource5, hero7, "TEXT_QST_009_ASSASSIN2_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource5) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource6)
                        resources:ReleaseResource(resource5)
                        resources:ReleaseResource(resource7)
                        assassin3 = nil
                        assassin = nil
                        assassin1 = nil
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d1244c end
            end
            local scratchValue4 = resources:ScriptThing(resource6)
            local fret_01 = quest:GetHealth(scratchValue4)
            if 0.0 < fret_01 then
                local hero8 = quest:GetHero()
                resources:Speak(resource6, hero8, "TEXT_QST_009_ASSASSIN3_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource6) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        goto LAB_00d128bf
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d1244c end
            end
            local hero9 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(assassin1, hero9)
            local hero10 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(assassin, hero10)
            local hero11 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(assassin3, hero11)
            predicateResult = true
            resources:PrepareResource(resource7)
            resources:PrepareResource(resource5)
            resources:PrepareResource(resource6)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        ::LAB_00d12226::
    end
    if not (quest:IsActiveThreadTerminating()) then
        goto FLOW_past_lab_00d12245
    end
    ::LAB_00d12245::
    quest:DeregisterTimer(timerId)
    goto LAB_00d128bf
    ::FLOW_past_lab_00d12245::
    quest:EntitySetAsKillable(assassin1, true, true)
    quest:EntitySetAsKillable(assassin, true, true)
    quest:EntitySetAsKillable(assassin3, true, true)
    if not quest:GetStateBool("AssassinCutsceneTriggered") then
        goto LAB_00d128a3
    elseif not quest:IsActiveThreadTerminating() then
        local resource = resources:NewResource()
        local resource2 = resources:NewResource()
        local resource3 = resources:NewResource()
        local resource4 = resources:NewResource()
        local hero12 = quest:GetHero()
        resources:TryAcquire(resource, hero12, 4)
        resources:TryAcquire(resource2, assassin1, 4)
        resources:TryAcquire(resource3, assassin, 4)
        resources:TryAcquire(resource4, assassin3, 4)
        quest:EntitySetInFaction(assassin1, "FACTION_TWINBLADE_CAMP_BANDITS")
        quest:EntitySetInFaction(assassin, "FACTION_TWINBLADE_CAMP_BANDITS")
        quest:EntitySetInFaction(assassin3, "FACTION_TWINBLADE_CAMP_BANDITS")
        local actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource)
        resources:SetActor(actorMap, "ASS1", resource2)
        resources:SetActor(actorMap, "ASS2", resource3)
        resources:SetActor(actorMap, "ASS3", resource4)
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_BANDITCAMP_ASSATTACK", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateBool("Gate3Open", true)
        local gate3Guard = quest:GetThingWithScriptName("Gate3Guard")
        quest:RemoveThing(gate3Guard, false, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource4)
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        goto LAB_00d128a3
    end
    goto FLOW_past_lab_00d128a3
    ::LAB_00d128a3::
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::FLOW_past_lab_00d128a3::
    quest:DeregisterTimer(timerId)
    ::LAB_00d128bf::
    resources:ReleaseResource(resource6)
    resources:ReleaseResource(resource5)
    resources:ReleaseResource(resource7)
end

-- AssassinMarker.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- AssassinMarker.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- AssassinMarker.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

