-- Readable native conversion: Expression_Steal. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Steal.Main (retail 0x00eebdf0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue7, scratchValue8, scratchValue9, scratchValue10
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    local getHeroTargetedThing = quest:GetHeroTargetedThing()
    if quest:IsEntityStealable(getHeroTargetedThing) then
        if not quest:IsInMovieSequence() then
            local movie = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 5)
            scratchValue8 = in_stack_ffffffcc
            local getStealDuration = quest:GetStealDuration(getHeroTargetedThing)
            -- TODO(native): xStack_24 = (CCharString)((float)(int)xStack_20 + 1.0);
            local getConstantFPS = quest:GetConstantFPS()
            scratchValue7 = math.tointeger(math.modf(getConstantFPS * (getStealDuration / (scratchValue9 / (quest:GetHeroStatMax(5) + 1.0)))))
            local scratchValue = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(hero, 6, getHeroTargetedThing)
            repeat
                if not quest:NewScriptFrame() then goto LAB_00eec252 end
                if not quest:IsDPadButtonHeldForExpression("EXPRESSION_STEAL") then
                    scratchValue8 = CONCAT13(1,CONCAT12(1,scratchValue8))
                end
                if hero ~= nil and hero:MsgIsHitBy("") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    scratchValue8 = CONCAT13(1,CONCAT12(1,scratchValue8))
                end
                if quest:IsDeedWitnessed(scratchValue) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    scratchValue8 = CONCAT13(1,CONCAT12(1,scratchValue8))
                end
                -- TODO(native): xStack_28 = (CCharString)(_DAT_0122ded8 - (float)i_stk_2c / (float)value);
                if 1.0 < scratchValue10 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                end
                quest:UpdateMiniGameInfoBar(1.0)
                if not (getHeroTargetedThing ~= nil and not getHeroTargetedThing:IsNull()) or not (getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    scratchValue8 = CONCAT13(1,int3scratchValue8)
                    goto LAB_00eec18c
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    if 1.0 <= 0x3f800000 then
                        if not quest:IsActiveThreadTerminating() then goto LAB_00eec18c end
                        goto LAB_00eec252
                    end
                end
                goto FLOW_past_lab_00eec18c
                ::LAB_00eec18c::
                scratchValue8 = CONCAT13(scratchValue8 >> 24,CONCAT12(1,scratchValue8))
                ::FLOW_past_lab_00eec18c::
                if 0 < scratchValue7 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                    scratchValue7 = scratchValue7 - 1
                end
            until scratchValue8 >> 16 ~= 0
            if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
            quest:RemoveOpinionDeedStillSearchingForWitnesses(hero, scratchValue)
            quest:DisplayMiniGameInfo(false, 5)
            if scratchValue8 >> 24 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                if getHeroTargetedThing ~= nil and getHeroTargetedThing:IsAlive() then
                    math.random(0, 32767)
                    -- TODO(native): xStack_20 = (CCharString)(iVar8 % 100);
                    if getConstantFPS < 0x3f800000 * 100.0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eec252 end
                        quest:EntitySetAsStolen(getHeroTargetedThing)
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
        if quest:IsActiveThreadTerminating() then goto LAB_00eebeef end
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::LAB_00eebeef::
end

-- Expression_Steal.Init (retail 0x00eebde0)
function Init(quest)
end

-- Expression_Steal.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

