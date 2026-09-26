-- Generated native draft: Expression_Steal. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar4, cVar5, fVar2, fVar3, iVar8, iVar9, i_stk_2c, pActor, pCVar6, pCVar7, r1, uVar10, value, xStack_10, xStack_20, xStack_24, xStack_28
    local alive = true
    pCVar6 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    bVar4 = false
    pCVar7 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar7, bVar4)
    r1 = quest:GetHeroTargetedThing()
    bVar4 = quest:IsEntityStealable(r1)
    if bVar4 then
        bVar4 = quest:IsInMovieSequence()
        if not bVar4 then
            xStack_10 = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 5)
            uVar10 = in_stack_ffffffcc
            xStack_20 = quest:GetStealDuration(r1)
            fVar2 = xStack_20
            xStack_20 = quest:GetHeroStatLevel(5)
            -- TODO(native): xStack_24 = (CCharString)((float)(int)xStack_20 + 1.0);
            xStack_20 = quest:GetHeroStatMax(5)
            fVar3 = xStack_24 / (xStack_20 + 1.0)
            xStack_20 = quest:GetConstantFPS()
            value = xStack_20 * (fVar2 / fVar3)
            i_stk_2c = math.tointeger(math.modf(value))
            pCVar6 = r1
            iVar9 = 6
            pActor = quest:GetHero()
            iVar8 = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(pActor, iVar9, pCVar6)
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00eec252 end
                bVar4 = quest:IsDPadButtonHeldForExpression("EXPRESSION_STEAL")
                if not bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    uVar10 = CONCAT13(1,CONCAT12(1,uVar10))
                end
                pCVar6 = quest:GetHero()
                bVar4 = (pCVar6 ~= nil and pCVar6:MsgIsHitBy(""))
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    uVar10 = CONCAT13(1,CONCAT12(1,uVar10))
                end
                bVar4 = quest:IsDeedWitnessed(iVar8)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    uVar10 = CONCAT13(1,CONCAT12(1,uVar10))
                end
                -- TODO(native): xStack_28 = (CCharString)(_DAT_0122ded8 - (float)i_stk_2c / (float)value);
                if 1.0 < xStack_28 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                end
                quest:UpdateMiniGameInfoBar(1.0)
                __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
                if not __native_condition_1 then
                    cVar5 = (r1 ~= nil and r1:IsAlive())
                    __native_condition_1 = not cVar5
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    uVar10 = CONCAT13(1,(int3)uVar10)
                    goto LAB_00eec18c
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    if 1.0 <= 0x3f800000 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then goto LAB_00eec18c end
                        goto LAB_00eec252
                    end
                end
                goto FLOW_past_lab_00eec18c
                ::LAB_00eec18c::
                uVar10 = CONCAT13((uVar10 >> 0x18),CONCAT12(1,uVar10))
                ::FLOW_past_lab_00eec18c::
                if 0 < i_stk_2c then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    i_stk_2c = i_stk_2c + -1
                end
            until not ((uVar10 >> 0x10) == 0)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                pCVar6 = quest:GetHero()
                quest:RemoveOpinionDeedStillSearchingForWitnesses(pCVar6, iVar8)
                quest:DisplayMiniGameInfo(false, 5)
                if (uVar10 >> 0x18) == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    iVar8 = (r1 ~= nil and r1:IsAlive())
                    if iVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00eec252 end
                        iVar8 = math.random(0, 32767)
                        -- TODO(native): xStack_20 = (CCharString)(iVar8 % 100);
                        if xStack_20 < 0x3f800000 * 100.0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00eec252 end
                            quest:EntitySetAsStolen(r1)
                            quest:ChangeHeroMoralityDueToTheft()
                        end
                    end
                end
                resources:DestroyMovie(xStack_10)
                uVar10 = 0
                pCVar7 = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pCVar7, uVar10)
                return
            end
            ::LAB_00eec252::
            resources:DestroyMovie(xStack_10)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00eebeef end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
    end
    uVar10 = 0
    pCVar7 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar7, uVar10)
    ::LAB_00eebeef::
end

function Init(quest)
end

function OnPersist(quest, context)
end

