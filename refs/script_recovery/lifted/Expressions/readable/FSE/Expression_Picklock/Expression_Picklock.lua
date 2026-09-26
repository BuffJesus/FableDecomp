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
    local scratchValue8, scratchValue9, scratchValue10, scratchValue11
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local getHeroTargetedThing = quest:GetHeroTargetedThing()
    if quest:IsEntityPickLockable(getHeroTargetedThing) then
        if not quest:IsInMovieSequence() then
            local movie = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 4)
            local scratchValue = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PicklockDurationSeconds)
            scratchValue9 = in_stack_ffffffcc
            -- TODO(native): xStack_24 = (CCharString)((float)(int)xStack_20 + 1.0);
            local getConstantFPS = quest:GetConstantFPS()
            scratchValue8 = math.tointeger(math.modf(getConstantFPS * (scratchValue / (scratchValue10 / (quest:GetHeroStatMax(5) + 1.0)))))
            local scratchValue5 = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(hero, 4, getHeroTargetedThing)
            quest:StartSneaking()
            quest:EntitySetFacingAngleTowardsThing(hero, getHeroTargetedThing, true)
            repeat
                if not quest:NewScriptFrame() then goto LAB_00eebc32 end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKLOCK") then
                    scratchValue9 = CONCAT13(1,CONCAT12(1,scratchValue9))
                end
                if hero ~= nil and hero:MsgIsHitBy("") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    scratchValue9 = CONCAT13(1,CONCAT12(1,scratchValue9))
                end
                if quest:IsDeedWitnessed(scratchValue5) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    scratchValue9 = CONCAT13(1,CONCAT12(1,scratchValue9))
                end
                -- TODO(native): xStack_28 = (CCharString)(_DAT_0122ded8 - (float)i_stk_2c / (float)value);
                if 1.0 < scratchValue11 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                end
                quest:UpdateMiniGameInfoBar(1.0)
                if not (getHeroTargetedThing ~= nil and not getHeroTargetedThing:IsNull()) or not (getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    scratchValue9 = CONCAT13(1,int3scratchValue9)
                    goto LAB_00eebb6c
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    if 1.0 <= 0x3f800000 then
                        if not quest:IsActiveThreadTerminating() then goto LAB_00eebb6c end
                        goto LAB_00eebc32
                    end
                end
                goto FLOW_past_lab_00eebb6c
                ::LAB_00eebb6c::
                scratchValue9 = CONCAT13(scratchValue9 >> 24,CONCAT12(1,scratchValue9))
                ::FLOW_past_lab_00eebb6c::
                if 0 < scratchValue8 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                    scratchValue8 = scratchValue8 - 1
                end
            until scratchValue9 >> 16 ~= 0
            if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
            quest:RemoveOpinionDeedStillSearchingForWitnesses(hero, scratchValue5)
            quest:DisplayMiniGameInfo(false, 4)
            if scratchValue9 >> 24 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                if getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive() then
                    math.random(0, 32767)
                    -- TODO(native): xStack_20 = (CCharString)(iVar9 % 100);
                    if getConstantFPS < 0x3f800000 * 100.0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eebc32 end
                        quest:EntitySetAsPickLocked(getHeroTargetedThing)
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
        if quest:IsActiveThreadTerminating() then goto LAB_00eeb8af end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::LAB_00eeb8af::
end

-- Expression_Picklock.Init (retail 0x00eeb7a0)
function Init(quest)
end

-- Expression_Picklock.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

