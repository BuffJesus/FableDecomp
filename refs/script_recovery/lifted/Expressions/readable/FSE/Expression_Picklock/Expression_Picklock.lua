-- Readable native conversion: Expression_Picklock. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    PicklockDurationSeconds = 4124,  -- 8.0
}

-- Expression_Picklock.Main (retail 0x00eeb7b0)
function Main(quest)
    return quest:WithRetailResources(function(resources)
    local picklockTargetOwner = 0
    local predicateResult, scratchValue, progress, entityPostOpinionDeedKeepSearchingForWitnesses
    local retailRandModulo, scratchValue7, flag2UVar, flag3UVar, getHero, getActiveQuestName, hero
    local scratchValue8, scratchValue9
    getHero = quest:GetHero()
    quest:RegisterQuestConsciousCondition(getHero)
    quest:NewScriptFrame()
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then
        resources:RetireThingOutput(picklockTargetOwner); picklockTargetOwner = 0
        return
    end
    scratchValue = false
    getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(getActiveQuestName, scratchValue)
    local target = quest:GetHeroTargetedThing()
    picklockTargetOwner = resources:TrackThingOutput(target)
    scratchValue = quest:IsEntityPickLockable(target)
    if scratchValue then
        scratchValue = quest:IsInMovieSequence()
        if not scratchValue then
            local movie = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 4)
            local scratchValue3 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.PicklockDurationSeconds)
            flag2UVar = false
            flag3UVar = false
            scratchValue9 = quest:GetHeroStatLevel(5)
            local scratchValue5 = scratchValue9 + 1.0
            scratchValue9 = quest:GetHeroStatMax(5)
            local scratchValue4 = scratchValue5 / (scratchValue9 + 1.0)
            scratchValue9 = quest:GetConstantFPS()
            local value = scratchValue9 * (scratchValue3 / scratchValue4)
            scratchValue7 = math.tointeger(math.modf(value))
            getHero = target
            entityPostOpinionDeedKeepSearchingForWitnesses = 4
            hero = quest:GetHero()
            entityPostOpinionDeedKeepSearchingForWitnesses = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(hero, entityPostOpinionDeedKeepSearchingForWitnesses, getHero)
            quest:StartSneaking()
            scratchValue = true
            getHero = target
            hero = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(hero, getHero, scratchValue)
            repeat
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then goto LAB_00eebc32 end
                scratchValue = quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKLOCK")
                if not scratchValue then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    flag2UVar = true
                    flag3UVar = true
                end
                getHero = quest:GetHero()
                scratchValue = getHero ~= nil and getHero:MsgIsHitBy("")
                if scratchValue then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    flag2UVar = true
                    flag3UVar = true
                end
                scratchValue = quest:IsDeedWitnessed(entityPostOpinionDeedKeepSearchingForWitnesses)
                if scratchValue then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    flag2UVar = true
                    flag3UVar = true
                end
                progress = 1 - scratchValue7 / value
                if 1.0 < progress then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    progress = 1.0
                end
                quest:UpdateMiniGameInfoBar(progress)
                predicateResult = not (target ~= nil and not target:IsNull())
                if not predicateResult then
                    local scratchValue2 = target ~= nil and target:IsAlive()
                    predicateResult = not scratchValue2
                end
                if predicateResult then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    flag3UVar = true
                    goto LAB_00eebb6c
                else
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    if 1.0 <= progress then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if not scratchValue then goto LAB_00eebb6c end
                        goto LAB_00eebc32
                    end
                end
                goto FLOW_past_lab_00eebb6c
                ::LAB_00eebb6c::
                flag2UVar = true
                ::FLOW_past_lab_00eebb6c::
                if 0 < scratchValue7 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    scratchValue7 = scratchValue7 - 1
                end
            until flag2UVar
            scratchValue = quest:IsActiveThreadTerminating()
            if not scratchValue then
                getHero = quest:GetHero()
                quest:RemoveOpinionDeedStillSearchingForWitnesses(getHero, entityPostOpinionDeedKeepSearchingForWitnesses)
                quest:DisplayMiniGameInfo(false, 4)
                if not flag3UVar then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00eebc32 end
                    retailRandModulo = target ~= nil and target:IsAlive()
                    if retailRandModulo then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00eebc32 end
                        retailRandModulo = quest:RetailRandModulo(32768)
                        local scratchValue6 = retailRandModulo % 100
                        if scratchValue6 < progress * 100.0 then
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00eebc32 end
                            quest:EntitySetAsPickLocked(target)
                            quest:ChangeHeroMoralityDueToPicklock()
                        end
                    end
                end
                resources:DestroyMovie(movie)
                scratchValue8 = 0
                getActiveQuestName = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(getActiveQuestName, scratchValue8)
                resources:RetireThingOutput(picklockTargetOwner); picklockTargetOwner = 0
                return
            end
            ::LAB_00eebc32::
            resources:DestroyMovie(movie)
            resources:RetireThingOutput(picklockTargetOwner); picklockTargetOwner = 0
            return
        end
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then goto LAB_00eeb8af end
    else
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            resources:RetireThingOutput(picklockTargetOwner); picklockTargetOwner = 0
            return
        end
    end
    scratchValue8 = 0
    getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, scratchValue8)
    ::LAB_00eeb8af::
    resources:RetireThingOutput(picklockTargetOwner); picklockTargetOwner = 0
    end)
end

-- Expression_Picklock.Init (retail 0x00eeb7a0)
function Init(quest)
end

-- Expression_Picklock.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

