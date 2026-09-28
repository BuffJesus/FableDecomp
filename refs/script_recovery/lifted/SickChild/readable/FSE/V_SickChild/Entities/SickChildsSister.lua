-- Readable native conversion: SickChildsSister. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local leadHeroToSickChild

-- SickChildsSister.Main (retail 0x00ec68a0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, msgIsHitByAnySpecialAbilityFromHero, isTalkedToByHero, predicateResult6
    local predicateResult, predicateResult2, scratchValue5, scratchValue7, isPerformingScriptTask
    local sequence1, movie, getPos, sickChildsMother, value, movie5, movie6, resource, registerTimer
    local timerId
    local function ReleaseEverything()
        local movie = movie5
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        quest:DeregisterTimer(registerTimer)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything3()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    scratchValue5 = 0
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
    if not leadHeroToSickChild and not quest:GetStateBool("MotherIntroDone") then
        if not quest:IsActiveThreadTerminating() then
            me:FollowThing(hero, 1.0, true)
            timerId = quest:RegisterTimer()
            ::LAB_00ec69d4::
            isTalkedToByHero = me:IsTalkedToByHero()
            sequence1 = isTalkedToByHero
            if not sequence1 then
                isTalkedToByHero = true
                sequence1 = quest:GetStateBool("MotherIntroDone")
            end
            if isTalkedToByHero and not sequence1 then
                if not quest:NewScriptFrame(me) then ReleaseEverything3(); return end
                if quest:GetTimer(timerId) == 0 then
                    me:StopFollowingThing(hero)
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B10_SISTER_CALLS_OVER_10", me, hero, false)
                    quest:ReadGlobalGameData(1872)
                    quest:ReadGlobalGameData(1868)
                    math.random(0, 32767)
                    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x74c) + (uVar8 % (uint)(iVar17 - iVar19 >> 2)) * 4),(int)&xStack_94);
                    me:PlayAnimation(nil --[[missing]], false, false, false, true, true, false, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
                    quest:SetTimer(timerId, 10)
                    me:FollowThing(hero, 1.0, true)
                end
                scratchValue = scratchValue5 | 2
                if me:MsgIsHitByHero() then
                    goto LAB_00ec6c50
                else
                    scratchValue = scratchValue5 | 6
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue = 14
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec6c50 end
                    end
                    predicateResult6 = false
                end
                goto FLOW_past_lab_00ec6c50
                ::LAB_00ec6c50::
                predicateResult6 = true
                ::FLOW_past_lab_00ec6c50::
                if scratchValue & 8 ~= 0 then
                    scratchValue = scratchValue & 247
                end
                if scratchValue & 4 ~= 0 then
                    scratchValue = scratchValue & 251
                end
                scratchValue5 = scratchValue
                if scratchValue & 2 ~= 0 then
                    scratchValue5 = scratchValue & 253
                end
                if predicateResult6 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
                    me:StopFollowingThing(hero)
                    movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_B10_SISTER_ON_HIT", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
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
                    me:FollowThing(hero, 1.0, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                end
                goto LAB_00ec69d4
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            me:StopFollowingThing(hero)
            if quest:GetStateBool("MotherIntroDone") then goto LAB_00ec734b end
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            quest:FixMovieSequenceCamera(true)
            quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
            movie6 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetHealth(resources:ScriptThing(resource)) > 0.0 then
                me:Speak(hero, "TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20", GROUP_SELECT_FIRST, false, true, false)
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
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            quest:DeregisterTimer(registerTimer)
            leadHeroToSickChild = true
            goto LAB_00ec734b
            ::FLOW_past_lab_00ec7326::
            goto FLOW_past_lab_00ec734b
            ::LAB_00ec734b::
            quest:DeregisterTimer(timerId)
            goto LAB_00ec7354
            ::FLOW_past_lab_00ec734b::
            repeat
                if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                msgIsHitByAnySpecialAbilityFromHero = (quest:GetDistanceBetweenThings(hero, sickChildsMother) ^ 2) <= (quest:GetDistanceBetweenThings(me, sickChildsMother) ^ 2)
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) or msgIsHitByAnySpecialAbilityFromHero then
                    if quest:IsDistanceBetweenThingsUnder(me, hero, 11.0) or msgIsHitByAnySpecialAbilityFromHero then
                        if not quest:IsActiveThreadTerminating() then
                            if isPerformingScriptTask ~= 0 then
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
                        if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                        me:ClearCommands()
                        movie6 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                            goto LAB_00ec75b3
                        end
                        goto FLOW_past_lab_00ec75b3
                        ::LAB_00ec75b3::
                        isPerformingScriptTask = 2
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto LAB_00ec75cd
                        ::FLOW_past_lab_00ec75b3::
                        me:Speak(hero, "TEXT_QST_B10_SISTER_THIS_WAY", GROUP_SELECT_FIRST, false, true, false)
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
                    else
                        goto LAB_00ec75cd
                    end
                    ::FLOW_after_lab_00ec7821::
                    goto FLOW_past_lab_00ec75cd
                    ::LAB_00ec75cd::
                    scratchValue = scratchValue5 | 16
                    if me:MsgIsHitByHero() then
                        goto LAB_00ec765b
                    else
                        scratchValue = scratchValue5 | 48
                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                            scratchValue = scratchValue5 | 112
                            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec765b end
                        end
                        scratchValue5 = scratchValue
                        predicateResult = false
                    end
                    goto FLOW_past_lab_00ec765b
                    ::LAB_00ec765b::
                    scratchValue5 = scratchValue
                    predicateResult = true
                    ::FLOW_past_lab_00ec765b::
                    if scratchValue5 & 64 ~= 0 then
                        scratchValue5 = scratchValue5 & 191
                    end
                    if scratchValue5 & 32 ~= 0 then
                        scratchValue5 = scratchValue5 & 223
                    end
                    if scratchValue5 & 16 ~= 0 then
                        scratchValue5 = scratchValue5 & 239
                    end
                    if not predicateResult then goto LAB_00ec730e end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00ec75cd end
                    me:ClearCommands()
                    movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_B10_SISTER_ON_HIT", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                ReleaseEverything(); return  -- TODO(native): goto FLOW_after_lab_00ec7821
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            ReleaseEverything()
                            goto FLOW_after_lab_00ec7821
                        end
                    end
                    isPerformingScriptTask = 2
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00ec730e
                    ::FLOW_past_lab_00ec75cd::
                    ::FLOW_past_lab_00ec7467::
                    ReleaseEverything2()
                    return
                end
                me:ClearCommands()
                me:ClearAllActions()
                if scratchValue7 == 0 then
                    if not quest:IsActiveThreadTerminating() then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        local conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, hero)
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_B10_SISTER_THIS_WAY", me, hero, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:Pause(0.8)
                            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue7 = 1
                                isPerformingScriptTask = 2
                                quest:SetTimer(registerTimer, 5)
                                goto LAB_00ec72e4
                            end
                        end
                    end
                    ReleaseEverything2(); return
                end
                ::LAB_00ec72e4::
                if quest:GetTimer(registerTimer) == 0 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
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
    msgIsHitByAnySpecialAbilityFromHero = false
    quest:ClearThingHasInformation(me)
    value = 10
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            if quest:GetStateBool("FinishedQuest") then
                if not quest:IsActiveThreadTerminating() then
                    movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                        goto LAB_00ec79b4
                    end
                    goto FLOW_past_lab_00ec79b4
                    ::LAB_00ec79b4::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00ec7cd4
                    ::FLOW_past_lab_00ec79b4::
                    if not me:Speak(hero, "TEXT_QST_B10_SISTER_THANKS", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00ec7fd1
                    end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00ec79b4 end
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00ec7fd1
                end
            else
                goto FLOW_past_lab_00ec7fd1
                -- LAB_00ec7fd1: (native jump target)
                resources:DestroyMovie(movie)
            end
            break
            ::FLOW_past_lab_00ec7fd1::
            if quest:GetStateBool("MotherIntroDone") then
                if not quest:IsActiveThreadTerminating() then
                    registerTimer = "TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. tostring(value)
                    if not quest:TextEntryExists(registerTimer) then
                        value = 10
                        registerTimer = "TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. tostring(10)
                    end
                    -- TODO(native): xStack_88 = (CCharString)((int)value + 0xa);
                    local movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, registerTimer, 0, false, true, false)
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
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_B10_SISTER_REMINDER", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00ec7fd1
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00ec7fd1
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        ::LAB_00ec7cd4::
        if me:MsgIsHitByHero() then
            goto LAB_00ec7d68
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                msgIsHitByAnySpecialAbilityFromHero = true
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec7d68 end
            end
            msgIsHitByAnySpecialAbilityFromHero = true
            predicateResult2 = false
        end
        goto FLOW_past_lab_00ec7d68
        ::LAB_00ec7d68::
        predicateResult2 = true
        ::FLOW_past_lab_00ec7d68::
        scratchValue = scratchValue5 | 128
        msgIsHitByAnySpecialAbilityFromHero = msgIsHitByAnySpecialAbilityFromHero and false
        if scratchValue & 128 ~= 0 then
            scratchValue = scratchValue5 & 127
        end
        if predicateResult2 then
            if quest:IsActiveThreadTerminating() then break end
            local movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_B10_SISTER_ON_HIT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec7fca end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec7fca end
                goto FLOW_past_lab_00ec7fca
                ::LAB_00ec7fca::
                -- TODO(native): goto LAB_00ec7fd1
                ::FLOW_past_lab_00ec7fca::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
        end
        quest:NewScriptFrame(me)
        scratchValue5 = scratchValue
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- SickChildsSister.Init (retail 0x00ec6860)
function Init(quest, me)
    leadHeroToSickChild = false
    quest:SetThingHasInformation(me, false, true, false)
end

-- SickChildsSister.OnPersist (retail 0x00ecd860)
function OnPersist(quest, me, context)
    quest:SetStateBool("LeadHeroToSickChild", quest:PersistTransferBool(context, "LeadHeroToSickChild", quest:GetStateBool("LeadHeroToSickChild")))
end

-- SickChildsSister.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

