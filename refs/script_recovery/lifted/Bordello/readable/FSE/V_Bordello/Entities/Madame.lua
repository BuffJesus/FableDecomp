-- Readable native conversion: Madame. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local doneIntro_, mentionedNunnery

-- Madame.Main (retail 0x00e3bb70)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, __push10, __push14, __push15, __push9, scratchValue3, scratchValue6
    local scratchValue7, ctr_140, health, scratchValue8, scratchValue9, scratchValue10
    local scratchValue11, timerId, getTimer, scratchValue12, timerId2, sequence1, sequence, p0
    local movie, scratchValue13, scratchValue14, line, line2, this_00, scratchValue17
    local scratchValue18, scratchValue19, startMovie, startMovie2, newResource, scratchValue22
    local scratchValue23, movie3, movie4, movie5, movie6, predicateResult, scriptThing, scriptThing2
    local scriptThing3
    scriptThing3 = 0
    quest:NewScriptFrame(me)
    scratchValue6 = quest:IsActiveThreadTerminating()
    if scratchValue6 then
        return
    end
    if not quest:GetStateBool("BecomeNunnery") then
        scratchValue6 = quest:IsActiveThreadTerminating()
        if scratchValue6 then
            return
        end
        scratchValue12 = 1
        quest:SetThingHasInformation(me, true, false, false)
    end
    local scratchValue15 = 0
    ctr_140 = nil
    quest:SetIsPushableByHero(me, scratchValue15 ~= 0)
    timerId2 = 0
    scratchValue22 = p0
    quest:EntitySetAsKillable(me, false, false)
    startMovie = p0
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    newResource = resources:NewResource()
    local madameResource = newResource
    resources:PrepareResource(madameResource)
    startMovie2 = newResource
    local scratchValue21 = p0
    scratchValue7 = resources:TryAcquire(madameResource, me, 4)
    while not scratchValue7 do
        quest:NewScriptFrame(me)
        scratchValue6 = quest:IsActiveThreadTerminating()
        if scratchValue6 then resources:ReleaseResource(madameResource); return end
        scratchValue18 = p0
        scratchValue7 = resources:TryAcquire(madameResource, me, 4)
    end
    scratchValue6 = quest:IsActiveThreadTerminating()
    if scratchValue6 then resources:ReleaseResource(madameResource); return end
    resources:AssignResource(resources:MemberResource("seh_Madam"), madameResource)
    timerId = quest:RegisterTimer()
    timerId2 = timerId
    quest:SetTimer(timerId, 2)
    if not doneIntro_ then
        scratchValue6 = quest:IsActiveThreadTerminating()
        if not scratchValue6 then
            startMovie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
            if scratchValue6 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if not scratchValue6 then
                    line = "TEXT_CS_B13_INTRO_HEROLADY_10"
                    goto LAB_00e3bde9
                end
                goto FLOW_hoist_lab_00e3bde9_1
            else
                scratchValue6 = quest:IsActiveThreadTerminating()
                if not scratchValue6 then
                    line = "TEXT_CS_B13_INTRO_10"
                    goto LAB_00e3bde9
                end
                quest:PauseAllNonScriptedEntities(false)
                movie = startMovie2
            end
            goto FLOW_past_lab_00e3bde9
            ::LAB_00e3bde9::
            resources:SetString(resources:MemberStringMap("csargs"), "$LADYINTRO", line)
            helpers.PlayCutscene(quest, me, "CS_BORDELLO_INTRO", false)
            doneIntro_ = true
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(startMovie2)
            goto LAB_00e3be37
            ::FLOW_hoist_lab_00e3bde9_1::
            quest:PauseAllNonScriptedEntities(false)
            movie = startMovie2
            ::FLOW_past_lab_00e3bde9::
            goto LAB_00e3e2ea
        end
    else
        goto LAB_00e3be37
    end
    goto FLOW_past_lab_00e3be37
    ::LAB_00e3be37::
    scratchValue7 = quest:GetStateBool("PlayerOwned")
    while not scratchValue7 do
        quest:NewScriptFrame(me)
        scratchValue6 = quest:IsActiveThreadTerminating()
        timerId = timerId2
        if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
        getTimer = quest:GetTimer(timerId2)
        if getTimer == 0 then
            scratchValue8 = 8.0
            scratchValue14 = quest:GetHero()
            scratchValue6 = quest:IsDistanceBetweenThingsUnder(me, scratchValue14, scratchValue8)
            if scratchValue6 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                quest:SetTimer(timerId, 4)
                local __push1 = quest:GetHero()
                scratchValue18 = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
            end
        end
        scratchValue22 = ""
        g_PresentedItemName = nil
        scratchValue6 = me:MsgIsPresentedWithItem()
        if scratchValue6 then scratchValue22 = g_PresentedItemName end
        if scratchValue6 then
            if scratchValue22 == nil then
                scratchValue6 = false
                if scratchValue6 then
                    goto LAB_00e3bf28
                end
            else
                local scratchValue16 = scratchValue22
                timerId = scratchValue16 == "OBJECT_BEER_TANKARD" and 0 or 1
                scratchValue17 = timerId == 0
                if scratchValue17 then goto LAB_00e3bf28 end
            end
            goto FLOW_past_lab_00e3bf28
            ::LAB_00e3bf28::
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
            movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue3 = resources:ScriptThing(madameResource)
            scratchValue14 = scratchValue3
            health = quest:GetHealth(scratchValue14)
            scratchValue = scratchValue21
            scratchValue17 = health > 0.0
            if scratchValue17 then
                scratchValue11 = 0
                scratchValue10 = 1
                scratchValue9 = 0
                getTimer = 0
                line = "TEXT_QST_B13_MADAME_GIVEN_BEER"
                timerId = quest:GetHero()
                local speechResult = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                if not speechResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(madameResource)
                    return
                end
                timerId = me:IsPerformingScriptTask()
                scratchValue7 = timerId
                while scratchValue7 do
                    quest:NewScriptFrame(me)
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if not scratchValue6 then timerId = me:IsPerformingScriptTask(); scratchValue7 = timerId; goto continue_1 end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(madameResource)
                    do return end
                    timerId = me:IsPerformingScriptTask()
                    scratchValue7 = timerId
                    ::continue_1::
                end
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie5
                    goto LAB_00e3e2ea
                end
            end
            quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie5)
            ::FLOW_past_lab_00e3bf28::
        end
        scratchValue17 = me:IsTalkedToByHero()
        if scratchValue17 then
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
            sequence1 = false
            sequence1 = not quest:GetStateBool("HeroTricking")
            if sequence1 then
                scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
                sequence1 = scratchValue6
            end
            if sequence1 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local __push2 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push2, false)
                quest:Pause(1.0)
                local __push3 = quest:GetHero()
                newResource = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push3, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
                local scratchValue4 = resources:ScriptThing(madameResource)
                scratchValue14 = scratchValue4
                health = quest:GetHealth(scratchValue14)
                scratchValue7 = 0.0 < health
                if scratchValue7 then
                    scratchValue11 = 0
                    scratchValue10 = 1
                    scratchValue9 = 0
                    getTimer = 0
                    line = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                    timerId = quest:GetHero()
                    local speechResult12 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                    if not speechResult12 then
                        goto LAB_00e3d063
                    end
                    timerId = me:IsPerformingScriptTask()
                    scratchValue7 = timerId
                    while scratchValue7 do
                        quest:NewScriptFrame(me)
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d063 end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                    end
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then goto LAB_00e3d473 end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                while timerId < 0 do
                    quest:NewScriptFrame(me)
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then goto LAB_00e3d063 end
                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then goto LAB_00e3d473 end
                scratchValue6 = quest:IsActiveThreadTerminating()
                if timerId == 1 then
                    if scratchValue6 then goto LAB_00e3d063 end
                    scriptThing = resources:ScriptThing(madameResource)
                    getTimer = scriptThing
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_JOINED"
                        timerId = quest:GetHero()
                        local speechResult16 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        if not speechResult16 then
                            goto LAB_00e3d473
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d063 end
                    end
                    quest:SetStateBool("HeroTricking", true)
                else
                    if scratchValue6 then goto LAB_00e3d473 end
                    scriptThing2 = resources:ScriptThing(madameResource)
                    getTimer = scriptThing2
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                        timerId = quest:GetHero()
                        local speechResult17 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        if not speechResult17 then
                            goto LAB_00e3d063
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d063 end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d473 end
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
            else
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                local __push4 = quest:GetHero()
                scratchValue17 = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", __push4)
                if scratchValue17 then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIAN_LEAVES", false)
                    quest:SetStateBool("PlayerOwned", true)
                    scratchValue14 = quest:GetThingWithScriptName("BordelloHouse")
                    quest:SetHouseOwnedByPlayer(scratchValue14, true, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e3cab0
                end
                sequence = false
                sequence = not quest:GetStateBool("HeroTricking")
                if not sequence then
                    scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
                    sequence = not scratchValue6
                end
                if sequence then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    local __push5 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push5, false)
                    quest:Pause(1.0)
                    local __push6 = quest:GetHero()
                    newResource = me
                    quest:EntitySetFacingAngleTowardsThing(me, __push6, true)
                    quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
                    if 5 < me then
                        scratchValue12 = 1
                    end
                    scratchValue13 = tostring(scratchValue12)
                    local scratchValue20 = "TEXT_QST_B13_MADAME_CHIT_CHAT_0" .. scratchValue13
                    local scratchValue5 = resources:ScriptThing(madameResource)
                    scratchValue14 = scratchValue5
                    health = quest:GetHealth(scratchValue14)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line2 = scratchValue20
                        timerId = quest:GetHero()
                        local speechResult18 = me:Speak(timerId, line2, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        if not speechResult18 then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                            goto LAB_00e3e2ea
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie2
                                goto LAB_00e3e2ea
                            end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                            goto LAB_00e3e2ea
                        end
                    end
                    ctr_140 = ctr_140 + 1
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie2
                else
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
                    scratchValue23 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    local __push7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push7, false)
                    quest:Pause(1.0)
                    local __push8 = quest:GetHero()
                    newResource = me
                    quest:EntitySetFacingAngleTowardsThing(me, __push8, true)
                    quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
                    scriptThing3 = resources:ScriptThing(madameResource)
                    scratchValue14 = scriptThing3
                    health = quest:GetHealth(scratchValue14)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                        timerId = quest:GetHero()
                        local speechResult19 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        if not speechResult19 then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = scratchValue23
                            goto LAB_00e3e2ea
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = scratchValue23
                                goto LAB_00e3e2ea
                            end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = scratchValue23
                            goto LAB_00e3e2ea
                        end
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    movie = scratchValue23
                end
            end
            resources:DestroyMovie(movie)
        end
        ::LAB_00e3cab0::
        scratchValue6 = me:MsgIsHitByHero()
        if scratchValue6 then
            goto LAB_00e3cb3c
        else
            scratchValue6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if scratchValue6 then
                scratchValue6 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                if not scratchValue6 then goto LAB_00e3cb3c end
            end
            scratchValue17 = false
        end
        goto FLOW_past_lab_00e3cb3c
        ::LAB_00e3cb3c::
        scratchValue17 = true
        ::FLOW_past_lab_00e3cb3c::
        if not scratchValue17 then scratchValue7 = quest:GetStateBool("PlayerOwned"); goto continue_2 end
        scratchValue6 = quest:IsActiveThreadTerminating()
        if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(madameResource); return end
        movie6 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        __push9 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, __push9, false)
        quest:Pause(1.0)
        __push10 = quest:GetHero()
        newResource = me
        quest:EntitySetFacingAngleTowardsThing(me, __push10, true)
        quest:NewScriptFrame(me)
        getTimer = -1
        quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
        scratchValue23 = resources:ScriptThing(madameResource)
        scratchValue14 = scratchValue23
        health = quest:GetHealth(scratchValue14)
        scratchValue7 = 0.0 < health
        if scratchValue7 then
            local p5 = 0
            scratchValue11 = 1
            scratchValue10 = 0
            scratchValue9 = 2
            scratchValue13 = helpers.GetHeroStatusTextTag(quest, me)
            scratchValue13 = "TEXT_QST_B13_MADAME_ATTACKED" .. scratchValue13
            line2 = scratchValue13
            timerId = quest:GetHero()
            local speechResult20 = me:Speak(timerId, line2, scratchValue9, scratchValue10 ~= 0, scratchValue11 ~= 0, p5 ~= 0)
            if not speechResult20 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                quest:DeregisterTimer(getTimer)
                resources:ReleaseResource(madameResource)
                return
            end
            timerId = me:IsPerformingScriptTask()
            scratchValue7 = timerId
            while scratchValue7 do
                quest:NewScriptFrame(me)
                scratchValue6 = quest:IsActiveThreadTerminating()
                if not scratchValue6 then timerId = me:IsPerformingScriptTask(); scratchValue7 = timerId; goto continue_3 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                quest:DeregisterTimer(getTimer)
                resources:ReleaseResource(madameResource)
                do return end
                timerId = me:IsPerformingScriptTask()
                scratchValue7 = timerId
                ::continue_3::
            end
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then
                quest:PauseAllNonScriptedEntities(false)
                movie = movie6
                goto LAB_00e3e2ea
            end
        end
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie6)
        scratchValue7 = quest:GetStateBool("PlayerOwned")
        ::continue_2::
    end
    scratchValue6 = quest:IsActiveThreadTerminating()
    if not scratchValue6 then
        scratchValue6 = quest:IsActiveThreadTerminating()
        while true do
            scratchValue19 = timerId
            if scratchValue6 then break end
            getTimer = quest:GetTimer(timerId2)
            if getTimer == 0 then
                scratchValue8 = 8.0
                scratchValue14 = quest:GetHero()
                scratchValue6 = quest:IsDistanceBetweenThingsUnder(me, scratchValue14, scratchValue8)
                if scratchValue6 then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then break end
                    quest:SetTimer(timerId2, 4)
                    local __push11 = quest:GetHero()
                    scratchValue18 = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push11, false)
                end
            end
            scratchValue17 = me:IsTalkedToByHero()
            if scratchValue17 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then break end
                startMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local __push12 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push12, false)
                quest:Pause(1.0)
                local __push13 = quest:GetHero()
                newResource = me
                quest:EntitySetFacingAngleTowardsThing(me, __push13, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
                if not quest:GetStateBool("BecomeNunnery") then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then goto LAB_00e3d473 end
                    scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
                    if scratchValue6 then
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d063 end
                        if not quest:GetStateBool("HeroTricking") then
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            local scratchValue32 = resources:ScriptThing(madameResource)
                            getTimer = scratchValue32
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                                timerId = quest:GetHero()
                                local speechResult21 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                if not speechResult21 then
                                    goto LAB_00e3d063
                                end
                                timerId = me:IsPerformingScriptTask()
                                scratchValue7 = timerId
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d063 end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                end
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d473 end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                            while timerId < 0 do
                                quest:NewScriptFrame(me)
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if timerId == 1 then
                                if scratchValue6 then goto LAB_00e3d063 end
                                if not quest:GetStateBool("PlayerOwned") then
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d063 end
                                    if not quest:GetStateBool("PlayerOwned") then
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d063 end
                                        scriptThing2 = resources:ScriptThing(madameResource)
                                        getTimer = scriptThing2
                                        health = quest:GetHealth(getTimer)
                                        scratchValue7 = 0.0 < health
                                        if scratchValue7 then
                                            scratchValue11 = 0
                                            scratchValue10 = 1
                                            scratchValue9 = 0
                                            getTimer = 0
                                            line = "TEXT_QST_B13_MADAME_HERO_JOINED"
                                            timerId = quest:GetHero()
                                            local speechResult22 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                            if not speechResult22 then
                                                goto LAB_00e3d473
                                            end
                                            timerId = me:IsPerformingScriptTask()
                                            scratchValue7 = timerId
                                            while scratchValue7 do
                                                quest:NewScriptFrame(me)
                                                scratchValue6 = quest:IsActiveThreadTerminating()
                                                if scratchValue6 then goto LAB_00e3d473 end
                                                timerId = me:IsPerformingScriptTask()
                                                scratchValue7 = timerId
                                            end
                                            scratchValue6 = quest:IsActiveThreadTerminating()
                                            if scratchValue6 then goto LAB_00e3d063 end
                                        end
                                    else
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d473 end
                                        local scratchValue27 = resources:ScriptThing(madameResource)
                                        getTimer = scratchValue27
                                        health = quest:GetHealth(getTimer)
                                        scratchValue7 = 0.0 < health
                                        if scratchValue7 then
                                            scratchValue11 = 0
                                            scratchValue10 = 1
                                            scratchValue9 = 0
                                            getTimer = 0
                                            line = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                            timerId = quest:GetHero()
                                            local speechResult2 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                            if not speechResult2 then
                                                goto LAB_00e3d063
                                            end
                                            timerId = me:IsPerformingScriptTask()
                                            scratchValue7 = timerId
                                            while scratchValue7 do
                                                quest:NewScriptFrame(me)
                                                scratchValue6 = quest:IsActiveThreadTerminating()
                                                if scratchValue6 then goto LAB_00e3d063 end
                                                timerId = me:IsPerformingScriptTask()
                                                scratchValue7 = timerId
                                            end
                                            scratchValue6 = quest:IsActiveThreadTerminating()
                                            if not scratchValue6 then
                                                quest:SetStateBool("HeroTricking", true)
                                                goto LAB_00e3dfb3
                                            end
                                            goto LAB_00e3d473
                                        end
                                    end
                                else
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d473 end
                                    local scratchValue31 = resources:ScriptThing(madameResource)
                                    getTimer = scratchValue31
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                        timerId = quest:GetHero()
                                        local speechResult3 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                        if not speechResult3 then
                                            goto LAB_00e3d063
                                        end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                        while scratchValue7 do
                                            quest:NewScriptFrame(me)
                                            scratchValue6 = quest:IsActiveThreadTerminating()
                                            if scratchValue6 then goto LAB_00e3d063 end
                                            timerId = me:IsPerformingScriptTask()
                                            scratchValue7 = timerId
                                        end
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d473 end
                                    end
                                end
                                quest:SetStateBool("HeroTricking", true)
                            else
                                if scratchValue6 then goto LAB_00e3d473 end
                                scriptThing = resources:ScriptThing(madameResource)
                                getTimer = scriptThing
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                                    timerId = quest:GetHero()
                                    local speechResult4 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                    if not speechResult4 then
                                        goto LAB_00e3d063
                                    end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d063 end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                    end
                                    goto LAB_00e3dfa4
                                end
                            end
                        else
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d063 end
                            local scratchValue28 = resources:ScriptThing(madameResource)
                            getTimer = scratchValue28
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                                timerId = quest:GetHero()
                                local speechResult5 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                if not speechResult5 then
                                    goto LAB_00e3d473
                                end
                                timerId = me:IsPerformingScriptTask()
                                scratchValue7 = timerId
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d473 end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                end
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                            end
                        end
                    else
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d473 end
                        if not quest:GetStateBool("BecomeNunnery") then
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            if not mentionedNunnery then
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                                mentionedNunnery = true
                                local scratchValue26 = resources:ScriptThing(madameResource)
                                getTimer = scratchValue26
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY"
                                    timerId = quest:GetHero()
                                    local speechResult6 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                    if not speechResult6 then
                                        goto LAB_00e3d473
                                    end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d473 end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                    end
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d063 end
                                end
                            else
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d473 end
                                local scratchValue25 = resources:ScriptThing(madameResource)
                                getTimer = scratchValue25
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 2
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_REPEAT"
                                    timerId = quest:GetHero()
                                    local speechResult7 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                    if not speechResult7 then
                                        goto LAB_00e3d063
                                    end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d063 end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                    end
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d473 end
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_BECOME_NUNNERY_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                            while timerId < 0 do
                                quest:NewScriptFrame(me)
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if timerId == 1 then
                                if scratchValue6 then goto LAB_00e3d063 end
                                local scratchValue2 = resources:ScriptThing(madameResource)
                                getTimer = scratchValue2
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE"
                                    timerId = quest:GetHero()
                                    local speechResult8 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                    if not speechResult8 then
                                        goto LAB_00e3d473
                                    end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d473 end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                    end
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d063 end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                while timerId < 0 do
                                    quest:NewScriptFrame(me)
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d473 end
                                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if timerId == 1 then
                                    if scratchValue6 then goto LAB_00e3d473 end
                                    local scratchValue33 = resources:ScriptThing(madameResource)
                                    getTimer = scratchValue33
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_THANKS"
                                        timerId = quest:GetHero()
                                        local speechResult9 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                        if not speechResult9 then
                                            goto LAB_00e3d063
                                        end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                        while scratchValue7 do
                                            quest:NewScriptFrame(me)
                                            scratchValue6 = quest:IsActiveThreadTerminating()
                                            if scratchValue6 then goto LAB_00e3d063 end
                                            timerId = me:IsPerformingScriptTask()
                                            scratchValue7 = timerId
                                        end
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d473 end
                                    end
                                    quest:TakeObjectFromHero("OBJECT_DEEDS_BORDELLO")
                                    scratchValue14 = quest:GetThingWithScriptName("BordelloHouse")
                                    quest:SetHouseOwnedByPlayer(scratchValue14, movie4 ~= 0, false)
                                    quest:SetStateBool("BecomeNunnery", true)
                                    quest:ClearThingHasInformation(me)
                                    quest:GiveHeroRenownPoints(100)
                                    quest:GiveHeroMorality(0.20000000298023224)
                                else
                                    if scratchValue6 then goto LAB_00e3d063 end
                                    local scratchValue24 = resources:ScriptThing(madameResource)
                                    getTimer = scratchValue24
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_RECONSIDER"
                                        timerId = quest:GetHero()
                                        local speechResult10 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                        if not speechResult10 then
                                            goto LAB_00e3d473
                                        end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                        while scratchValue7 do
                                            quest:NewScriptFrame(me)
                                            scratchValue6 = quest:IsActiveThreadTerminating()
                                            if scratchValue6 then goto LAB_00e3d473 end
                                            timerId = me:IsPerformingScriptTask()
                                            scratchValue7 = timerId
                                        end
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d063 end
                                    end
                                end
                            else
                                if scratchValue6 then goto LAB_00e3d473 end
                                movie4 = resources:ScriptThing(madameResource)
                                getTimer = movie4
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_DECLINED"
                                    timerId = quest:GetHero()
                                    local speechResult11 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                    if not speechResult11 then
                                        goto LAB_00e3d063
                                    end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        scratchValue6 = quest:IsActiveThreadTerminating()
                                        if scratchValue6 then goto LAB_00e3d063 end
                                        timerId = me:IsPerformingScriptTask()
                                        scratchValue7 = timerId
                                    end
                                    goto LAB_00e3dfa4
                                end
                            end
                        else
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d063 end
                            local scratchValue30 = resources:ScriptThing(madameResource)
                            getTimer = scratchValue30
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                                timerId = quest:GetHero()
                                local speechResult13 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                                if not speechResult13 then
                                    goto LAB_00e3d473
                                end
                                timerId = me:IsPerformingScriptTask()
                                scratchValue7 = timerId
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    scratchValue6 = quest:IsActiveThreadTerminating()
                                    if scratchValue6 then goto LAB_00e3d473 end
                                    timerId = me:IsPerformingScriptTask()
                                    scratchValue7 = timerId
                                end
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue6 then goto LAB_00e3d063 end
                            end
                        end
                    end
                    goto FLOW_past_lab_00e3dfa4
                    ::LAB_00e3dfa4::
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then goto LAB_00e3d473 end
                    ::FLOW_past_lab_00e3dfa4::
                else
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then goto LAB_00e3d063 end
                    local scratchValue29 = resources:ScriptThing(madameResource)
                    getTimer = scratchValue29
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                        timerId = quest:GetHero()
                        local speechResult14 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        if not speechResult14 then
                            goto LAB_00e3d473
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then goto LAB_00e3d473 end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then goto LAB_00e3d063 end
                    end
                end
                ::LAB_00e3dfb3::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(startMovie)
            end
            scratchValue7 = me:MsgIsHitByHero()
            if not (scratchValue7) then
                scratchValue7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if scratchValue7 then
                    scratchValue7 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                    if not scratchValue7 then goto LAB_00e3e06a end
                end
                predicateResult = false
                goto FLOW_past_lab_00e3e06a
            end
            ::LAB_00e3e06a::
            predicateResult = true
            ::FLOW_past_lab_00e3e06a::
            if not predicateResult then quest:NewScriptFrame(me); scratchValue6 = quest:IsActiveThreadTerminating(); timerId = timerId2; goto continue_4 end
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then break end
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            __push14 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, __push14, false)
            quest:Pause(1.0)
            __push15 = quest:GetHero()
            newResource = me
            quest:EntitySetFacingAngleTowardsThing(me, __push15, true)
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint("CAM_B_MADAM", me, -1.0, 0, -1)
            movie6 = resources:ScriptThing(madameResource)
            scratchValue14 = movie6
            health = quest:GetHealth(scratchValue14)
            scratchValue7 = 0.0 < health
            if scratchValue7 then
                scratchValue11 = 0
                scratchValue10 = 1
                scratchValue9 = 0
                getTimer = 2
                line2 = this_00
                timerId = quest:GetHero()
                local speechResult15 = me:Speak(timerId, line2, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                if not speechResult15 then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    timerId = me:IsPerformingScriptTask()
                    scratchValue7 = timerId
                    while scratchValue7 do
                        quest:NewScriptFrame(me)
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00e3e2e3
                        end
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                    end
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00e3e2e3
                    end
                    goto FLOW_past_lab_00e3e2e3
                end
                ::LAB_00e3e2e3::
                movie = movie3
                goto LAB_00e3e2ea
                ::FLOW_past_lab_00e3e2e3::
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            quest:NewScriptFrame(me)
            scratchValue6 = quest:IsActiveThreadTerminating()
            timerId = timerId2
            ::continue_4::
        end
    end
    ::FLOW_past_lab_00e3be37::
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(madameResource)
    do return end
    ::LAB_00e3d473::
    quest:PauseAllNonScriptedEntities(false)
    movie = startMovie
    goto LAB_00e3e2ea
    ::LAB_00e3d063::
    quest:PauseAllNonScriptedEntities(false)
    movie = startMovie
    ::LAB_00e3e2ea::
    resources:DestroyMovie(movie)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(madameResource)
end

-- Madame.Init (retail 0x00e3ab60)
function Init(quest, me)
    doneIntro_ = false
    mentionedNunnery = false
end

-- Madame.OnPersist (retail 0x00e3ba50)
function OnPersist(quest, me, context)
    local doneIntro = doneIntro_ or false
    doneIntro_ = quest:PersistTransferBool(context, "DoneIntro", doneIntro)
end

-- Madame.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

