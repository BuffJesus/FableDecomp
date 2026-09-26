-- Generated native draft: BanditKing. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local CVar8, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, bVar2, bVar3, bVar4, bVar5, bVar6, cVar7, fVar1, fret_0, fret_00, fret_v0, fret_v00, iVar10, iVar9, i_stk_74, max, native_arg_switch_1, p0, p1, p4, p5, pCVar11, pThing, r1, scale, xStack_10, xStack_20, xStack_60, xStack_6c, xStack_70, x_stk_2c
    local alive = true
    bVar6 = false
    bVar2 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_07", "", "BanditCampMain")
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
        quest:EntitySetAbleToBeEngagedInCombat(me, false)
        fret_v0 = quest:GetHealth(me)
        CVar8 = math.tointeger(math.modf(fret_v0))
        bVar3 = false
        bVar5 = false
        xStack_60 = CVar8
        iVar9 = quest:RegisterTimer()
        i_stk_74 = iVar9
        quest:SetTimer(i_stk_74, 0)
        cVar7 = quest:GetStateBool("ItsAllOver")
        while not cVar7 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d0ad0a: (native jump target)
                quest:DeregisterTimer(i_stk_74)
                return
            end
            fret_v00 = quest:GetHealth(me)
            iVar10 = math.tointeger(math.modf(fret_v00))
            quest:SetStateInt("KingHealth", iVar10)
            __native_condition_2 = quest:GetStateInt("KingHealth") < math.tointeger(math.modf((CVar8 * 3) / 4))
            if __native_condition_2 then
                __native_condition_2 = not bVar3
            end
            __native_condition_1 = __native_condition_2
            if __native_condition_1 then
                iVar10 = quest:GetTimer(i_stk_74)
                __native_condition_1 = iVar10 < 1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_74)
                    return
                end
                iVar10 = quest:AddNewConversation(me, false, false)
                pCVar11 = quest:GetHero()
                quest:AddPersonToConversation(iVar10, pCVar11)
                pCVar11 = quest:GetHero()
                quest:AddLineToConversation(iVar10, "TEXT_QST_009_TWINBLADE_TAUNT_10", me, pCVar11, false)
                bVar3 = true
                quest:SetTimer(i_stk_74, 8)
                iVar9 = i_stk_74
                CVar8 = xStack_60
            end
            __native_condition_4 = quest:GetStateInt("KingHealth") < math.tointeger(math.modf(CVar8 / 2))
            if __native_condition_4 then
                __native_condition_4 = not bVar5
            end
            __native_condition_3 = __native_condition_4
            if __native_condition_3 then
                iVar10 = quest:GetTimer(i_stk_74)
                __native_condition_3 = iVar10 < 1
            end
            if __native_condition_3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:DeregisterTimer(i_stk_74)
                    return
                end
                iVar10 = quest:AddNewConversation(me, false, false)
                pCVar11 = quest:GetHero()
                quest:AddPersonToConversation(iVar10, pCVar11)
                pCVar11 = quest:GetHero()
                quest:AddLineToConversation(iVar10, "TEXT_QST_009_TWINBLADE_TAUNT_20", me, pCVar11, false)
                bVar5 = true
                quest:SetTimer(i_stk_74, 8)
                iVar9 = i_stk_74
                CVar8 = xStack_60
            end
            cVar7 = quest:GetStateBool("ItsAllOver")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d0ad21: (native jump target)
            quest:DeregisterTimer(i_stk_74)
            return
        end
        CVar8 = 0x0
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        bVar3 = resources:TryAcquire(xStack_20, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d0b2a0 end
            bVar3 = resources:TryAcquire(xStack_20, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_70 = quest:RegisterTimer()
            quest:SetTimer(xStack_70, 0)
            bVar3 = false
            pCVar11 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, pCVar11, bVar3)
            bVar3 = false
            pThing = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(pThing, me, bVar3)
            me:PlayLoopingAnimation("DEFEATED_POSE", -1, false, false, false)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            repeat
                if bVar3 then
                    quest:DeregisterTimer(xStack_70)
                    resources:ReleaseResource(xStack_20)
                    quest:DeregisterTimer(i_stk_74)
                    return
                end
                iVar9 = quest:GetTimer(xStack_70)
                if 0 < iVar9 then goto LAB_00d0ae81 end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d0b297 end
                iVar9 = quest:AddNewConversation(me, false, false)
                pCVar11 = quest:GetHero()
                quest:AddPersonToConversation(iVar9, pCVar11)
                native_arg_switch_1 = CVar8
                repeat
                    if native_arg_switch_1 == 0 then
                        pCVar11 = quest:GetHero()
                        quest:AddLineToConversation(iVar9, "TEXT_QST_009_TWINBLADE_BEGGING_10", me, pCVar11, false)
                        CVar8 = 0x1
                        xStack_6c = CVar8
                        break
                    else
                        if native_arg_switch_1 == 1 then
                            pCVar11 = quest:GetHero()
                            quest:AddLineToConversation(iVar9, "TEXT_QST_009_TWINBLADE_BEGGING_20", me, pCVar11, false)
                            CVar8 = 0x2
                            xStack_6c = CVar8
                            break
                        else
                            if native_arg_switch_1 == 2 then
                                pCVar11 = quest:GetHero()
                                quest:AddLineToConversation(iVar9, "TEXT_QST_009_TWINBLADE_BEGGING_30", me, pCVar11, false)
                                goto LAB_00d0ae5e
                            else
                                if native_arg_switch_1 == 3 then
                                    pCVar11 = quest:GetHero()
                                    quest:AddLineToConversation(iVar9, "TEXT_QST_009_TWINBLADE_BEGGING_40", me, pCVar11, false)
                                    CVar8 = 0x4
                                    xStack_6c = CVar8
                                    break
                                else
                                    if native_arg_switch_1 == 4 then
                                        pCVar11 = quest:GetHero()
                                        quest:AddLineToConversation(iVar9, "TEXT_QST_009_TWINBLADE_BEGGING_50", me, pCVar11, false)
                                        goto LAB_00d0ae5e
                                    end
                                end
                            end
                            goto FLOW_past_lab_00d0ae5e
                            ::LAB_00d0ae5e::
                            CVar8 = 0x3
                            xStack_6c = CVar8
                            ::FLOW_past_lab_00d0ae5e::
                        end
                    end
                until not (false)
                quest:SetTimer(xStack_70, 10)
                ::LAB_00d0ae81::
                bVar3 = me:MsgIsHitByHero()
                if bVar3 then
                    goto LAB_00d0af0f
                else
                    bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar6 then
                        bVar6 = true
                        bVar2 = true
                        bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar3 then goto LAB_00d0af0f end
                    end
                    bVar6 = true
                    bVar3 = false
                end
                goto FLOW_past_lab_00d0af0f
                ::LAB_00d0af0f::
                bVar3 = true
                ::FLOW_past_lab_00d0af0f::
                if bVar2 then
                    bVar2 = false
                end
                if bVar6 then
                    bVar6 = false
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        goto LAB_00d0b297
                    end
                    goto FLOW_hoist_lab_00d0b297_1
                end
                goto FLOW_past_lab_00d0b297
                ::LAB_00d0b297::
                quest:DeregisterTimer(xStack_70)
                break
                ::FLOW_hoist_lab_00d0b297_1::
                xStack_10 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                me:ClearAllActionsIncludingLoopingAnimations()
                x_stk_2c = resources:ScriptThing(xStack_20)
                pCVar11 = x_stk_2c
                fret_0 = quest:GetHealth(pCVar11)
                fVar1 = 0.0
                if fVar1 < fret_0 then
                    p5 = 0
                    p4 = 1
                    iVar10 = 0
                    iVar9 = 0
                    p1 = "TEXT_QST_009_TWINBLADE_OVER"
                    pCVar11 = quest:GetHero()
                    r1 = me:Speak(pCVar11, p1, iVar9, (iVar10 ~= 0), (p4 ~= 0), (p5 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar7 = iVar9
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0b27a
                        end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar7 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00d0b27a
                    end
                    goto FLOW_past_lab_00d0b27a
                    ::LAB_00d0b27a::
                    resources:DestroyMovie(xStack_10)
                    goto LAB_00d0b297
                    ::FLOW_past_lab_00d0b27a::
                end
                quest:SetStateBool("TwinBladeAttacked", true)
                quest:SetStateInt("AngryBanditNeeded", 4)
                resources:PrepareResource(xStack_20)
                pCVar11 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, pCVar11)
                quest:EntitySetAsKillable(me, true, true)
                quest:EntitySetAbleToBeEngagedInCombat(me, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
                quest:DisplayQuestInfo(true)
                iVar9 = quest:AddQuestInfoBar(xStack_60, 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
                __native_entity_state:SetStateInt("HealthBarIndex", iVar9)
                bVar3 = me:IsAlive()
                CVar8 = xStack_6c
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d0b297 end
                        scale = -1.0
                        max = -1.0
                        fret_00 = quest:GetHealth(me)
                        quest:UpdateQuestInfoBar(__native_entity_state:GetStateInt("HealthBarIndex"), fret_00, max, scale)
                        bVar3 = me:IsAlive()
                        CVar8 = xStack_6c
                    until not (bVar3)
                end
                ::FLOW_past_lab_00d0b297::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            until false
        end
        ::LAB_00d0b2a0::
        resources:ReleaseResource(xStack_20)
        quest:DeregisterTimer(i_stk_74)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetStateBool("TwinBladeKilled", true)
        quest:SetMasterGameState("BanditCampTwinbladeKilled", true)
    end
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("HealthBarIndex"))
end

