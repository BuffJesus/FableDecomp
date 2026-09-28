-- Generated native draft: Gate3Guard. Review coverage report before use.
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
    local bVar4, b_stk_71, b_stk_72, cVar5, dist, fVar2, fret_0, fret_00, iVar12, iVar13, iVar6, iVar7, i_stk_70, p0, pCVar8, pCVar9, pcVar11, r1, r2, uVar10, uVar3, v_stk_6c, xStack_10, xStack_20, xStack_30, x_stk_3c, x_stk_48
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_30 = resources:NewResource()
        resources:PrepareResource(xStack_30)
        bVar4 = resources:TryAcquire(xStack_30, me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d07d05 end
            bVar4 = resources:TryAcquire(xStack_30, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        b_stk_72 = not alive
        if not b_stk_72 then
            iVar6 = quest:RegisterTimer()
            i_stk_70 = iVar6
            quest:SetTimer(i_stk_70, 0)
            quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain")
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            b_stk_71 = b_stk_72
            repeat
                if bVar4 then
                    quest:DeregisterTimer(i_stk_70)
                    resources:ReleaseResource(xStack_30)
                    return
                end
                bVar4 = me:MsgIsHitByHero()
                if bVar4 then
                    goto LAB_00d076a9
                else
                    bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar4 then
                        bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar4 then goto LAB_00d076a9 end
                    end
                    bVar4 = false
                end
                goto FLOW_past_lab_00d076a9
                ::LAB_00d076a9::
                bVar4 = true
                ::FLOW_past_lab_00d076a9::
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        pCVar8 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar8)
                        resources:PrepareResource(xStack_30)
                        quest:ClearThingHasInformation(me)
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                        until not (not bVar4)
                        quest:DeregisterTimer(i_stk_70)
                        resources:ReleaseResource(xStack_30)
                        return
                    end
                    -- TODO(native): override_prt_d07c09_8df7e253:
                    quest:DeregisterTimer(i_stk_70)
                    resources:ReleaseResource(xStack_30)
                    return
                end
                if not quest:GetStateBool("Gate3Open") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    -- TODO(native): if (bVar4) goto override_prt_d07c09_8df7e253;
                    iVar7 = quest:GetTimer(i_stk_70)
                    if iVar7 < 1 then
                        dist = 8.0
                        pCVar8 = quest:GetHero()
                        bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, dist)
                        if not bVar4 then goto LAB_00d0783a end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            iVar7 = quest:AddNewConversation(me, false, false)
                            pCVar8 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar8)
                            if b_stk_72 == false then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    b_stk_72 = true
                                    pCVar8 = quest:GetHero()
                                    quest:AddLineToConversation(iVar7, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", me, pCVar8, false)
                                    goto LAB_00d07820
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    pCVar8 = quest:GetHero()
                                    quest:AddLineToConversation(iVar7, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", me, pCVar8, false)
                                    goto LAB_00d07820
                                end
                            end
                            goto FLOW_past_lab_00d07820
                            ::LAB_00d07820::
                            iVar6 = i_stk_70
                            quest:SetTimer(i_stk_70, 10)
                            goto LAB_00d0783a
                            ::FLOW_past_lab_00d07820::
                        end
                    else
                        goto LAB_00d0783a
                    end
                    goto FLOW_past_lab_00d0783a
                    ::LAB_00d0783a::
                    bVar4 = me:IsTalkedToByHero()
                    if not bVar4 then goto LAB_00d07bbe end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        if b_stk_71 == false then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                resources:PrepareResource(xStack_30)
                                bVar4 = resources:TryAcquire(xStack_30, me, 4)
                                while not bVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d07cfc end
                                    bVar4 = resources:TryAcquire(xStack_30, me, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    xStack_20 = resources:StartMovie("")
                                    quest:PauseAllNonScriptedEntities(true)
                                    x_stk_48 = resources:ScriptThing(xStack_30)
                                    pCVar8 = x_stk_48
                                    fret_0 = quest:GetHealth(pCVar8)
                                    fVar2 = 0.0
                                    if fVar2 < fret_0 then
                                        iVar13 = 0
                                        iVar12 = 1
                                        iVar7 = 0
                                        iVar6 = 0
                                        pcVar11 = "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT"
                                        pCVar8 = quest:GetHero()
                                        r1 = me:Speak(pCVar8, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar5 = iVar6
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto LAB_00d07caf
                                            end
                                            iVar6 = me:IsPerformingScriptTask()
                                            cVar5 = iVar6
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d07caf
                                        end
                                        goto FLOW_past_lab_00d07caf
                                        ::LAB_00d07caf::
                                        pCVar9 = xStack_20
                                        goto LAB_00d07cf7
                                    end
                                    goto FLOW_hoist_lab_00d07cf7_1
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                resources:PrepareResource(xStack_30)
                                bVar4 = resources:TryAcquire(xStack_30, me, 4)
                                while not bVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d07cfc end
                                    bVar4 = resources:TryAcquire(xStack_30, me, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    xStack_10 = resources:StartMovie("")
                                    quest:PauseAllNonScriptedEntities(true)
                                    x_stk_3c = resources:ScriptThing(xStack_30)
                                    pCVar8 = x_stk_3c
                                    fret_00 = quest:GetHealth(pCVar8)
                                    fVar2 = 0.0
                                    if fVar2 < fret_00 then
                                        iVar13 = 0
                                        iVar12 = 1
                                        iVar7 = 0
                                        iVar6 = 0
                                        pcVar11 = "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT"
                                        pCVar8 = quest:GetHero()
                                        r2 = me:Speak(pCVar8, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar5 = iVar6
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto LAB_00d07ce2
                                            end
                                            iVar6 = me:IsPerformingScriptTask()
                                            cVar5 = iVar6
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d07ce2
                                        end
                                        goto FLOW_past_lab_00d07ce2
                                        ::LAB_00d07ce2::
                                        pCVar9 = xStack_10
                                        goto LAB_00d07cf7
                                        ::FLOW_past_lab_00d07ce2::
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar9 = xStack_10
                                    goto LAB_00d07bb5
                                end
                            end
                        end
                        goto FLOW_past_lab_00d07cf7
                        ::LAB_00d07cf7::
                        goto LAB_00d07cfc
                        ::FLOW_past_lab_00d07caf::
                        ::FLOW_hoist_lab_00d07cf7_1::
                        b_stk_71 = true
                        quest:PauseAllNonScriptedEntities(false)
                        pCVar9 = xStack_20
                        ::LAB_00d07bb5::
                        iVar6 = i_stk_70
                        goto LAB_00d07bbe
                        ::FLOW_past_lab_00d07cf7::
                    end
                    ::FLOW_past_lab_00d0783a::
                    ::LAB_00d07cfc::
                    quest:DeregisterTimer(i_stk_70)
                    break
                end
                ::LAB_00d07bbe::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
            until false
        end
        ::LAB_00d07d05::
        resources:ReleaseResource(xStack_30)
    end
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

