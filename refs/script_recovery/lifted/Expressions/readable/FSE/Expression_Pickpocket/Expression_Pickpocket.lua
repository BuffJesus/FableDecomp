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
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, predicateResult4, predicateResult, progress
    local scratchValue11, target3, movie, scratchValue15
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local target = quest:GetHeroTargetedThing()
    if quest:IsEntityPickPocketable(target) then
        if not quest:IsInMovieSequence() then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:DisplayMiniGameInfo(true, 3)
            quest:EntitySetCutsceneBehaviour(target, CUTSCENE_BEHAVIOUR_PAUSED)
            quest:PauseAllNonScriptedEntities(true)
            scratchValue11 = 0
            predicateResult = false
            predicateResult2 = false
            predicateResult4 = false
            local scratchValue5 = (quest:GetHeroStatLevel(5) + 1.0) / (quest:GetHeroStatMax(5) + 1.0)
            local value = quest:GetConstantFPS() * (quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketDurationSeconds) / scratchValue5)
            scratchValue = math.tointeger(math.modf(value))
            scratchValue15 = scratchValue
            repeat
                if not quest:NewScriptFrame() then ReleaseEverything(); return end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKPOCKET") then
                    if predicateResult4 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        if 0 < scratchValue11 then
                            predicateResult4 = false
                        end
                    end
                    predicateResult = true
                end
                if hero_ ~= nil and hero_:MsgIsHitBy("") then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    predicateResult = true
                    predicateResult2 = true
                end
                if predicateResult4 then
                    if scratchValue11 == 0 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        predicateResult = true
                    end
                end
                progress = 1 - scratchValue / value
                if 1.0 < progress then
                    progress = 1.0
                end
                quest:UpdateMiniGameInfoBar(progress)
                target3 = quest:GetHeroTargetedThing()
                if target3 == nil or not (target3 ~= nil and target3:IsAlive()) then
                    goto LAB_00eeb3f9
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    if 1.0 <= progress then goto LAB_00eeb3f9 end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    if scratchValue < scratchValue15 then
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        if math.random(0, 32767) % 100 < quest:ReadGlobalGameData(SCRIPT_DEF.PickpocketSpottedChancePerSecond) then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                return
                            end
                            predicateResult4 = true
                            scratchValue11 = math.tointeger(math.modf(quest:GetConstantFPS() * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketSpottedDurationSeconds)))
                            quest:EntitySetFacingAngleTowardsThing(target3, hero_, true)
                        end
                        scratchValue15 = scratchValue
                    end
                end
                goto FLOW_past_lab_00eeb3f9
                ::LAB_00eeb3f9::
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                predicateResult = true
                ::FLOW_past_lab_00eeb3f9::
                if 0 < scratchValue then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    scratchValue = scratchValue - 1
                end
                if 0 < scratchValue11 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    scratchValue11 = scratchValue11 - 1
                end
            until predicateResult
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:PauseAllNonScriptedEntities(false)
            quest:EntitySetCutsceneBehaviour(target3, CUTSCENE_BEHAVIOUR_DEFAULT)
            quest:DisplayMiniGameInfo(false, 3)
            if predicateResult2 then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            if not (target3 ~= nil and target3:IsAlive()) then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                return
            end
            quest:EntitySetCutsceneBehaviour(target3, CUTSCENE_BEHAVIOUR_DEFAULT)
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                if target3 ~= nil and not target3:IsNull() then
                    -- TODO(native): GetPThing is not a ForgeFSE binding
                end
                quest:SendEntityEvent(19, hero_, target3)
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if not ((math.random(0, 32767) % 100) < progress * 100.0) then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                quest:EntitySetAsPickPocketed(target3)
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            return
        end
        if quest:IsActiveThreadTerminating() then return end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Expression_Pickpocket.Init (retail 0x00eeaee0)
function Init(quest)
end

-- Expression_Pickpocket.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

