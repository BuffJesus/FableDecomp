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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, predicateResult4, predicateResult, scratchValue9
    local scratchValue12, movie, getConstantFPS, scratchValue15, scratchValue16, scratchValue17
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local getHeroTargetedThing = quest:GetHeroTargetedThing()
    if quest:IsEntityPickPocketable(getHeroTargetedThing) then
        if not quest:IsInMovieSequence() then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:DisplayMiniGameInfo(true, 3)
            quest:EntitySetCutsceneBehaviour(getHeroTargetedThing, CUTSCENE_BEHAVIOUR_PAUSED)
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue4 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketDurationSeconds)
            scratchValue9 = 0
            predicateResult = false
            predicateResult2 = false
            predicateResult4 = false
            -- TODO(native): xStack_30 = (CCharString)((float)(int)xStack_2c + 1.0);
            scratchValue = math.tointeger(math.modf(quest:GetConstantFPS() * (scratchValue4 / (scratchValue15 / (quest:GetHeroStatMax(5) + 1.0)))))
            getConstantFPS = scratchValue
            repeat
                if not quest:NewScriptFrame() then ReleaseEverything(); return end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKPOCKET") then
                    if predicateResult4 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        if 0 < scratchValue9 then
                            predicateResult4 = false
                        end
                    end
                    predicateResult = true
                end
                if hero ~= nil and hero:MsgIsHitBy("") then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    predicateResult = true
                    predicateResult2 = true
                end
                if predicateResult4 then
                    if scratchValue9 == 0 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        predicateResult = true
                    end
                end
                -- TODO(native): xStack_38 = (CCharString)(_DAT_0122ded8 - (float)(int)C_stk_40 / (float)value);
                if 1.0 < scratchValue16 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                quest:UpdateMiniGameInfoBar(1.0)
                -- TODO(native): piVar3 = *(pCVar11 + 0x8)
                local scratchValue11 = nil --[[unresolved native value]]
                -- TODO(native): piVar4 = *(pCVar11 + 0x4)
                scratchValue12 = nil --[[unresolved native value]]
                if scratchValue17 ~= scratchValue11 then
                    -- TODO(native): xStack_28._4_4_ = piVar4;
                    scratchValue17 = scratchValue11
                    if scratchValue11 ~= nil then
                        -- TODO(native): *piVar3 = *piVar3 + 1;
                    end
                end
                if not (getHeroTargetedThing ~= nil and not getHeroTargetedThing:IsNull()) or not (getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive()) then
                    goto LAB_00eeb3f9
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    if 1.0 <= 0x3f800000 then goto LAB_00eeb3f9 end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    if scratchValue < getConstantFPS then
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        if math.random(0, 32767) % 100 < quest:ReadGlobalGameData(SCRIPT_DEF.PickpocketSpottedChancePerSecond) then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                return
                            end
                            predicateResult4 = true
                            getConstantFPS = quest:GetConstantFPS()
                            scratchValue9 = math.tointeger(math.modf(getConstantFPS * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PickpocketSpottedDurationSeconds)))
                            quest:EntitySetFacingAngleTowardsThing(getHeroTargetedThing, hero, true)
                        end
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
                if 0 < scratchValue9 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            until predicateResult
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:PauseAllNonScriptedEntities(false)
            quest:EntitySetCutsceneBehaviour(getHeroTargetedThing, CUTSCENE_BEHAVIOUR_DEFAULT)
            quest:DisplayMiniGameInfo(false, 3)
            if predicateResult2 then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            if not (getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive()) then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                return
            end
            quest:EntitySetCutsceneBehaviour(getHeroTargetedThing, CUTSCENE_BEHAVIOUR_DEFAULT)
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                if getHeroTargetedThing ~= nil and not getHeroTargetedThing:IsNull() then
                    -- TODO(native): GetPThing is not a ForgeFSE binding
                end
                quest:SendEntityEvent(19, hero, getHeroTargetedThing)
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                math.random(0, 32767)
                -- TODO(native): xStack_2c = (CCharString)(iVar13 % 100);
                if not (getConstantFPS < 0x3f800000 * 100.0) then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
                quest:EntitySetAsPickPocketed(getHeroTargetedThing)
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            return
        end
        if quest:IsActiveThreadTerminating() then return end
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00eeafc7 end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::LAB_00eeafc7::
end

-- Expression_Pickpocket.Init (retail 0x00eeaee0)
function Init(quest)
end

-- Expression_Pickpocket.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

