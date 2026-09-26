-- Generated native draft: Expression_Picklock. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar4, cVar5, fVar2, fVar3, iVar10, iVar9, i_stk_2c, pCVar6, pCVar7, pCVar8, r1, uVar11, value, xStack_10, xStack_20, xStack_24, xStack_28
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
            uVar11 = in_stack_ffffffcc
            xStack_20 = quest:GetHeroStatLevel(5)
            -- TODO(native): xStack_24 = (CCharString)((float)(int)xStack_20 + 1.0);
            xStack_20 = quest:GetHeroStatMax(5)
            fVar3 = xStack_24 / (xStack_20 + 1.0)
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
                    uVar11 = CONCAT13(1,CONCAT12(1,uVar11))
                end
                pCVar6 = quest:GetHero()
                bVar4 = (pCVar6 ~= nil and pCVar6:MsgIsHitBy(""))
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    uVar11 = CONCAT13(1,CONCAT12(1,uVar11))
                end
                bVar4 = quest:IsDeedWitnessed(iVar10)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    uVar11 = CONCAT13(1,CONCAT12(1,uVar11))
                end
                -- TODO(native): xStack_28 = (CCharString)(_DAT_0122ded8 - (float)i_stk_2c / (float)value);
                if 1.0 < xStack_28 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
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
                    if bVar4 then goto LAB_00eebc32 end
                    uVar11 = CONCAT13(1,(int3)uVar11)
                    goto LAB_00eebb6c
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    if 1.0 <= 0x3f800000 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then goto LAB_00eebb6c end
                        goto LAB_00eebc32
                    end
                end
                goto FLOW_past_lab_00eebb6c
                ::LAB_00eebb6c::
                uVar11 = CONCAT13((uVar11 >> 0x18),CONCAT12(1,uVar11))
                ::FLOW_past_lab_00eebb6c::
                if 0 < i_stk_2c then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    i_stk_2c = i_stk_2c + -1
                end
            until not ((uVar11 >> 0x10) == 0)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                pCVar6 = quest:GetHero()
                quest:RemoveOpinionDeedStillSearchingForWitnesses(pCVar6, iVar10)
                quest:DisplayMiniGameInfo(false, 4)
                if (uVar11 >> 0x18) == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00eebc32 end
                    iVar9 = (r1 ~= nil and r1:IsAlive())
                    if iVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00eebc32 end
                        iVar9 = math.random(0, 32767)
                        -- TODO(native): xStack_20 = (CCharString)(iVar9 % 100);
                        if xStack_20 < 0x3f800000 * 100.0 then
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

