-- Readable native conversion: Expression_Steal. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Steal.Main (retail 0x00eebdf0)
function Main(quest)
    return quest:WithRetailResources(function(resources)
        local stealTargetOwner = 0
        local progress, scratchValue7, flag2UVar, flag3UVar, getActiveQuestName3
        local hero = quest:GetHero()
        quest:RegisterQuestConsciousCondition(hero)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:RetireThingOutput(stealTargetOwner); stealTargetOwner = 0
            return
        end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsPersistent(getActiveQuestName, false)
        local target = quest:GetHeroTargetedThing()
        stealTargetOwner = resources:TrackThingOutput(target)
        if quest:IsEntityStealable(target) then
            if not quest:IsInMovieSequence() then
                local movie = resources:StartMovie("")
                quest:DisplayMiniGameInfo(true, 5)
                flag2UVar = false
                flag3UVar = false
                local getStealDuration = quest:GetStealDuration(target)
                local scratchValue = (quest:GetHeroStatLevel(5) + 1.0) / (quest:GetHeroStatMax(5) + 1.0)
                local value = quest:GetConstantFPS() * (getStealDuration / scratchValue)
                scratchValue7 = math.tointeger(math.modf(value))
                local pActor = quest:GetHero()
                local opinionDeedId = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(pActor, 6, target)
                repeat
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    if not quest:IsDPadButtonHeldForExpression("EXPRESSION_STEAL") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        flag2UVar = true
                        flag3UVar = true
                    end
                    local hero2 = quest:GetHero()
                    if hero2 ~= nil and hero2:MsgIsHitBy("") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        flag2UVar = true
                        flag3UVar = true
                    end
                    if quest:IsDeedWitnessed(opinionDeedId) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        flag2UVar = true
                        flag3UVar = true
                    end
                    progress = 1 - scratchValue7 / value
                    if 1.0 < progress then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        progress = 1.0
                    end
                    quest:UpdateMiniGameInfoBar(progress)
                    if not (target ~= nil and not target:IsNull()) or not (target ~= nil and target:IsAlive()) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        flag3UVar = true
                        goto LAB_00eec18c
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        if 1.0 <= progress then
                            if not quest:IsActiveThreadTerminating() then goto LAB_00eec18c end
                            goto LAB_00eec252
                        end
                    end
                    goto FLOW_past_lab_00eec18c
                    ::LAB_00eec18c::
                    flag2UVar = true
                    ::FLOW_past_lab_00eec18c::
                    if 0 < scratchValue7 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        scratchValue7 = scratchValue7 - 1
                    end
                until flag2UVar
                if not quest:IsActiveThreadTerminating() then
                    local hero3 = quest:GetHero()
                    quest:RemoveOpinionDeedStillSearchingForWitnesses(hero3, opinionDeedId)
                    quest:DisplayMiniGameInfo(false, 5)
                    if not flag3UVar then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        if target ~= nil and target:IsAlive() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                            if (quest:RetailRandModulo(32768) % 100) < progress * 100.0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                                quest:EntitySetAsStolen(target)
                                quest:ChangeHeroMoralityDueToTheft()
                            end
                        end
                    end
                    resources:DestroyMovie(movie)
                    local getActiveQuestName2 = quest:GetActiveQuestName()
                    quest:DeactivateQuestLater(getActiveQuestName2, 0)
                    resources:RetireThingOutput(stealTargetOwner); stealTargetOwner = 0
                    return
                end
                ::LAB_00eec252::
                resources:DestroyMovie(movie)
                resources:RetireThingOutput(stealTargetOwner); stealTargetOwner = 0
                return
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00eebeef end
        elseif quest:IsActiveThreadTerminating() then
            resources:RetireThingOutput(stealTargetOwner); stealTargetOwner = 0
            return
        end
        getActiveQuestName3 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(getActiveQuestName3, 0)
        ::LAB_00eebeef::
        resources:RetireThingOutput(stealTargetOwner); stealTargetOwner = 0
    end)
end

-- Expression_Steal.Init (retail 0x00eebde0)
function Init(quest)
end

-- Expression_Steal.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

