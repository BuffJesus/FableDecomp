-- Readable native conversion: AssassinMarker. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- AssassinMarker.Main (retail 0x00d11930)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, scratchValue, getStateBool, ctr_c, scratchValue2, scratchValue3
    local scratchValue4, scratchValue5, scratchValue6, addNewConversation, sequence1, thing, line
    local assassin3, speechResult, speechResult2, speechResult3, resource4, scriptThing, actorMap
    local movie
    quest:NewScriptFrame(me)
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then
        return
    end
    local assassin1 = quest:GetThingWithScriptName("Assassin1")
    local assassin = quest:GetThingWithScriptName("Assassin2")
    assassin3 = quest:GetThingWithScriptName("Assassin3")
    quest:EntitySetAsKillable(assassin1, false, true)
    quest:EntitySetAsKillable(assassin, false, true)
    quest:EntitySetAsKillable(assassin3, false, true)
    local resource7 = resources:NewResource()
    local resource5 = resources:NewResource()
    local resource6 = resources:NewResource()
    isActiveThreadTerminating = false
    addNewConversation = 0
    ctr_c = 0
    scratchValue6 = quest:RegisterTimer()
    local timerId = scratchValue6
    quest:SetTimer(timerId, 0)
    getStateBool = quest:GetStateBool("AssassinCutsceneTriggered")
    while not getStateBool and not isActiveThreadTerminating do
        quest:NewScriptFrame(me)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            quest:DeregisterTimer(timerId)
            goto LAB_00d128bf
        end
        if not quest:GetStateBool("AssassinsUnderAttack") then
            local dist = 4.0
            thing = hero
            scratchValue = quest:IsDistanceBetweenThingsUnder(me, thing, dist)
            isActiveThreadTerminating = false
            sequence1 = false
            sequence1 = not scratchValue
            if not sequence1 then
                scratchValue = quest:IsConversationActive(addNewConversation)
                sequence1 = scratchValue
            end
            sequence1 = sequence1 or quest:GetStateBool("TalkedToAssassin")
            if sequence1 then goto LAB_00d12226 end
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then goto LAB_00d12245 end
            -- TODO(native): switch(ctr_c8) {
            -- TODO(native): case (CCharString)0x0:
            addNewConversation = quest:AddNewConversation(assassin1, false, false)
            quest:AddPersonToConversation(addNewConversation, assassin)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_ONE", assassin1, assassin, false)
            break
            -- TODO(native): case (CCharString)0x1:
            addNewConversation = quest:AddNewConversation(assassin, false, false)
            quest:AddPersonToConversation(addNewConversation, assassin1)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN2_LISTENED_ONE", assassin, assassin1, false)
            break
            -- TODO(native): case (CCharString)0x2:
            addNewConversation = quest:AddNewConversation(assassin1, false, false)
            quest:AddPersonToConversation(addNewConversation, assassin)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_TWO", assassin1, assassin, false)
            break
            -- TODO(native): case (CCharString)0x3:
            addNewConversation = quest:AddNewConversation(assassin, false, false)
            quest:AddPersonToConversation(addNewConversation, assassin1)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN2_LISTENED_TWO", assassin, assassin1, false)
            break
            -- TODO(native): case (CCharString)0x4:
            addNewConversation = quest:AddNewConversation(assassin1, false, false)
            thing = hero
            quest:AddPersonToConversation(addNewConversation, thing)
            thing = hero
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_THREE", assassin1, thing, false)
            break
            -- TODO(native): case (CCharString)0x5:
            addNewConversation = quest:AddNewConversation(assassin1, false, false)
            thing = hero
            quest:AddPersonToConversation(addNewConversation, thing)
            thing = hero
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_ASSASSIN1_LISTENED_FOUR", assassin1, thing, false)
            break
            -- TODO(native): default:
            -- TODO(native): goto switchD_00d11fb1_default;
        end
        -- TODO(native): switchD_00d11fb1_default:
        ctr_c = ctr_c + 1
    else
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then goto LAB_00d12245 end
        resources:TryAcquire(resource7, assassin1, 4)
        resources:TryAcquire(resource5, assassin, 4)
        resources:TryAcquire(resource6, assassin3, 4)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        resource4 = resources:ScriptThing(resource7)
        thing = resource4
        local fret_0 = quest:GetHealth(thing)
        scratchValue2 = 0.0
        resource4 = nil
        if scratchValue2 < fret_0 then
            scratchValue5 = 0
            scratchValue4 = 1
            scratchValue3 = 0
            scratchValue6 = 0
            line = "TEXT_QST_009_ASSASSIN1_ATTACKED"
            thing = hero
            speechResult = me:Speak(thing, line, scratchValue6, scratchValue3 ~= 0, scratchValue4 ~= 0, scratchValue5 ~= 0)
            scratchValue6 = me:IsPerformingScriptTask()
            getStateBool = scratchValue6
            while getStateBool do
                quest:NewScriptFrame(me)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if not isActiveThreadTerminating then scratchValue6 = me:IsPerformingScriptTask(); getStateBool = scratchValue6; goto continue_1 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource6)
                resources:ReleaseResource(resource5)
                resources:ReleaseResource(resource7)
                assassin3 = nil
                do return end
                scratchValue6 = me:IsPerformingScriptTask()
                getStateBool = scratchValue6
                ::continue_1::
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not isActiveThreadTerminating then goto LAB_00d11c9b end
            goto LAB_00d1244c
        end
        goto FLOW_past_lab_00d1244c
        ::LAB_00d1244c::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(timerId)
        goto LAB_00d128bf
        ::FLOW_past_lab_00d1244c::
        ::LAB_00d11c9b::
        scriptThing = resources:ScriptThing(resource5)
        thing = scriptThing
        local fret_00 = quest:GetHealth(thing)
        scratchValue2 = 0.0
        scriptThing = nil
        if scratchValue2 < fret_00 then
            scratchValue5 = 0
            scratchValue4 = 1
            scratchValue3 = 0
            scratchValue6 = 0
            line = "TEXT_QST_009_ASSASSIN2_ATTACKED"
            thing = hero
            speechResult2 = me:Speak(thing, line, scratchValue6, scratchValue3 ~= 0, scratchValue4 ~= 0, scratchValue5 ~= 0)
            scratchValue6 = me:IsPerformingScriptTask()
            getStateBool = scratchValue6
            while getStateBool do
                quest:NewScriptFrame(me)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if not isActiveThreadTerminating then scratchValue6 = me:IsPerformingScriptTask(); getStateBool = scratchValue6; goto continue_2 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource6)
                resources:ReleaseResource(resource5)
                resources:ReleaseResource(resource7)
                assassin3 = nil
                do return end
                scratchValue6 = me:IsPerformingScriptTask()
                getStateBool = scratchValue6
                ::continue_2::
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then goto LAB_00d1244c end
        end
        actorMap = resources:ScriptThing(resource6)
        thing = actorMap
        local fret_01 = quest:GetHealth(thing)
        scratchValue2 = 0.0
        actorMap = nil
        if scratchValue2 < fret_01 then
            scratchValue5 = 0
            scratchValue4 = 1
            scratchValue3 = 0
            scratchValue6 = 0
            line = "TEXT_QST_009_ASSASSIN3_ATTACKED"
            thing = hero
            speechResult3 = me:Speak(thing, line, scratchValue6, scratchValue3 ~= 0, scratchValue4 ~= 0, scratchValue5 ~= 0)
            scratchValue6 = me:IsPerformingScriptTask()
            getStateBool = scratchValue6
            while getStateBool do
                quest:NewScriptFrame(me)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if not isActiveThreadTerminating then scratchValue6 = me:IsPerformingScriptTask(); getStateBool = scratchValue6; goto continue_3 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DeregisterTimer(timerId)
                goto LAB_00d128bf
                scratchValue6 = me:IsPerformingScriptTask()
                getStateBool = scratchValue6
                ::continue_3::
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then goto LAB_00d1244c end
        end
        thing = hero
        quest:GiveThingBestEnemyTarget(assassin1, thing)
        thing = hero
        quest:GiveThingBestEnemyTarget(assassin, thing)
        thing = hero
        quest:GiveThingBestEnemyTarget(assassin3, thing)
        isActiveThreadTerminating = true
        resources:PrepareResource(resource7)
        resources:PrepareResource(resource5)
        resources:PrepareResource(resource6)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    ::LAB_00d12226::
    getStateBool = quest:GetStateBool("AssassinCutsceneTriggered")
    scratchValue6 = timerId
    end
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then
        goto LAB_00d12245
    end
    goto FLOW_past_lab_00d12245
    ::LAB_00d12245::
    quest:DeregisterTimer(timerId)
    goto LAB_00d128bf
    ::FLOW_past_lab_00d12245::
    quest:EntitySetAsKillable(assassin1, true, true)
    quest:EntitySetAsKillable(assassin, true, true)
    quest:EntitySetAsKillable(assassin3, true, true)
    if not quest:GetStateBool("AssassinCutsceneTriggered") then
        goto LAB_00d128a3
    else
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if not isActiveThreadTerminating then
            local resource = resources:NewResource()
            local resource2 = resources:NewResource()
            local resource3 = resources:NewResource()
            resource4 = resources:NewResource()
            scratchValue6 = 4
            local pScriptObject = resource
            thing = hero
            resources:TryAcquire(pScriptObject, thing, scratchValue6)
            resources:TryAcquire(resource2, assassin1, 4)
            resources:TryAcquire(resource3, assassin, 4)
            resources:TryAcquire(resource4, assassin3, 4)
            quest:EntitySetInFaction(assassin1, "FACTION_TWINBLADE_CAMP_BANDITS")
            quest:EntitySetInFaction(assassin, "FACTION_TWINBLADE_CAMP_BANDITS")
            quest:EntitySetInFaction(assassin3, "FACTION_TWINBLADE_CAMP_BANDITS")
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource)
            resources:SetActor(actorMap, "ASS1", resource2)
            resources:SetActor(actorMap, "ASS2", resource3)
            resources:SetActor(actorMap, "ASS3", resource4)
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_BANDITCAMP_ASSATTACK", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("Gate3Open", true)
            scratchValue = true
            isActiveThreadTerminating = false
            thing = quest:GetThingWithScriptName("Gate3Guard")
            quest:RemoveThing(thing, isActiveThreadTerminating, scratchValue)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource4)
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            goto LAB_00d128a3
        end
    end
    goto FLOW_past_lab_00d128a3
    ::LAB_00d128a3::
    repeat
        quest:NewScriptFrame(me)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    until isActiveThreadTerminating
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

