-- Generated native draft: Expression_Picklock. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar4, cVar5, fVar2, fVar3, f_stk_24, f_stk_28, iVar10, iVar9, i_stk_20, i_stk_2c, native_arg_flag2_uVar11, native_arg_flag3_uVar11, pCVar6, pCVar7, pCVar8, r1, uVar11, value, xStack_10, xStack_20
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
    bVar4 = quest:IsEntityPickLockable(r1)
    if bVar4 then
        bVar4 = quest:IsInMovieSequence()
        if not bVar4 then
            xStack_10 = resources:StartMovie("")
            quest:DisplayMiniGameInfo(true, 4)
            fVar2 = quest:ReadGlobalGameDataFloat(0x101c)
            native_arg_flag2_uVar11 = false
            native_arg_flag3_uVar11 = false
            xStack_20 = quest:GetHeroStatLevel(5)
            f_stk_24 = (xStack_20 + 1.0)
            xStack_20 = quest:GetHeroStatMax(5)
            fVar3 = f_stk_24 / (xStack_20 + 1.0)
            xStack_20 = quest:GetConstantFPS()
            value = xStack_20 * (fVar2 / fVar3)
            i_stk_2c = math.tointeger(math.modf(value))
            pCVar6 = r1
            iVar10 = 4
            pCVar8 = quest:GetHero()
            iVar10 = quest:EntityPostOpinionDeedKeepSearchingForWitnesses(pCVar8, iVar10, pCVar6)
            quest:StartSneaking()
            bVar4 = true
            pCVar6 = r1
            pCVar8 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(pCVar8, pCVar6, bVar4)
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00eebc32 end
                bVar4 = quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKLOCK")
                if not bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    native_arg_flag2_uVar11 = true
                    native_arg_flag3_uVar11 = true
                end
                pCVar6 = quest:GetHero()
                bVar4 = (pCVar6 ~= nil and pCVar6:MsgIsHitBy(""))
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    native_arg_flag2_uVar11 = true
                    native_arg_flag3_uVar11 = true
                end
                bVar4 = quest:IsDeedWitnessed(iVar10)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    native_arg_flag2_uVar11 = true
                    native_arg_flag3_uVar11 = true
                end
                f_stk_28 = (1 - i_stk_2c / value)
                if 1.0 < f_stk_28 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
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
                    if bVar4 then goto LAB_00eebc32 end
                    native_arg_flag3_uVar11 = true
                    goto LAB_00eebb6c
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    if 1.0 <= f_stk_28 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then goto LAB_00eebb6c end
                        goto LAB_00eebc32
                    end
                end
                goto FLOW_past_lab_00eebb6c
                ::LAB_00eebb6c::
                native_arg_flag2_uVar11 = true
                ::FLOW_past_lab_00eebb6c::
                if 0 < i_stk_2c then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    i_stk_2c = i_stk_2c + -1
                end
            until not (not native_arg_flag2_uVar11)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                pCVar6 = quest:GetHero()
                quest:RemoveOpinionDeedStillSearchingForWitnesses(pCVar6, iVar10)
                quest:DisplayMiniGameInfo(false, 4)
                if not native_arg_flag3_uVar11 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    iVar9 = (r1 ~= nil and r1:IsAlive())
                    if iVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00eebc32 end
                        iVar9 = math.random(0, 32767)
                        i_stk_20 = (iVar9 % 100)
                        if i_stk_20 < f_stk_28 * 100.0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00eebc32 end
                            quest:EntitySetAsPickLocked(r1)
                            quest:ChangeHeroMoralityDueToPicklock()
                        end
                    end
                end
                resources:DestroyMovie(xStack_10)
                uVar11 = 0
                pCVar7 = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pCVar7, uVar11)
                return
            end
            ::LAB_00eebc32::
            resources:DestroyMovie(xStack_10)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00eeb8af end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
    end
    uVar11 = 0
    pCVar7 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar7, uVar11)
    ::LAB_00eeb8af::
end

function Init(quest)
end

function OnPersist(quest, context)
end

