-- Generated native draft: Expression_Steal. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar4, cVar5, fVar2, fVar3, f_stk_24, f_stk_28, iVar8, iVar9, i_stk_20, i_stk_2c, native_arg_flag2_uVar10, native_arg_flag3_uVar10, pActor, pCVar6, pCVar7, r1, uVar10, value, xStack_10, xStack_20
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
            native_arg_flag2_uVar10 = false
            native_arg_flag3_uVar10 = false
            xStack_20 = quest:GetStealDuration(r1)
            fVar2 = xStack_20
            xStack_20 = quest:GetHeroStatLevel(5)
            f_stk_24 = (xStack_20 + 1.0)
            xStack_20 = quest:GetHeroStatMax(5)
            fVar3 = f_stk_24 / (xStack_20 + 1.0)
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
                    native_arg_flag2_uVar10 = true
                    native_arg_flag3_uVar10 = true
                end
                pCVar6 = quest:GetHero()
                bVar4 = (pCVar6 ~= nil and pCVar6:MsgIsHitBy(""))
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    native_arg_flag2_uVar10 = true
                    native_arg_flag3_uVar10 = true
                end
                bVar4 = quest:IsDeedWitnessed(iVar8)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    native_arg_flag2_uVar10 = true
                    native_arg_flag3_uVar10 = true
                end
                f_stk_28 = (1 - i_stk_2c / value)
                if 1.0 < f_stk_28 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    f_stk_28 = 1.0
                end
                quest:UpdateMiniGameInfoBar(f_stk_28)
                __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
                if not __native_condition_1 then
                    cVar5 = (r1 ~= nil and r1:IsAlive())
                    __native_condition_1 = not cVar5
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    native_arg_flag3_uVar10 = true
                    goto LAB_00eec18c
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    if 1.0 <= f_stk_28 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then goto LAB_00eec18c end
                        goto LAB_00eec252
                    end
                end
                goto FLOW_past_lab_00eec18c
                ::LAB_00eec18c::
                native_arg_flag2_uVar10 = true
                ::FLOW_past_lab_00eec18c::
                if 0 < i_stk_2c then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    i_stk_2c = i_stk_2c + -1
                end
            until not (not native_arg_flag2_uVar10)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                pCVar6 = quest:GetHero()
                quest:RemoveOpinionDeedStillSearchingForWitnesses(pCVar6, iVar8)
                quest:DisplayMiniGameInfo(false, 5)
                if not native_arg_flag3_uVar10 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eec252 end
                    iVar8 = (r1 ~= nil and r1:IsAlive())
                    if iVar8 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00eec252 end
                        iVar8 = math.random(0, 32767)
                        i_stk_20 = (iVar8 % 100)
                        if i_stk_20 < f_stk_28 * 100.0 then
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

