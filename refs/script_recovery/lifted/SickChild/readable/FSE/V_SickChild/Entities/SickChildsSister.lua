-- Readable native conversion: SickChildsSister. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    SickChildsSisterAttentionAnims = 1868,  -- '0300000088e70400af9e04009d9e0400'
}

-- per-entity fields (native class members; one Lua state per entity instance)
local leadHeroToSickChild_

-- SickChildsSister.Main (retail 0x00ec68a0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isTalkedToByHero, scratchValue7, isPerformingScriptTask, sequence1
    local nextPostIntroReminderNumber, movie, hero2, line, postIntroReminderNumber, getPos
    local sickChildsMother, movie5, movie6, resource, registerTimer, timerId
    local function ReleaseEverything()
        quest:DeregisterTimer(registerTimer)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything3()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if not leadHeroToSickChild_ and not quest:GetStateBool("MotherIntroDone") then
        if not quest:IsActiveThreadTerminating() then
            local hero3 = quest:GetHero()
            me:FollowThing(hero3, 1.0, true)
            timerId = quest:RegisterTimer()
            ::LAB_00ec69d4::
            registerTimer = "SCRIPT_NAME_HERO"
            isTalkedToByHero = me:IsTalkedToByHero()
            sequence1 = isTalkedToByHero
            if not sequence1 then
                isTalkedToByHero = true
                sequence1 = quest:GetStateBool("MotherIntroDone")
            end
            if isTalkedToByHero and not sequence1 then
                if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                if quest:GetTimer(timerId) == 0 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    local hero4 = quest:GetHero()
                    me:StopFollowingThing(hero4)
                    local conversationId = quest:AddNewConversation(me, false, false)
                    local hero5 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId, hero5)
                    local hero6 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B10_SISTER_CALLS_OVER_10", me, hero6, false)
                    local scratchValue9 = quest:ReadGlobalGameData(1872)
                    local scratchValue21 = quest:ReadGlobalGameData(SCRIPT_DEF.SickChildsSisterAttentionAnims)
                    local randomChoice = quest:RetailRandModulo(32768)
                    local scratchValue38 = quest:ReadGlobalGameDataDefStringAt(1868, randomChoice % ((scratchValue9 - scratchValue21) >> 2))
                    resources:PlayAnimationWithNativeArgument5(resource, scratchValue38, false, false, false, true, false, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    quest:SetTimer(timerId, 10)
                    local hero7 = quest:GetHero()
                    me:FollowThing(hero7, 1.0, true)
                end
                if resources:IsHitByHeroExceptAbility(me, 14) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    local hero8 = quest:GetHero()
                    me:StopFollowingThing(hero8)
                    movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero8 = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        line = "TEXT_QST_B10_SISTER_ON_HIT"
                        local hero9 = quest:GetHero()
                        if not me:Speak(hero9, line, 2, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    local hero10 = quest:GetHero()
                    me:FollowThing(hero10, 1.0, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                end
                goto LAB_00ec69d4
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            hero2 = quest:GetHero()
            me:StopFollowingThing(hero2)
            if quest:GetStateBool("MotherIntroDone") then goto LAB_00ec734b end
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            quest:FixMovieSequenceCamera(true)
            quest:CameraUseCameraPoint("CAM_SC_GIRL", me, -1.0, 0, -1)
            movie6 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            hero2 = nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                line = "TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20"
                hero2 = quest:GetHero()
                if not me:Speak(hero2, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie6)
            sickChildsMother = quest:GetThingWithScriptName("SickChildsMother")
            quest:SetThingHasInformation(me, false, false, false)
            scratchValue7 = 0
            isPerformingScriptTask = 0
            registerTimer = quest:RegisterTimer()
            me:ClearCommands()
            if not (sickChildsMother ~= nil and not sickChildsMother:IsNull()) then
                getPos = {x = 0, y = 0, z = 0}
            else
                getPos = sickChildsMother:GetPos()
            end
            me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
            if quest:IsDistanceBetweenThingsUnder(me, sickChildsMother, 2.0) then
                goto LAB_00ec7326
            end
            goto FLOW_past_lab_00ec7326
            ::LAB_00ec7326::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:DeregisterTimer(registerTimer)
            leadHeroToSickChild_ = true
            goto LAB_00ec734b
            ::FLOW_past_lab_00ec7326::
            goto FLOW_past_lab_00ec734b
            ::LAB_00ec734b::
            quest:DeregisterTimer(timerId)
            goto LAB_00ec7354
            ::FLOW_past_lab_00ec734b::
            repeat
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                local scratchValue37 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(me, sickChildsMother)))
                local hero11 = quest:GetHero()
                local scratchValue = resources:DistanceBetweenThings(hero11, sickChildsMother) <= scratchValue37
                hero2 = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0) or scratchValue then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    hero2 = quest:GetHero()
                    if quest:IsDistanceBetweenThingsUnder(me, hero2, 11.0) or scratchValue then
                        if not quest:IsActiveThreadTerminating() then
                            if isPerformingScriptTask ~= 0 then
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                me:ClearCommands()
                                if not (sickChildsMother ~= nil and not sickChildsMother:IsNull()) then
                                    getPos = {x = 0, y = 0, z = 0}
                                else
                                    getPos = sickChildsMother:GetPos()
                                end
                                me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_RUN, false, true)
                                isPerformingScriptTask = 0
                            end
                            goto LAB_00ec7467
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        if isPerformingScriptTask ~= 1 then
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            me:ClearCommands()
                            if not (sickChildsMother ~= nil and not sickChildsMother:IsNull()) then
                                getPos = {x = 0, y = 0, z = 0}
                            else
                                getPos = sickChildsMother:GetPos()
                            end
                            me:MoveToPosition(getPos, 1.0, ENTITY_MOVE_WALK, false, true)
                            isPerformingScriptTask = 1
                        end
                        goto LAB_00ec7467
                    end
                    goto FLOW_past_lab_00ec7467
                    ::LAB_00ec7467::
                    scratchValue7 = 0
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        me:ClearCommands()
                        movie6 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        hero2 = nil
                        if not resources:ResourceThingHealthAbove(resource, 0.0) then
                            goto LAB_00ec75b3
                        end
                        goto FLOW_past_lab_00ec75b3
                        ::LAB_00ec75b3::
                        isPerformingScriptTask = 2
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto LAB_00ec75cd
                        ::FLOW_past_lab_00ec75b3::
                        line = "TEXT_QST_B10_SISTER_THIS_WAY"
                        hero2 = quest:GetHero()
                        if not me:Speak(hero2, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie6
                            goto LAB_00ec7821
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie6
                                goto LAB_00ec7821
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00ec75b3 end
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie6
                        ::LAB_00ec7821::
                        resources:DestroyMovie(movie)
                        ReleaseEverything(); return
                    else
                        goto LAB_00ec75cd
                    end
                    goto FLOW_past_lab_00ec75cd
                    ::LAB_00ec75cd::
                    if not resources:IsHitByHeroExceptAbility(me, 14) then goto LAB_00ec730e end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00ec75cd end
                    me:ClearCommands()
                    movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero2 = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        line = "TEXT_QST_B10_SISTER_ON_HIT"
                        hero2 = quest:GetHero()
                        if not me:Speak(hero2, line, 2, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            resources:DestroyMovie(movie)
                            ReleaseEverything(); return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie5
                                resources:DestroyMovie(movie)
                                ReleaseEverything(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            resources:DestroyMovie(movie)
                            ReleaseEverything(); return
                        end
                    end
                    isPerformingScriptTask = 2
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00ec730e
                    ::FLOW_past_lab_00ec75cd::
                    ::FLOW_past_lab_00ec7467::
                    ReleaseEverything()
                    return
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                me:ClearCommands()
                me:ClearAllActions()
                if scratchValue7 == 0 then
                    if not quest:IsActiveThreadTerminating() then
                        local hero12 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, hero12, false)
                        local conversationId2 = quest:AddNewConversation(me, false, false)
                        local hero13 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId2, hero13)
                        hero2 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_B10_SISTER_THIS_WAY", me, hero2, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:Pause(0.8)
                            resources:PlayAnimationWithNativeArgument5(resource, "STANDARD_WAVE", false, false, false, true, false, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue7 = 1
                                isPerformingScriptTask = 2
                                quest:SetTimer(registerTimer, 5)
                                goto LAB_00ec72e4
                            end
                        end
                    end
                    ReleaseEverything(); return
                end
                ::LAB_00ec72e4::
                if quest:GetTimer(registerTimer) == 0 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    scratchValue7 = 0
                end
                ::LAB_00ec730e::
                if quest:IsDistanceBetweenThingsUnder(me, sickChildsMother, 2.0) then goto LAB_00ec7326 end
            until false
        end
    else
        goto LAB_00ec7354
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ec7354::
    quest:ClearThingHasInformation(me)
    nextPostIntroReminderNumber = 10
    postIntroReminderNumber = 10
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            if quest:GetStateBool("FinishedQuest") then
                if not quest:IsActiveThreadTerminating() then
                    movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero2 = nil
                    if not resources:ResourceThingHealthAbove(resource, 0.0) then
                        goto LAB_00ec79b4
                    end
                    goto FLOW_past_lab_00ec79b4
                    ::LAB_00ec79b4::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00ec7cd4
                    ::FLOW_past_lab_00ec79b4::
                    line = "TEXT_QST_B10_SISTER_THANKS"
                    hero2 = quest:GetHero()
                    if not me:Speak(hero2, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie5
                        ReleaseEverything3(); return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            ReleaseEverything3(); do return end
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00ec79b4 end
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie5
                    ReleaseEverything3()
                    return
                end
                break
            end
            if quest:GetStateBool("MotherIntroDone") then
                if not quest:IsActiveThreadTerminating() then
                    registerTimer = "TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. tostring(postIntroReminderNumber)
                    if not quest:TextEntryExists(registerTimer) then
                        postIntroReminderNumber = 10
                        registerTimer = "TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. tostring(10)
                    end
                    nextPostIntroReminderNumber = ((postIntroReminderNumber + 10 + 0x80000000) & 0xffffffff) - 0x80000000
                    local movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    sickChildsMother, hero2 = nil, nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        local p1 = registerTimer
                        local hero = quest:GetHero()
                        if not me:Speak(hero, p1, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    goto LAB_00ec7cd4
                end
                break
            end
            if quest:IsActiveThreadTerminating() then break end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            hero2 = nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                line = "TEXT_QST_B10_SISTER_REMINDER"
                hero2 = quest:GetHero()
                if not me:Speak(hero2, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie2
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie2
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie2
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        ::LAB_00ec7cd4::
        if resources:IsHitByHeroExceptAbility(me, 14) then
            if quest:IsActiveThreadTerminating() then break end
            local movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            movie6, hero2 = nil, nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                line = "TEXT_QST_B10_SISTER_ON_HIT"
                hero2 = quest:GetHero()
                if not me:Speak(hero2, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    ReleaseEverything3(); return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
        end
        quest:NewScriptFrame(me)
        postIntroReminderNumber = nextPostIntroReminderNumber
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- SickChildsSister.Init (retail 0x00ec6860)
function Init(quest, me)
    leadHeroToSickChild_ = false
    quest:SetThingHasInformation(me, false, true, false)
end

-- SickChildsSister.OnPersist (retail 0x00ecd860)
function OnPersist(quest, me, context)
    local leadHeroToSickChild = leadHeroToSickChild_ or false
    leadHeroToSickChild_ = quest:PersistTransferBool(context, "LeadHeroToSickChild", leadHeroToSickChild)
end

-- SickChildsSister.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

