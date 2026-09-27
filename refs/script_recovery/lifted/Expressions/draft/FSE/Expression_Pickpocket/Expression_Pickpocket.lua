-- Generated native draft: Expression_Pickpocket. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local C_stk_40, __native_condition_1, bVar6, bVar7, bVar8, bVar9, cVar10, fVar2, fVar5, f_stk_30, f_stk_38, iVar13, i_stk_2c, i_stk_3c, pCVar11, pCVar12, pExtraData, pSender, r1, uVar14, value, xStack_10, xStack_2c
    local alive = true
    local function __cleanup_LAB_00eeb4fa()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00eeb5dd()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    pCVar11 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar7 = not alive
    if bVar7 then
        return
    end
    bVar7 = false
    pCVar12 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar12, bVar7)
    r1 = quest:GetHeroTargetedThing()
    bVar7 = quest:IsEntityPickPocketable(r1)
    if bVar7 then
        bVar7 = quest:IsInMovieSequence()
        if not bVar7 then
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:DisplayMiniGameInfo(true, 3)
            quest:EntitySetCutsceneBehaviour(r1, 1)
            quest:PauseAllNonScriptedEntities(true)
            fVar2 = quest:ReadGlobalGameDataFloat(0x1010)
            i_stk_3c = 0
            bVar9 = false
            bVar6 = false
            bVar7 = false
            xStack_2c = quest:GetHeroStatLevel(5)
            f_stk_30 = (xStack_2c + 1.0)
            xStack_2c = quest:GetHeroStatMax(5)
            fVar5 = f_stk_30 / (xStack_2c + 1.0)
            xStack_2c = quest:GetConstantFPS()
            value = xStack_2c * (fVar2 / fVar5)
            C_stk_40 = math.tointeger(math.modf(value))
            xStack_2c = C_stk_40
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                if bVar8 then
                    __cleanup_LAB_00eeb4fa()
                    return
                end
                bVar8 = quest:IsDPadButtonHeldForExpression("EXPRESSION_PICKPOCKET")
                if not bVar8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then __cleanup_LAB_00eeb5dd(); return end
                    if bVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar9 = not alive
                        if bVar9 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            return
                        end
                        if 0 < i_stk_3c then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar7 = not alive
                            if bVar7 then __cleanup_LAB_00eeb5dd(); return end
                            bVar7 = false
                        end
                    end
                    bVar9 = true
                end
                pCVar11 = quest:GetHero()
                bVar8 = (pCVar11 ~= nil and pCVar11:MsgIsHitBy(""))
                if bVar8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                        return
                    end
                    bVar9 = true
                    bVar6 = true
                end
                if bVar7 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then __cleanup_LAB_00eeb5dd(); return end
                    if i_stk_3c == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar9 = not alive
                        if bVar9 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            return
                        end
                        bVar9 = true
                    end
                end
                f_stk_38 = (1 - C_stk_40 / value)
                if 1.0 < f_stk_38 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then __cleanup_LAB_00eeb5dd(); return end
                    f_stk_38 = 1.0
                end
                quest:UpdateMiniGameInfoBar(f_stk_38)
                pCVar11 = quest:GetHeroTargetedThing()
                r1 = pCVar11
                pCVar11 = nil
                __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
                if not __native_condition_1 then
                    cVar10 = (r1 ~= nil and r1:IsAlive())
                    __native_condition_1 = not cVar10
                end
                if __native_condition_1 then
                    goto LAB_00eeb3f9
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                        return
                    end
                    if 1.0 <= f_stk_38 then goto LAB_00eeb3f9 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                        return
                    end
                    if C_stk_40 < xStack_2c then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar8 then __cleanup_LAB_00eeb5dd(); return end
                        iVar13 = math.random(0, 32767)
                        if iVar13 % 100 < quest:ReadGlobalGameData(0x1018) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar7 = not alive
                            if bVar7 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                            bVar7 = true
                            xStack_2c = quest:GetConstantFPS()
                            i_stk_3c = math.tointeger(math.modf(xStack_2c * quest:ReadGlobalGameDataFloat(0x1014)))
                            bVar8 = true
                            pCVar11 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(r1, pCVar11, bVar8)
                        end
                        xStack_2c = C_stk_40
                    end
                end
                goto FLOW_past_lab_00eeb3f9
                ::LAB_00eeb3f9::
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then __cleanup_LAB_00eeb5dd(); return end
                bVar9 = true
                ::FLOW_past_lab_00eeb3f9::
                if 0 < C_stk_40 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then
                        -- LAB_00eeb521: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                        return
                    end
                    C_stk_40 = (C_stk_40 + -1)
                end
                if 0 < i_stk_3c then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar8 then __cleanup_LAB_00eeb5dd(); return end
                    i_stk_3c = i_stk_3c + -1
                end
            until not (not bVar9)
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then __cleanup_LAB_00eeb4fa(); return end
            quest:PauseAllNonScriptedEntities(false)
            quest:EntitySetCutsceneBehaviour(r1, 0)
            quest:DisplayMiniGameInfo(false, 3)
            if not bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    __cleanup_LAB_00eeb5dd()
                    return
                end
                iVar13 = (r1 ~= nil and r1:IsAlive())
                if iVar13 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
                        -- LAB_00eeb583: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                        return
                    end
                    quest:EntitySetCutsceneBehaviour(r1, 0)
                    if bVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar7 = not alive
                        if bVar7 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            return
                        end
                        if not (r1 ~= nil and not r1:IsNull()) then
                            pExtraData = 0x0
                        else
                            -- TODO(native): GetPThing is not a ForgeFSE binding
                            pExtraData = r1:GetPThing()
                        end
                        pCVar11 = r1
                        pSender = quest:GetHero()
                        quest:SendEntityEvent(0x13, pSender, pCVar11)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar7 = not alive
                        if bVar7 then __cleanup_LAB_00eeb4fa(); return end
                        iVar13 = math.random(0, 32767)
                        i_stk_2c = (iVar13 % 100)
                        if i_stk_2c < f_stk_38 * 100.0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar7 = not alive
                            if bVar7 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                            quest:EntitySetAsPickPocketed(r1)
                        end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            uVar14 = 0
            pCVar12 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar12, uVar14)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then
            return
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then goto LAB_00eeafc7 end
    end
    uVar14 = 0
    pCVar12 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar12, uVar14)
    ::LAB_00eeafc7::
end

function Init(quest)
end

function OnPersist(quest, context)
end

