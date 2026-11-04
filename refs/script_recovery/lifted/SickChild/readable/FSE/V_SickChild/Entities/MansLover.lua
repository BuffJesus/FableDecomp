-- Readable native conversion: MansLover. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    ObjectToGiveToMansLover = 1852,  -- 'OBJECT_SICK_CHILD_MANS_LETTER'
    ObjectFromMansLover = 1856,  -- 'OBJECT_SICK_CHILD_WOMANS_LETTER'
    TellingTheTruth = 1880,  -- 0.009999999776482582
    LyingToWomanMorality = 1884,  -- -0.009999999776482582
}

-- per-entity fields (native class members; one Lua state per entity instance)
local leavingRegion_

-- MansLover.Main (retail 0x00ec9d30)
function Main(quest, me)
    local resources = quest:RetailResources()
    local getStateBool, scratchValue, scratchValue2, scratchValue3, scratchValue4
    local isPerformingScriptTask, scratchValue5, letterPresentationPending, letterQuestionAnswer
    local mansLoverDialogueState, hero, line, mansLoverLeavesHere, retailRandModulo
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
    letterPresentationPending = false
    local scratchValue6 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectToGiveToMansLover)
    local expectedLetterDefinition = scratchValue6
    scratchValue = quest:IsActiveThreadTerminating()
    while not scratchValue do
        local local_presentedItemName = ""
        scratchValue2 = me:MsgIsPresentedWithItem()
        if scratchValue2 then local_presentedItemName = _G.g_PresentedItemName end
        if scratchValue2 and expectedLetterDefinition == local_presentedItemName then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then break end
            letterPresentationPending = true
        end
        scratchValue2 = me:IsTalkedToByHero()
        scratchValue = not ((not scratchValue2) and (not letterPresentationPending))
        if scratchValue then
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then break end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not letterPresentationPending then
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    break
                end
                quest:FixMovieSequenceCamera(true)
                scratchValue = false
                hero = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero, scratchValue)
                quest:Pause(1.0)
                scratchValue = true
                hero = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero, scratchValue)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_REL_WOMAN", me, -1.0, 0, -1)
            else
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    break
                end
                quest:FixMovieSequenceCamera(true)
                scratchValue = false
                hero = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero, scratchValue)
                quest:Pause(1.0)
                scratchValue = true
                hero = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero, scratchValue)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_REL_WOMANHERO", me, -1.0, 0, -1)
            end
            local scratchValue7 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectToGiveToMansLover)
            hero = quest:GetHero()
            scratchValue = quest:IsObjectInThingsPossession(scratchValue7, hero)
            local predicateResult = quest:IsActiveThreadTerminating()
            if scratchValue then
                if predicateResult then goto LAB_00ecaf49 end
                if not letterPresentationPending then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if not scratchValue then
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            scratchValue4 = 0
                            scratchValue3 = 1
                            scratchValue5 = 0
                            isPerformingScriptTask = 0
                            line = "TEXT_QST_B10_MANS_LOVER_INTRO"
                            hero = quest:GetHero()
                            local speechResult = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                            if not speechResult then
                                goto LAB_00ecaf49
                            end
                            isPerformingScriptTask = me:IsPerformingScriptTask()
                            scratchValue2 = isPerformingScriptTask
                            while scratchValue2 do
                                quest:NewScriptFrame(me)
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                            end
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00eca325 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while letterQuestionAnswer < 0 do
                            quest:NewScriptFrame(me)
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00ecaf49 end
                            letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if letterQuestionAnswer ~= 1 then
                            if scratchValue then goto LAB_00ecaf49 end
                            quest:CameraUseCameraPoint("CAM_REL_WOMANHERO", me, -1.0, 0, -1)
                            hero = nil
                            if not resources:ResourceThingHealthAbove(resource, 0.0) then goto FLOW_past_lab_00ecae05 end
                            scratchValue4 = 0
                            scratchValue3 = 1
                            scratchValue5 = 0
                            isPerformingScriptTask = 0
                            line = "TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10"
                            hero = quest:GetHero()
                            local speechResult6 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                            if not speechResult6 then
                                goto LAB_00eca325
                            end
                            isPerformingScriptTask = me:IsPerformingScriptTask()
                            scratchValue2 = isPerformingScriptTask
                            while scratchValue2 do
                                quest:NewScriptFrame(me)
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
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
                    if not letterPresentationPending then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        quest:CameraUseCameraPoint("CAM_REL_WOMANHERO", me, -1.0, 0, -1)
                    else
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        letterPresentationPending = false
                    end
                    local scratchValue9 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectToGiveToMansLover)
                    quest:TakeObjectFromHero(scratchValue9)
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        scratchValue4 = 0
                        scratchValue3 = 1
                        scratchValue5 = 0
                        isPerformingScriptTask = 0
                        line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_10"
                        hero = quest:GetHero()
                        local speechResult7 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                        if not speechResult7 then
                            goto LAB_00ecaf49
                        end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
                        while scratchValue2 do
                            quest:NewScriptFrame(me)
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00ecaf49 end
                            isPerformingScriptTask = me:IsPerformingScriptTask()
                            scratchValue2 = isPerformingScriptTask
                        end
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while letterQuestionAnswer < 0 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        letterQuestionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if not scratchValue then
                        quest:CameraUseCameraPoint("CAM_REL_WOMAN", me, -1.0, 0, -1)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if letterQuestionAnswer == 0 then
                            if scratchValue then goto LAB_00ecaf49 end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                scratchValue4 = 0
                                scratchValue3 = 1
                                scratchValue5 = 0
                                isPerformingScriptTask = 0
                                line = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10"
                                hero = quest:GetHero()
                                local speechResult8 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                                if not speechResult8 then
                                    goto LAB_00eca325
                                end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00eca325 end
                                    isPerformingScriptTask = me:IsPerformingScriptTask()
                                    scratchValue2 = isPerformingScriptTask
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                            end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                scratchValue4 = 0
                                scratchValue3 = 1
                                scratchValue5 = 0
                                isPerformingScriptTask = 0
                                line = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20"
                                hero = quest:GetHero()
                                local speechResult9 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                                if not speechResult9 then
                                    goto LAB_00eca325
                                end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00eca325 end
                                    isPerformingScriptTask = me:IsPerformingScriptTask()
                                    scratchValue2 = isPerformingScriptTask
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00ecaf49 end
                            end
                            local scratchValue8 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectFromMansLover)
                            quest:GiveHeroObject(scratchValue8, -1, false)
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TellingTheTruth))
                            quest:SetStateInt("MansLoverState", 2)
                        else
                            if scratchValue then goto LAB_00eca325 end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                scratchValue4 = 0
                                scratchValue3 = 1
                                scratchValue5 = 0
                                isPerformingScriptTask = 0
                                line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_20"
                                hero = quest:GetHero()
                                local speechResult10 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                                if not speechResult10 then
                                    goto LAB_00ecaf49
                                end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    isPerformingScriptTask = me:IsPerformingScriptTask()
                                    scratchValue2 = isPerformingScriptTask
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                scratchValue4 = 0
                                scratchValue3 = 1
                                scratchValue5 = 0
                                isPerformingScriptTask = 0
                                line = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_30"
                                hero = quest:GetHero()
                                local speechResult11 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                                if not speechResult11 then
                                    goto LAB_00ecaf49
                                end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    isPerformingScriptTask = me:IsPerformingScriptTask()
                                    scratchValue2 = isPerformingScriptTask
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            quest:GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", -1, false)
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                scratchValue4 = 0
                                scratchValue3 = 1
                                scratchValue5 = 0
                                isPerformingScriptTask = 0
                                line = "TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10"
                                hero = quest:GetHero()
                                local speechResult12 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                                if not speechResult12 then
                                    goto LAB_00ecaf49
                                end
                                isPerformingScriptTask = me:IsPerformingScriptTask()
                                scratchValue2 = isPerformingScriptTask
                                while scratchValue2 do
                                    quest:NewScriptFrame(me)
                                    scratchValue = quest:IsActiveThreadTerminating()
                                    if scratchValue then goto LAB_00ecaf49 end
                                    isPerformingScriptTask = me:IsPerformingScriptTask()
                                    scratchValue2 = isPerformingScriptTask
                                end
                                scratchValue = quest:IsActiveThreadTerminating()
                                if scratchValue then goto LAB_00eca325 end
                            end
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LyingToWomanMorality))
                            quest:SetStateInt("MansLoverState", 1)
                        end
                        quest:ClearThingHasInformation(me)
                        leavingRegion_ = true
                        goto FLOW_past_lab_00ecae05
                    end
                end
                ::FLOW_past_lab_00eca2ed::
                goto LAB_00eca325
            end
            goto FLOW_past_lab_00eca325
            ::LAB_00eca325::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            break
            ::FLOW_past_lab_00eca325::
            if predicateResult then goto LAB_00ecaf49 end
            me:ClearCommands()
            if not leavingRegion_ then
                goto LAB_00ecab59
            else
                retailRandModulo = quest:RetailRandModulo(32768)
                retailRandModulo = retailRandModulo & 0x80000001
                scratchValue = retailRandModulo == 0
                if retailRandModulo < 0 then
                    scratchValue = (retailRandModulo - 1 | 0xfffffffe) == 0xffffffff
                end
                if not scratchValue then goto LAB_00ecab59 end
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then goto LAB_00eca325 end
                hero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    scratchValue4 = 0
                    scratchValue3 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10"
                    hero = quest:GetHero()
                    local speechResult13 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                    if not speechResult13 then
                        goto LAB_00ecaf49
                    end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue2 = isPerformingScriptTask
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00ecaf49 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
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
            goto FLOW_past_lab_00ecae05
            ::LAB_00ecaf49::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            break
            ::FLOW_hoist_lab_00ecaf49_1::
            mansLoverDialogueState = quest:GetStateInt("MansLoverState")
            if mansLoverDialogueState == 0 then
                hero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    scratchValue4 = 0
                    scratchValue3 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER"
                    hero = quest:GetHero()
                    local speechResult2 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                    if not speechResult2 then
                        goto LAB_00eca325
                    end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue2 = isPerformingScriptTask
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
                    end
                    goto LAB_00ecae05
                end
            elseif mansLoverDialogueState == 1 then
                hero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    scratchValue4 = 0
                    scratchValue3 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                    hero = quest:GetHero()
                    local speechResult3 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                    if not speechResult3 then
                        goto LAB_00eca325
                    end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue2 = isPerformingScriptTask
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
                    end
                    goto LAB_00ecae05
                end
            elseif mansLoverDialogueState == 2 then
                hero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    scratchValue4 = 0
                    scratchValue3 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN"
                    hero = quest:GetHero()
                    local speechResult4 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                    if not speechResult4 then
                        goto LAB_00eca325
                    end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue2 = isPerformingScriptTask
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
                    end
                    goto LAB_00ecae05
                end
            else
                hero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    scratchValue4 = 0
                    scratchValue3 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                    hero = quest:GetHero()
                    local speechResult5 = me:Speak(hero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue3 ~= 0, scratchValue4 ~= 0)
                    if not speechResult5 then
                        goto LAB_00eca325
                    end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue2 = isPerformingScriptTask
                    while scratchValue2 do
                        quest:NewScriptFrame(me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eca325 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue2 = isPerformingScriptTask
                    end
                    goto LAB_00ecae05
                end
            end
            goto FLOW_past_lab_00ecae05
            ::LAB_00ecae05::
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then goto LAB_00ecaf49 end
            ::FLOW_past_lab_00ecae05::
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        getStateBool = leavingRegion_
        if getStateBool then
            isPerformingScriptTask = me:IsPerformingScriptTask()
            getStateBool = not isPerformingScriptTask
        end
        if not getStateBool then
            quest:NewScriptFrame(me);
            scratchValue = quest:IsActiveThreadTerminating();
            goto continue_2
        end
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
    leavingRegion_ = false
end

-- MansLover.OnPersist (retail 0x00ecd930)
function OnPersist(quest, me, context)
    local leavingRegion = leavingRegion_ or false
    leavingRegion_ = quest:PersistTransferBool(context, "LeavingRegion", leavingRegion)
end

-- MansLover.OnPredicateFail (retail 0x00ec9cd0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateInt("MansLoverState", 3)
    end
end

