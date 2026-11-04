-- Readable native conversion: Expression_Pickpocket. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_DEFAULT = 0  -- ECutsceneBehaviour (Ego_r.pdb)
local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    PickpocketDurationSeconds = 4112,  -- 2.0
    PickpocketSpottedDurationSeconds = 4116,  -- 1.0
    PickpocketSpottedChancePerSecond = 4120,  -- 10
}

-- Expression_Pickpocket.Main (retail 0x00eeaef0)
function Main(quest)
    return quest:WithRetailResources(function(resources)
    local pickpocketTargetOwner = 0
    local pickpocketTemporaryOwner = 0
    local scratchValue, predicateResult, predicateResult2, scratchValue2, scratchValue3
    local isActiveThreadTerminating, progress, retailRandModulo, scratchValue9, scratchValue10
    local getActiveQuestName, scratchValue11, movie, scratchValue12
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
    end
    scratchValue10 = quest:GetHero()
    quest:RegisterQuestConsciousCondition(scratchValue10)
    quest:NewScriptFrame()
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
        return
    end
    scratchValue2 = false
    getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(getActiveQuestName, scratchValue2)
    local target = quest:GetHeroTargetedThing()
    pickpocketTargetOwner = resources:TrackThingOutput(target)
    scratchValue2 = quest:IsEntityPickPocketable(target)
    if scratchValue2 then
        scratchValue2 = quest:IsInMovieSequence()
        if not scratchValue2 then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:DisplayMiniGameInfo(true, 3)
            quest:EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_PAUSED)
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue5 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketDurationSeconds)
            scratchValue9 = 0
            isActiveThreadTerminating = false
            predicateResult2 = false
            scratchValue2 = false
            scratchValue12 = quest:GetHeroStatLevel(5)
            local scratchValue7 = scratchValue12 + 1.0
            scratchValue12 = quest:GetHeroStatMax(5)
            local scratchValue6 = scratchValue7 / (scratchValue12 + 1.0)
            scratchValue12 = quest:GetConstantFPS()
            local value = scratchValue12 * (scratchValue5 / scratchValue6)
            scratchValue = math.tointeger(math.modf(value))
            scratchValue12 = scratchValue
            repeat
                quest:NewScriptFrame()
                scratchValue3 = quest:IsActiveThreadTerminating()
                if scratchValue3 then
                    ReleaseEverything()
                    return
                end
                scratchValue3 = quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKPOCKET")
                if not scratchValue3 then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if isActiveThreadTerminating then ReleaseEverything(); return end
                    if scratchValue2 then
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if isActiveThreadTerminating then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                            return
                        end
                        if 0 < scratchValue9 then
                            scratchValue2 = quest:IsActiveThreadTerminating()
                            if scratchValue2 then ReleaseEverything(); return end
                            scratchValue2 = false
                        end
                    end
                    isActiveThreadTerminating = true
                end
                scratchValue10 = quest:GetHero()
                scratchValue3 = scratchValue10 ~= nil and scratchValue10:MsgIsHitBy("")
                if scratchValue3 then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if isActiveThreadTerminating then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                        return
                    end
                    isActiveThreadTerminating = true
                    predicateResult2 = true
                end
                if scratchValue2 then
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then ReleaseEverything(); return end
                    if scratchValue9 == 0 then
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if isActiveThreadTerminating then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                            return
                        end
                        isActiveThreadTerminating = true
                    end
                end
                progress = 1 - scratchValue / value
                if 1.0 < progress then
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then ReleaseEverything(); return end
                    progress = 1.0
                end
                quest:UpdateMiniGameInfoBar(progress)
                scratchValue10 = quest:GetHeroTargetedThing()
                pickpocketTemporaryOwner = resources:TrackThingOutput(scratchValue10)
                resources:AssignThingOutput(pickpocketTargetOwner, scratchValue10)
                resources:RetireThingOutput(pickpocketTemporaryOwner); pickpocketTemporaryOwner = 0
                scratchValue10 = nil
                predicateResult = not (target ~= nil and not target:IsNull())
                if not predicateResult then
                    local scratchValue4 = target ~= nil and target:IsAlive()
                    predicateResult = not scratchValue4
                end
                if predicateResult then
                    goto LAB_00eeb3f9
                else
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                        return
                    end
                    if 1.0 <= progress then goto LAB_00eeb3f9 end
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                        return
                    end
                    if scratchValue < scratchValue12 then
                        scratchValue3 = quest:IsActiveThreadTerminating()
                        if scratchValue3 then ReleaseEverything(); return end
                        retailRandModulo = quest:RetailRandModulo(32768)
                        if retailRandModulo % 100 < quest:ReadGlobalGameData(SCRIPT_DEF.PickpocketSpottedChancePerSecond) then
                            scratchValue2 = quest:IsActiveThreadTerminating()
                            if scratchValue2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                                return
                            end
                            scratchValue2 = true
                            scratchValue12 = quest:GetConstantFPS()
                            scratchValue9 = math.tointeger(math.modf(scratchValue12 * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketSpottedDurationSeconds)))
                            scratchValue3 = true
                            scratchValue10 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(target, scratchValue10, scratchValue3)
                        end
                        scratchValue12 = scratchValue
                    end
                end
                goto FLOW_past_lab_00eeb3f9
                ::LAB_00eeb3f9::
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then ReleaseEverything(); return end
                isActiveThreadTerminating = true
                ::FLOW_past_lab_00eeb3f9::
                if 0 < scratchValue then
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                        return
                    end
                    scratchValue = scratchValue - 1
                end
                if 0 < scratchValue9 then
                    scratchValue3 = quest:IsActiveThreadTerminating()
                    if scratchValue3 then ReleaseEverything(); return end
                    scratchValue9 = scratchValue9 - 1
                end
            until isActiveThreadTerminating
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then ReleaseEverything(); return end
            quest:PauseAllNonScriptedEntities(false)
            quest:EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_DEFAULT)
            quest:DisplayMiniGameInfo(false, 3)
            if not predicateResult2 then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then
                    ReleaseEverything()
                    return
                end
                retailRandModulo = target ~= nil and target:IsAlive()
                if retailRandModulo then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if isActiveThreadTerminating then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                        return
                    end
                    quest:EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_DEFAULT)
                    if scratchValue2 then
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                            return
                        end
                        resources:SendEntityEventFromHeroWithThingData(19, target, target)
                    else
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then ReleaseEverything(); return end
                        retailRandModulo = quest:RetailRandModulo(32768)
                        local scratchValue8 = retailRandModulo % 100
                        if scratchValue8 < progress * 100.0 then
                            scratchValue2 = quest:IsActiveThreadTerminating()
                            if scratchValue2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
                                return
                            end
                            quest:EntitySetAsPickPocketed(target)
                        end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            scratchValue11 = 0
            getActiveQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName, scratchValue11)
            resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
            return
        end
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
            return
        end
    else
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then goto LAB_00eeafc7 end
    end
    scratchValue11 = 0
    getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, scratchValue11)
    ::LAB_00eeafc7::
    resources:RetireThingOutput(pickpocketTargetOwner); pickpocketTargetOwner = 0
    end)
end

-- Expression_Pickpocket.Init (retail 0x00eeaee0)
function Init(quest)
end

-- Expression_Pickpocket.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

