-- Readable native conversion: Madame. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local doneIntro, mentionedNunnery

-- Madame.Main (retail 0x00e3bb70)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, __push10, __push14, __push15, __push9, scratchValue3, scratchValue6
    local scratchValue7, ctr_140, health, scratchValue8, scratchValue9, scratchValue10
    local scratchValue11, timerId, getTimer, scratchValue12, timerId2, sequence1, sequence, p0
    local resource, scratchValue13, thing, line, line2, speechResult, speechResult2, speechResult3
    local speechResult4, speechResult5, speechResult6, speechResult7, speechResult8, speechResult9
    local speechResult10, speechResult11, speechResult12, speechResult13, speechResult14
    local speechResult15, speechResult16, speechResult17, speechResult18, speechResult19
    local speechResult20, speechResult21, speechResult22, this_00, scratchValue15, scratchValue16
    local scratchValue17, resource2, scratchValue18, scratchValue19, resource3, resource4
    local startMovie, movie, newResource, scratchValue22, scratchValue23, scratchValue24, movie3
    local movie4, movie5, movie6, scratchValue26, scriptThing, scriptThing2, scriptThing3
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
    scratchValue16 = 0
    ctr_140 = nil
    -- TODO(native): xStack_174 = *(undefined ***)(this + 0x10);
    if newResource ~= nil then
        -- TODO(native): *xStack_174 = *xStack_174 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], scratchValue16 ~= 0)
    timerId2 = 0
    scratchValue23 = p0
    quest:EntitySetAsKillable(me, false, false)
    -- TODO(native): xStack_124._0_3_ = 0;
    -- TODO(native): xStack_124_b3 = '\0';
    startMovie = p0
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    newResource = resources:NewResource()
    resources:PrepareResource(newResource)
    local scratchValue21 = p0
    scratchValue7 = me:AcquireControl(4)
    while not scratchValue7 do
        quest:NewScriptFrame(me)
        scratchValue6 = quest:IsActiveThreadTerminating()
        if scratchValue6 then resources:ReleaseResource(newResource); return end
        scratchValue19 = p0
        scratchValue7 = me:AcquireControl(4)
    end
    scratchValue6 = quest:IsActiveThreadTerminating()
    if scratchValue6 then resources:ReleaseResource(newResource); return end
    resources:AssignResource(resources:MemberResource("seh_Madam"), newResource)
    timerId = quest:RegisterTimer()
    timerId2 = timerId
    quest:SetTimer(timerId, 2)
    if not doneIntro then
        scratchValue6 = quest:IsActiveThreadTerminating()
        if not scratchValue6 then
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
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
                resource = movie
            end
            goto FLOW_past_lab_00e3bde9
            ::LAB_00e3bde9::
            resources:SetString(resources:MemberStringMap("csargs"), "$LADYINTRO", line)
            -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
            doneIntro = true
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00e3be37
            ::FLOW_hoist_lab_00e3bde9_1::
            quest:PauseAllNonScriptedEntities(false)
            resource = movie
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
        if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
        getTimer = quest:GetTimer(timerId2)
        if getTimer == 0 then
            scratchValue8 = 8.0
            thing = hero
            scratchValue6 = quest:IsDistanceBetweenThingsUnder(me, thing, scratchValue8)
            if scratchValue6 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                quest:SetTimer(timerId, 4)
                local __push1 = hero
                scratchValue19 = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
            end
        end
        scratchValue6 = me:MsgIsPresentedWithItem()
        if scratchValue6 then scratchValue22 = _G.g_PresentedItemName end
        if scratchValue6 then
            if scratchValue22 == nil then
                scratchValue6 = false
                if scratchValue6 then
                    goto LAB_00e3bf28
                end
            else
                timerId = scratchValue17 == "OBJECT_BEER_TANKARD" and 0 or 1
                -- TODO(native): xStack_124_b3 = !(iVar5 != 0);
                if scratchValue18 ~= 0 then goto LAB_00e3bf28 end
            end
            goto FLOW_past_lab_00e3bf28
            ::LAB_00e3bf28::
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
            movie5 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            scratchValue3 = resources:ScriptThing(resource2)
            thing = scratchValue3
            health = quest:GetHealth(thing)
            scratchValue = scratchValue21
            if health <= 0.0 then
                -- TODO(native): xStack_164 = CVar2 & 0xffffff;
            end
            scratchValue11 = 0
            scratchValue10 = 1
            scratchValue9 = 0
            getTimer = 0
            line = "TEXT_QST_B13_MADAME_GIVEN_BEER"
            timerId = hero
            speechResult = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
            timerId = me:IsPerformingScriptTask()
            scratchValue7 = timerId
            while scratchValue7 do
                quest:NewScriptFrame(me)
                scratchValue6 = quest:IsActiveThreadTerminating()
                if not scratchValue6 then timerId = me:IsPerformingScriptTask(); scratchValue7 = timerId; goto continue_1 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                quest:DeregisterTimer(movie)
                resources:DestroyMovie(movie)
                do return end
                timerId = me:IsPerformingScriptTask()
                scratchValue7 = timerId
                ::continue_1::
            end
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00e3e2ea
            end
            quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            ::FLOW_past_lab_00e3bf28::
        end
        scratchValue18 = me:IsTalkedToByHero()
        if scratchValue18 then
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
            sequence1 = false
            -- TODO(native): if (*(this + 0x14))[0x49] == nil then
            sequence1 = false
            if sequence1 then
                scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
                sequence1 = scratchValue6
            end
            if sequence1 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local __push2 = hero
                quest:EntitySetFacingAngleTowardsThing(me, __push2, false)
                quest:Pause(1.0)
                local __push3 = hero
                newResource = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push3, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 1, 1)
                local scratchValue4 = resources:ScriptThing(newResource)
                thing = scratchValue4
                health = quest:GetHealth(thing)
                scratchValue7 = 0.0 < health
                if scratchValue7 then
                    scratchValue11 = 0
                    scratchValue10 = 1
                    scratchValue9 = 0
                    getTimer = 0
                    line = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                    timerId = hero
                    speechResult12 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                    scriptThing = resources:ScriptThing(resource4)
                    getTimer = scriptThing
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_JOINED"
                        timerId = hero
                        speechResult16 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                    scriptThing2 = resources:ScriptThing(resource4)
                    getTimer = scriptThing2
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                        timerId = hero
                        speechResult17 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                local __push4 = hero
                scratchValue18 = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", __push4)
                if scratchValue18 then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                    movie4 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                    quest:SetStateBool("PlayerOwned", true)
                    thing = quest:GetThingWithScriptName("BordelloHouse")
                    quest:SetHouseOwnedByPlayer(thing, true, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e3cab0
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(newResource)
                    return
                end
                sequence = false
                -- TODO(native): if (*(this + 0x14))[0x49] == nil then
                sequence = false
                if not sequence then
                    scratchValue6 = helpers.IsHeroWearingBeard(quest, me)
                    sequence = not scratchValue6
                end
                if sequence then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                    local movie2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    local __push5 = hero
                    quest:EntitySetFacingAngleTowardsThing(me, __push5, false)
                    quest:Pause(1.0)
                    local __push6 = hero
                    newResource = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push6, true)
                    quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 1, scratchValue8)
                    if 5 < me then
                        scratchValue12 = 1
                    end
                    scratchValue13 = tostring(scratchValue12)
                    local scratchValue20 = "TEXT_QST_B13_MADAME_CHIT_CHAT_0" .. scratchValue13
                    local scratchValue5 = resources:ScriptThing(newResource)
                    thing = scratchValue5
                    health = quest:GetHealth(thing)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line2 = scratchValue20
                        timerId = hero
                        speechResult18 = me:Speak(timerId, line2, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resource = movie2
                                goto LAB_00e3e2ea
                            end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resource = movie2
                            goto LAB_00e3e2ea
                        end
                    end
                    ctr_140 = ctr_140 + 1
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resource = movie2
                else
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
                    scratchValue24 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    local __push7 = hero
                    quest:EntitySetFacingAngleTowardsThing(me, __push7, false)
                    quest:Pause(1.0)
                    local __push8 = hero
                    newResource = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push8, true)
                    quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 1, scratchValue12)
                    scriptThing3 = resources:ScriptThing(newResource)
                    thing = scriptThing3
                    health = quest:GetHealth(thing)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                        timerId = hero
                        speechResult19 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
                        timerId = me:IsPerformingScriptTask()
                        scratchValue7 = timerId
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resource = scratchValue24
                                goto LAB_00e3e2ea
                            end
                            timerId = me:IsPerformingScriptTask()
                            scratchValue7 = timerId
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resource = scratchValue24
                            goto LAB_00e3e2ea
                        end
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resource = scratchValue24
                end
            end
            resources:ReleaseResource(resource)
        end
        ::LAB_00e3cab0::
        scratchValue15 = scratchValue16 | 1
        scratchValue6 = me:MsgIsHitByHero()
        if scratchValue6 then
            goto LAB_00e3cb3c
        else
            scratchValue15 = scratchValue16 | 3
            scratchValue6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if scratchValue6 then
                scratchValue15 = scratchValue16 | 7
                scratchValue6 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                if not scratchValue6 then goto LAB_00e3cb3c end
            end
            -- TODO(native): xStack_124_b3 = '\0';
        end
        goto FLOW_past_lab_00e3cb3c
        ::LAB_00e3cb3c::
        -- TODO(native): xStack_124_b3 = '\x01';
        ::FLOW_past_lab_00e3cb3c::
        if scratchValue15 & 4 ~= 0 then
            scratchValue15 = scratchValue15 & 0xfffffffb
        end
        if scratchValue15 & 2 ~= 0 then
            scratchValue15 = scratchValue15 & 0xfffffffd
        end
        if scratchValue15 & 1 ~= 0 then
            scratchValue15 = scratchValue15 & 0xfffffffe
        end
        if scratchValue18 == 0 then scratchValue16 = scratchValue15; scratchValue7 = quest:GetStateBool("PlayerOwned"); goto continue_2 end
        scratchValue6 = quest:IsActiveThreadTerminating()
        if scratchValue6 then quest:DeregisterTimer(timerId2); resources:ReleaseResource(newResource); return end
        movie6 = resources:StartMovie("")
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        __push9 = hero
        quest:EntitySetFacingAngleTowardsThing(me, __push9, false)
        quest:Pause(1.0)
        __push10 = hero
        newResource = p0
        quest:EntitySetFacingAngleTowardsThing(me, __push10, true)
        quest:NewScriptFrame(me)
        getTimer = -1
        quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, getTimer, 1)
        scratchValue24 = resources:ScriptThing(newResource)
        thing = scratchValue24
        health = quest:GetHealth(thing)
        scratchValue7 = 0.0 < health
        if scratchValue7 then
            local p5 = 0
            scratchValue11 = 1
            scratchValue10 = 0
            scratchValue9 = 2
            scratchValue13 = helpers.GetHeroStatusTextTag(quest, me)
            scratchValue13 = a .. scratchValue13
            line2 = scratchValue13
            timerId = hero
            speechResult20 = me:Speak(timerId, line2, scratchValue9, scratchValue10 ~= 0, scratchValue11 ~= 0, p5 ~= 0)
            timerId = me:IsPerformingScriptTask()
            scratchValue7 = timerId
            while scratchValue7 do
                quest:NewScriptFrame(me)
                scratchValue6 = quest:IsActiveThreadTerminating()
                if not scratchValue6 then timerId = me:IsPerformingScriptTask(); scratchValue7 = timerId; goto continue_3 end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                quest:DeregisterTimer(getTimer)
                resources:ReleaseResource(newResource)
                do return end
                timerId = me:IsPerformingScriptTask()
                scratchValue7 = timerId
                ::continue_3::
            end
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then
                quest:PauseAllNonScriptedEntities(false)
                resource = movie6
                goto LAB_00e3e2ea
            end
        end
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie6)
        scratchValue16 = scratchValue15
        scratchValue7 = quest:GetStateBool("PlayerOwned")
        ::continue_2::
    end
    scratchValue6 = quest:IsActiveThreadTerminating()
    if not scratchValue6 then
        scratchValue6 = quest:IsActiveThreadTerminating()
        while true do
            local resource5 = timerId
            if scratchValue6 then break end
            getTimer = quest:GetTimer(4)
            if getTimer == 0 then
                scratchValue8 = 8.0
                thing = hero
                scratchValue6 = quest:IsDistanceBetweenThingsUnder(me, thing, scratchValue8)
                if scratchValue6 then
                    scratchValue6 = quest:IsActiveThreadTerminating()
                    if scratchValue6 then break end
                    quest:SetTimer(4, scratchValue8)
                    local __push11 = hero
                    scratchValue19 = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push11, false)
                end
            end
            scratchValue18 = me:IsTalkedToByHero()
            if scratchValue18 then
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue6 then break end
                startMovie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local __push12 = hero
                quest:EntitySetFacingAngleTowardsThing(me, __push12, false)
                quest:Pause(1.0)
                local __push13 = hero
                newResource = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push13, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, thing, -1.0, 1, nil --[[missing]])
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
                            local scratchValue34 = resources:ScriptThing(newResource)
                            getTimer = scratchValue34
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                                timerId = hero
                                speechResult21 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                        scriptThing2 = resources:ScriptThing(resource5)
                                        getTimer = scriptThing2
                                        health = quest:GetHealth(getTimer)
                                        scratchValue7 = 0.0 < health
                                        if scratchValue7 then
                                            scratchValue11 = 0
                                            scratchValue10 = 1
                                            scratchValue9 = 0
                                            getTimer = 0
                                            line = "TEXT_QST_B13_MADAME_HERO_JOINED"
                                            timerId = hero
                                            speechResult22 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                        local scratchValue29 = resources:ScriptThing(resource5)
                                        getTimer = scratchValue29
                                        health = quest:GetHealth(getTimer)
                                        scratchValue7 = 0.0 < health
                                        if scratchValue7 then
                                            scratchValue11 = 0
                                            scratchValue10 = 1
                                            scratchValue9 = 0
                                            getTimer = 0
                                            line = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                            timerId = hero
                                            speechResult2 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                    local scratchValue33 = resources:ScriptThing(resource5)
                                    getTimer = scratchValue33
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                        timerId = hero
                                        speechResult3 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                scriptThing = resources:ScriptThing(startMovie)
                                getTimer = scriptThing
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                                    timerId = hero
                                    speechResult4 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                            local scratchValue30 = resources:ScriptThing(resource5)
                            getTimer = scratchValue30
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                                timerId = hero
                                speechResult5 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                local scratchValue28 = resources:ScriptThing(newResource)
                                getTimer = scratchValue28
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY"
                                    timerId = hero
                                    speechResult6 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                local scratchValue27 = resources:ScriptThing(startMovie)
                                getTimer = scratchValue27
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 2
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_REPEAT"
                                    timerId = hero
                                    speechResult7 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                local scratchValue2 = resources:ScriptThing(resource5)
                                getTimer = scratchValue2
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE"
                                    timerId = hero
                                    speechResult8 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                    local scratchValue35 = resources:ScriptThing(resource3)
                                    getTimer = scratchValue35
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_THANKS"
                                        timerId = hero
                                        speechResult9 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                    thing = quest:GetThingWithScriptName("BordelloHouse")
                                    quest:SetHouseOwnedByPlayer(thing, movie4 ~= 0, false)
                                    quest:SetStateBool("BecomeNunnery", true)
                                    quest:ClearThingHasInformation(me)
                                    quest:GiveHeroRenownPoints(100)
                                    quest:GiveHeroMorality(0.20000000298023224)
                                else
                                    if scratchValue6 then goto LAB_00e3d063 end
                                    local scratchValue25 = resources:ScriptThing(resource3)
                                    getTimer = scratchValue25
                                    health = quest:GetHealth(getTimer)
                                    scratchValue7 = 0.0 < health
                                    if scratchValue7 then
                                        scratchValue11 = 0
                                        scratchValue10 = 1
                                        scratchValue9 = 0
                                        getTimer = 0
                                        line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_RECONSIDER"
                                        timerId = hero
                                        speechResult10 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                                movie4 = resources:ScriptThing(resource3)
                                getTimer = movie4
                                health = quest:GetHealth(getTimer)
                                scratchValue7 = 0.0 < health
                                if scratchValue7 then
                                    scratchValue11 = 0
                                    scratchValue10 = 1
                                    scratchValue9 = 0
                                    getTimer = 0
                                    line = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_DECLINED"
                                    timerId = hero
                                    speechResult11 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                            local scratchValue32 = resources:ScriptThing(resource5)
                            getTimer = scratchValue32
                            health = quest:GetHealth(getTimer)
                            scratchValue7 = 0.0 < health
                            if scratchValue7 then
                                scratchValue11 = 0
                                scratchValue10 = 1
                                scratchValue9 = 0
                                getTimer = 0
                                line = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                                timerId = hero
                                speechResult13 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                    local scratchValue31 = resources:ScriptThing(newResource)
                    getTimer = scratchValue31
                    health = quest:GetHealth(getTimer)
                    scratchValue7 = 0.0 < health
                    if scratchValue7 then
                        scratchValue11 = 0
                        scratchValue10 = 1
                        scratchValue9 = 0
                        getTimer = 0
                        line = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                        timerId = hero
                        speechResult14 = me:Speak(timerId, line, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
            scratchValue15 = scratchValue16 | 8
            scratchValue7 = me:MsgIsHitByHero()
            if scratchValue7 then goto LAB_00e3e06a end
            scratchValue15 = scratchValue16 | 24
            scratchValue7 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if scratchValue7 then
                scratchValue15 = scratchValue16 | 56
                scratchValue7 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                if not scratchValue7 then goto LAB_00e3e06a end
            end
            -- TODO(native): xStack_f8_b3 = '\0';
            goto FLOW_past_lab_00e3e06a
            ::LAB_00e3e06a::
            -- TODO(native): xStack_f8_b3 = '\x01';
            ::FLOW_past_lab_00e3e06a::
            if scratchValue15 & 32 ~= 0 then
                scratchValue15 = scratchValue15 & 0xffffffdf
            end
            if scratchValue15 & 16 ~= 0 then
                scratchValue15 = scratchValue15 & 0xffffffef
            end
            if scratchValue15 & 8 ~= 0 then
                scratchValue15 = scratchValue15 & 0xfffffff7
            end
            if scratchValue26 == 0 then quest:NewScriptFrame(me); scratchValue6 = quest:IsActiveThreadTerminating(); scratchValue16 = scratchValue15; goto continue_4 end
            scratchValue6 = quest:IsActiveThreadTerminating()
            if scratchValue6 then break end
            movie3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            __push14 = hero
            quest:EntitySetFacingAngleTowardsThing(me, __push14, false)
            quest:Pause(1.0)
            __push15 = hero
            newResource = p0
            quest:EntitySetFacingAngleTowardsThing(me, __push15, true)
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 1, nil --[[missing]])
            movie6 = resources:ScriptThing(newResource)
            thing = movie6
            health = quest:GetHealth(thing)
            scratchValue7 = 0.0 < health
            if scratchValue7 then
                scratchValue11 = 0
                scratchValue10 = 1
                scratchValue9 = 0
                getTimer = 2
                line2 = this_00
                timerId = hero
                speechResult15 = me:Speak(timerId, line2, getTimer, scratchValue9 ~= 0, scratchValue10 ~= 0, scratchValue11 ~= 0)
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
                ::LAB_00e3e2e3::
                resource = movie3
                goto LAB_00e3e2ea
                ::FLOW_past_lab_00e3e2e3::
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            quest:NewScriptFrame(me)
            scratchValue6 = quest:IsActiveThreadTerminating()
            scratchValue16 = scratchValue15
            ::continue_4::
        end
    end
    ::FLOW_past_lab_00e3be37::
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(newResource)
    do return end
    ::LAB_00e3d473::
    quest:PauseAllNonScriptedEntities(false)
    resource = startMovie
    goto LAB_00e3e2ea
    ::LAB_00e3d063::
    quest:PauseAllNonScriptedEntities(false)
    resource = startMovie
    ::LAB_00e3e2ea::
    resources:DestroyMovie(resource)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(newResource)
end

-- Madame.Init (retail 0x00e3ab60)
function Init(quest, me)
    doneIntro = false
    mentionedNunnery = false
end

-- Madame.OnPersist (retail 0x00e3ba50)
function OnPersist(quest, me, context)
    quest:SetStateBool("DoneIntro", quest:PersistTransferBool(context, "DoneIntro", quest:GetStateBool("DoneIntro")))
end

-- Madame.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

