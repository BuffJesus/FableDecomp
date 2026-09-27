-- Readable native conversion: MansLover. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local leavingRegion

-- MansLover.Main (retail 0x00ec9d30)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local getStateBool, scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5
    local scratchValue6, scratchValue7, scratchValue8, sequence1, getHero, line, speechResult
    local speechResult2, speechResult3, speechResult4, speechResult5, mansLoverLeavesHere
    local speechResult6, speechResult7, speechResult8, speechResult9, speechResult10, speechResult11
    local speechResult12, speechResult13, scratchValue9
    quest:NewScriptFrame(me)
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    scratchValue = resources:TryAcquire(resource, me, 4)
    while not scratchValue do
        quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
        if not scratchValue then
            scratchValue = resources:TryAcquire(resource, me, 4)
        else
            resources:ReleaseResource(resource)
            do return end
            scratchValue = resources:TryAcquire(resource, me, 4)
        end
    end
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then
        resources:ReleaseResource(resource)
        return
    end
    if quest:GetStateInt("MansLoverState") == 0 then
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            resources:ReleaseResource(resource)
            return
        end
        quest:SetThingHasInformation(me, true, true, false)
    end
    scratchValue3 = 0
    local scratchValue10 = quest:ReadGlobalGameDataString(1852)
    local scratchValue11 = scratchValue10
    scratchValue = quest:IsActiveThreadTerminating()
    while not scratchValue do
        local scratchValue12 = ""
        scratchValue2 = me:MsgIsPresentedWithItem()
        if scratchValue2 then scratchValue12 = _G.g_PresentedItemName end
        sequence1 = false
        sequence1 = scratchValue2
        if sequence1 then
            sequence1 = scratchValue11 == scratchValue12
            if not sequence1 then
                sequence1 = scratchValue11 ~= nil and scratchValue12 ~= nil and scratchValue11[1] == scratchValue12[1]
                if sequence1 then
                    -- TODO(native): iVar5 = CBasicString<char>::Compare((void *)*xStack_128,(void *)*xStack_12c);
                    sequence1 = scratchValue7 == 0
                end
            end
        end
        if sequence1 then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then break end
            scratchValue3 = 1
        end
        scratchValue2 = me:IsTalkedToByHero()
        scratchValue = not ((not scratchValue2) and (scratchValue3 == 0))
        if scratchValue then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then break end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if scratchValue3 == 0 then
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    break
                end
                quest:FixMovieSequenceCamera(true)
                scratchValue = false
                getHero = hero
                quest:EntitySetFacingAngleTowardsThing(me, getHero, scratchValue)
                quest:Pause(1.0)
                scratchValue = true
                getHero = hero
                quest:EntitySetFacingAngleTowardsThing(me, getHero, scratchValue)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
            else
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    break
                end
                quest:FixMovieSequenceCamera(true)
                scratchValue = false
                getHero = hero
                quest:EntitySetFacingAngleTowardsThing(me, getHero, scratchValue)
                quest:Pause(1.0)
                scratchValue = true
                getHero = hero
                quest:EntitySetFacingAngleTowardsThing(me, getHero, scratchValue)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
            end
            local scratchValue13 = quest:ReadGlobalGameDataString(1852)
            getHero = hero
            scratchValue = quest:IsObjectInThingsPossession(scratchValue13, getHero)
            local predicateResult = quest:IsActiveThreadTerminating()
            if scratchValue then
                if predicateResult then goto LAB_00ecaf49 end
                if scratchValue3 == 0 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if not scratchValue then
                        local scratchValue17 = resources:ScriptThing(resource)
                        getHero = scratchValue17
                        local fret_0 = quest:GetHealth(getHero)
                        scratchValue4 = 0.0
                        if scratchValue4 < fret_0 then
                            scratchValue6 = 0
                            scratchValue5 = 1
                            scratchValue8 = 0
                            scratchValue7 = 0
                            line = "TEXT_QST_B10_MANS_LOVER_INTRO"
                            getHero = hero
                            speechResult = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                            scratchValue7 = me:IsPerformingScriptTask()
                            scratchValue2 = scratchValue7
                            while scratchValue2 do
                                quest:NewScriptFrame(me)
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                            end
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00eca325 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue7 < 0 do
                            quest:NewScriptFrame(me)
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00ecaf49 end
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue7 ~= 1 then
                            if scratchValue then goto LAB_00ecaf49 end
                            quest:CameraUseCameraPoint(me, getHero, -1.0, 0, -1)
                            local scratchValue28 = resources:ScriptThing(resource)
                            getHero = scratchValue28
                            local fret_00 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if fret_00 <= scratchValue4 then goto LAB_00ecae14 end
                            scratchValue6 = 0
                            scratchValue5 = 1
                            scratchValue8 = 0
                            scratchValue7 = 0
                            line = "TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10"
                            getHero = hero
                            speechResult6 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                            scratchValue7 = me:IsPerformingScriptTask()
                            scratchValue2 = scratchValue7
                            while scratchValue2 do
                                quest:NewScriptFrame(me)
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                            end
                            goto LAB_00ecae05
                        end
                        if not scratchValue then goto LAB_00eca2ed end
                        goto LAB_00ecaf49
                    end
                else
                    scratchValue = quest:IsActiveThreadTerminating()
                    if not scratchValue then
                        goto LAB_00eca2ed
                    end
                end
                goto FLOW_past_lab_00eca2ed
                ::LAB_00eca2ed::
                scratchValue = quest:IsActiveThreadTerminating()
                if not scratchValue then
                    if scratchValue3 == 0 then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        quest:CameraUseCameraPoint(me, getHero, -1.0, 0, -1)
                    else
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        scratchValue3 = 0
                    end
                    local scratchValue15 = quest:ReadGlobalGameDataString(1852)
                    quest:TakeObjectFromHero(scratchValue15)
                    local scratchValue26 = resources:ScriptThing(resource)
                    getHero = scratchValue26
                    local fret_01 = quest:GetHealth(getHero)
                    scratchValue4 = 0.0
                    if scratchValue4 < fret_01 then
                        scratchValue6 = 0
                        scratchValue5 = 1
                        scratchValue8 = 0
                        scratchValue7 = 0
                        line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_10"
                        getHero = hero
                        speechResult7 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                        while scratchValue2 do
                            quest:NewScriptFrame(me)
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00ecaf49 end
                            scratchValue7 = me:IsPerformingScriptTask()
                            scratchValue2 = scratchValue7
                        end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue7 < 0 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if not scratchValue then
                        quest:CameraUseCameraPoint(me, getHero, -1.0, 0, -1)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue7 == 0 then
                            if scratchValue then goto LAB_00ecaf49 end
                            local scratchValue20 = resources:ScriptThing(resource)
                            getHero = scratchValue20
                            local fret_02 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if scratchValue4 < fret_02 then
                                scratchValue6 = 0
                                scratchValue5 = 1
                                scratchValue8 = 0
                                scratchValue7 = 0
                                line = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10"
                                getHero = hero
                                speechResult8 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00eca325 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    scratchValue2 = scratchValue7
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                            end
                            local scratchValue24 = resources:ScriptThing(resource)
                            getHero = scratchValue24
                            local fret_03 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if scratchValue4 < fret_03 then
                                scratchValue6 = 0
                                scratchValue5 = 1
                                scratchValue8 = 0
                                scratchValue7 = 0
                                line = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20"
                                getHero = hero
                                speechResult9 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00eca325 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    scratchValue2 = scratchValue7
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                            end
                            local scratchValue14 = quest:ReadGlobalGameDataString(1856)
                            quest:GiveHeroObject(scratchValue14, -1, false)
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1880))
                            quest:SetStateInt("MansLoverState", 2)
                        else
                            if scratchValue then goto LAB_00eca325 end
                            local scratchValue16 = resources:ScriptThing(resource)
                            getHero = scratchValue16
                            local fret_04 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if scratchValue4 < fret_04 then
                                scratchValue6 = 0
                                scratchValue5 = 1
                                scratchValue8 = 0
                                scratchValue7 = 0
                                line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_20"
                                getHero = hero
                                speechResult10 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    scratchValue2 = scratchValue7
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            local scratchValue22 = resources:ScriptThing(resource)
                            getHero = scratchValue22
                            local fret_05 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if scratchValue4 < fret_05 then
                                scratchValue6 = 0
                                scratchValue5 = 1
                                scratchValue8 = 0
                                scratchValue7 = 0
                                line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_30"
                                getHero = hero
                                speechResult11 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    scratchValue2 = scratchValue7
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            quest:GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", -1, false)
                            local scratchValue18 = resources:ScriptThing(resource)
                            getHero = scratchValue18
                            local fret_06 = quest:GetHealth(getHero)
                            scratchValue4 = 0.0
                            if scratchValue4 < fret_06 then
                                scratchValue6 = 0
                                scratchValue5 = 1
                                scratchValue8 = 0
                                scratchValue7 = 0
                                line = "TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10"
                                getHero = hero
                                speechResult12 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                                scratchValue7 = me:IsPerformingScriptTask()
                                scratchValue2 = scratchValue7
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    scratchValue2 = scratchValue7
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1884))
                            quest:SetStateInt("MansLoverState", 1)
                        end
                        quest:ClearThingHasInformation(me)
                        leavingRegion = true
                        goto LAB_00ecae14
                    end
                end
                ::FLOW_past_lab_00eca2ed::
                goto LAB_00eca325
            end
            goto FLOW_past_lab_00eca325
            ::LAB_00eca325::
            -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
            resources:DestroyMovie(movie)
            break
            ::FLOW_past_lab_00eca325::
            if predicateResult then goto LAB_00ecaf49 end
            me:ClearCommands()
            if not leavingRegion then
                goto LAB_00ecab59
            else
                scratchValue9 = math.random(0, 32767)
                scratchValue9 = scratchValue9 & 0x80000001
                scratchValue = scratchValue9 == 0
                if scratchValue9 < 0 then
                    scratchValue = (scratchValue9 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not scratchValue then goto LAB_00ecab59 end
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then goto LAB_00eca325 end
                local scratchValue27 = resources:ScriptThing(resource)
                getHero = scratchValue27
                local fret_07 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_07 then
                    scratchValue6 = 0
                    scratchValue5 = 1
                    scratchValue8 = 0
                    scratchValue7 = 0
                    line = "TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10"
                    getHero = hero
                    speechResult13 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                    scratchValue7 = me:IsPerformingScriptTask()
                    scratchValue2 = scratchValue7
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eca325 end
                end
            end
            goto FLOW_past_lab_00ecab59
            ::LAB_00ecab59::
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then
                goto LAB_00ecaf49
            end
            goto FLOW_hoist_lab_00ecaf49_1
            ::FLOW_past_lab_00ecab59::
            goto FLOW_past_lab_00ecaf49
            ::LAB_00ecaf49::
            -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
            resources:DestroyMovie(movie)
            break
            ::FLOW_hoist_lab_00ecaf49_1::
            scratchValue7 = quest:GetStateInt("MansLoverState")
            if scratchValue7 == 0 then
                local scratchValue19 = resources:ScriptThing(resource)
                getHero = scratchValue19
                local fret_11 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_11 then
                    scratchValue6 = 0
                    scratchValue5 = 1
                    scratchValue8 = 0
                    scratchValue7 = 0
                    line = "TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER"
                    getHero = hero
                    speechResult2 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                    scratchValue7 = me:IsPerformingScriptTask()
                    scratchValue2 = scratchValue7
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                    end
                    goto LAB_00ecae05
                end
            elseif scratchValue7 then
                local scratchValue21 = resources:ScriptThing(resource)
                getHero = scratchValue21
                local fret_10 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_10 then
                    scratchValue6 = 0
                    scratchValue5 = 1
                    scratchValue8 = 0
                    scratchValue7 = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                    getHero = hero
                    speechResult3 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                    scratchValue7 = me:IsPerformingScriptTask()
                    scratchValue2 = scratchValue7
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                    end
                    goto LAB_00ecae05
                end
            elseif scratchValue7 == 2 then
                local scratchValue23 = resources:ScriptThing(resource)
                getHero = scratchValue23
                local fret_09 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_09 then
                    scratchValue6 = 0
                    scratchValue5 = 1
                    scratchValue8 = 0
                    scratchValue7 = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN"
                    getHero = hero
                    speechResult4 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                    scratchValue7 = me:IsPerformingScriptTask()
                    scratchValue2 = scratchValue7
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                    end
                    goto LAB_00ecae05
                end
            else
                local scratchValue25 = resources:ScriptThing(resource)
                getHero = scratchValue25
                local fret_08 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_08 then
                    scratchValue6 = 0
                    scratchValue5 = 1
                    scratchValue8 = 0
                    scratchValue7 = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                    getHero = hero
                    speechResult5 = me:Speak(getHero, line, scratchValue7, scratchValue8 ~= 0, scratchValue5 ~= 0, scratchValue6 ~= 0)
                    scratchValue7 = me:IsPerformingScriptTask()
                    scratchValue2 = scratchValue7
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue7 = me:IsPerformingScriptTask()
                        scratchValue2 = scratchValue7
                    end
                    goto LAB_00ecae05
                end
            end
            goto FLOW_past_lab_00ecae05
            ::LAB_00ecae05::
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then goto LAB_00ecaf49 end
            ::FLOW_past_lab_00ecae05::
            ::FLOW_past_lab_00ecaf49::
            ::LAB_00ecae14::
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
            resources:DestroyMovie(movie)
        end
        getStateBool = leavingRegion
        if getStateBool then
            scratchValue7 = me:IsPerformingScriptTask()
            getStateBool = not scratchValue7
        end
        if not getStateBool then quest:NewScriptFrame(me); scratchValue = quest:IsActiveThreadTerminating(); goto continue_2 end
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then break end
        mansLoverLeavesHere = quest:GetThingWithScriptName("MansLoverLeavesHere")
        scratchValue = quest:IsDistanceBetweenThingsOver(me, mansLoverLeavesHere, 2.0)
        if scratchValue then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then
                goto LAB_00ecaf62
            end
            goto FLOW_hoist_lab_00ecaf62_1
        else
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then goto LAB_00ecaf62 end
            quest:FadeOutAndKillEntity(me, true, 1.5, true)
        end
        goto FLOW_past_lab_00ecaf62
        ::LAB_00ecaf62::
        break
        ::FLOW_hoist_lab_00ecaf62_1::
        me:MoveToThing(mansLoverLeavesHere, 1.0, ENTITY_MOVE_WALK)
        ::FLOW_past_lab_00ecaf62::
        quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
        ::continue_2::
    end
    resources:ReleaseResource(resource)
end

-- MansLover.Init (retail 0x00ec9cb0)
function Init(quest, me)
    quest:SetThingPersistent(me, true)
    leavingRegion = false
end

-- MansLover.OnPersist (retail 0x00ecd930)
function OnPersist(quest, me, context)
    quest:SetStateBool("LeavingRegion", quest:PersistTransferBool(context, "LeavingRegion", quest:GetStateBool("LeavingRegion")))
end

-- MansLover.OnPredicateFail (retail 0x00ec9cd0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateInt("MansLoverState", 3)
    end
end

