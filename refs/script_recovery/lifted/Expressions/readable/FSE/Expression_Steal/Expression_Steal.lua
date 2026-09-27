-- Readable native conversion: Expression_Steal. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Steal.Main (retail 0x00eebdf0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local progress, scratchValue, flag2UVar, flag3UVar
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local target = quest:GetHeroTargetedThing()
    if quest:IsEntityStealable(target) then
        if not quest:IsInMovieSequence() then
            local movie = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 5)
            flag2UVar = false
            flag3UVar = false
            local value = quest:GetConstantFPS() * (quest:GetStealDuration(target) / ((quest:GetHeroStatLevel(5) + 1.0) / (quest:GetHeroStatMax(5) + 1.0)))
            scratchValue = math.tointeger(math.modf(value))
            local opinionDeedId = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(hero, 6, target)
            repeat
                if not quest:NewScriptFrame() then goto LAB_00eec252 end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_STEAL") then
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
                progress = 1 - scratchValue / value
                if 1.0 < progress then
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
                if 0 < scratchValue then
                    scratchValue = scratchValue - 1
                end
            until flag2UVar
            quest:RemoveOpinionDeedStillSearchingForWitnesses(hero, opinionDeedId)
            quest:DisplayMiniGameInfo(false, 5)
            if not flag3UVar then
                if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                if target ~= nil and target:IsAlive() then
                    if (math.random(0, 32767) % 100) < progress * 100.0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        quest:EntitySetAsStolen(target)
                        quest:ChangeHeroMoralityDueToTheft()
                    end
                end
            end
            resources:DestroyMovie(movie)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            do return end
            ::LAB_00eec252::
            resources:DestroyMovie(movie)
            return
        end
        if quest:IsActiveThreadTerminating() then return end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Expression_Steal.Init (retail 0x00eebde0)
function Init(quest)
end

-- Expression_Steal.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

