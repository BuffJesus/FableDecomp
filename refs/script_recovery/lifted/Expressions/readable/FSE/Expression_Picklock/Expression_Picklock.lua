-- Readable native conversion: Expression_Picklock. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    PicklockDurationSeconds = 4124,  -- 8.0
}

-- Expression_Picklock.Main (retail 0x00eeb7b0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local progress, scratchValue9, flag2UVar, flag3UVar
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local target = quest:GetHeroTargetedThing()
    if quest:IsEntityPickLockable(target) then
        if not quest:IsInMovieSequence() then
            local movie = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 4)
            flag2UVar = false
            flag3UVar = false
            local scratchValue = (quest:GetHeroStatLevel(5) + 1.0) / (quest:GetHeroStatMax(5) + 1.0)
            local value = quest:GetConstantFPS() * (quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PicklockDurationSeconds) / scratchValue)
            scratchValue9 = math.tointeger(math.modf(value))
            local opinionDeedId = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(hero, 4, target)
            quest:StartSneaking()
            quest:EntitySetFacingAngleTowardsThing(hero, target, true)
            repeat
                if not quest:NewScriptFrame() then goto LAB_00eebc32 end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKLOCK") then
                    flag2UVar = true
                    flag3UVar = true
                end
                if hero ~= nil and hero:MsgIsHitBy("") then
                    flag2UVar = true
                    flag3UVar = true
                end
                if quest:IsDeedWitnessed(opinionDeedId) then
                    flag2UVar = true
                    flag3UVar = true
                end
                progress = 1 - scratchValue9 / value
                if 1.0 < progress then
                    progress = 1.0
                end
                quest:UpdateMiniGameInfoBar(progress)
                if not (target ~= nil and not target:IsNull()) or not (target ~= nil and target:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    flag3UVar = true
                    goto LAB_00eebb6c
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    if 1.0 <= progress then
                        if not quest:IsActiveThreadTerminating() then goto LAB_00eebb6c end
                        goto LAB_00eebc32
                    end
                end
                goto FLOW_past_lab_00eebb6c
                ::LAB_00eebb6c::
                flag2UVar = true
                ::FLOW_past_lab_00eebb6c::
                if 0 < scratchValue9 then
                    scratchValue9 = scratchValue9 - 1
                end
            until flag2UVar
            quest:RemoveOpinionDeedStillSearchingForWitnesses(hero, opinionDeedId)
            quest:DisplayMiniGameInfo(false, 4)
            if not flag3UVar then
                if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                if target ~= nil and target:IsAlive() then
                    if (math.random(0, 32767) % 100) < progress * 100.0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                        quest:EntitySetAsPickLocked(target)
                        quest:ChangeHeroMoralityDueToPicklock()
                    end
                end
            end
            resources:DestroyMovie(movie)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            do return end
            ::LAB_00eebc32::
            resources:DestroyMovie(movie)
            return
        end
        if quest:IsActiveThreadTerminating() then return end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Expression_Picklock.Init (retail 0x00eeb7a0)
function Init(quest)
end

-- Expression_Picklock.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

