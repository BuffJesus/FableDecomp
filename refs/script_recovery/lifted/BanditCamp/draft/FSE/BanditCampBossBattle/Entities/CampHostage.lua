-- Generated native draft: CampHostage. Review coverage report before use.
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
    local bVar3, cVar4, dist, fVar2, fret_0, fret_00, iVar10, iVar5, iVar6, iVar9, i_stk_70, native_arg_switch_1, p0, pCVar7, pcVar8, r1, r2, u_stk_6c, xStack_10, xStack_20, xStack_30, x_stk_3c, x_stk_48
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_30 = resources:NewResource()
        resources:PrepareResource(xStack_30)
        bVar3 = resources:TryAcquire(xStack_30, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d08517 end
            bVar3 = resources:TryAcquire(xStack_30, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            iVar5 = quest:RegisterTimer()
            i_stk_70 = iVar5
            quest:SetTimer(i_stk_70, 0)
            cVar4 = quest:GetStateBool("HostagesRescued")
            u_stk_6c = 0
            while (not cVar4 and (not quest:GetStateBool("HostageKilled"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d0850e end
                iVar6 = quest:GetTimer(i_stk_70)
                if 0 < iVar6 then goto LAB_00d08172 end
                dist = 8.0
                pCVar7 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, dist)
                if not bVar3 then goto LAB_00d08172 end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d0850e end
                bVar3 = false
                pCVar7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar3)
                iVar5 = quest:AddNewConversation(me, false, false)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar5, pCVar7)
                me:PlayAnimation("CS_SHOUT_FOR_HELP", false, false, false, true, true, false, false)
                native_arg_switch_1 = u_stk_6c
                repeat
                    if native_arg_switch_1 == 0 then
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar5, "TEXT_QST_009_HOSTAGE_SHOUT_FIRST", me, pCVar7, false)
                        u_stk_6c = 1
                        break
                    else
                        if native_arg_switch_1 == 1 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar5, "TEXT_QST_009_HOSTAGE_SHOUT_SECOND", me, pCVar7, false)
                            goto LAB_00d08150
                        else
                            if native_arg_switch_1 == 2 then
                                pCVar7 = quest:GetHero()
                                quest:AddLineToConversation(iVar5, "TEXT_QST_009_HOSTAGE_SHOUT_THIRD", me, pCVar7, false)
                                u_stk_6c = 3
                                break
                            else
                                if native_arg_switch_1 == 3 then
                                    pCVar7 = quest:GetHero()
                                    quest:AddLineToConversation(iVar5, "TEXT_QST_009_HOSTAGE_SHOUT_FOURTH", me, pCVar7, false)
                                    goto LAB_00d08150
                                end
                            end
                        end
                        goto FLOW_past_lab_00d08150
                        ::LAB_00d08150::
                        u_stk_6c = 2
                        ::FLOW_past_lab_00d08150::
                    end
                until not (false)
                iVar5 = i_stk_70
                quest:SetTimer(i_stk_70, 10)
                ::LAB_00d08172::
                bVar3 = me:IsTalkedToByHero()
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0850e end
                    resources:PrepareResource(xStack_30)
                    bVar3 = resources:TryAcquire(xStack_30, me, 4)
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:DeregisterTimer(i_stk_70)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                        bVar3 = resources:TryAcquire(xStack_30, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0850e end
                    xStack_20 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_48 = resources:ScriptThing(xStack_30)
                    pCVar7 = x_stk_48
                    fret_0 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_0 then
                        iVar10 = 0
                        iVar9 = 1
                        iVar5 = 0
                        iVar6 = 0
                        pcVar8 = "TEXT_QST_009_HOSTAGE_HELP"
                        pCVar7 = quest:GetHero()
                        r1 = me:Speak(pCVar7, pcVar8, iVar6, (iVar5 ~= 0), (iVar9 ~= 0), (iVar10 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar4 = iVar6
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                quest:DeregisterTimer(i_stk_70)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20)
                            goto LAB_00d0850e
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                    iVar5 = i_stk_70
                end
                if quest:GetStateBool("PlayGuardTooCloseCutscene") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0850e end
                    resources:PrepareResource(xStack_30)
                    bVar3 = resources:TryAcquire(xStack_30, me, 4)
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:DeregisterTimer(i_stk_70)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                        bVar3 = resources:TryAcquire(xStack_30, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0850e end
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_3c = resources:ScriptThing(xStack_30)
                    pCVar7 = x_stk_3c
                    fret_00 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_00 then
                        iVar10 = 0
                        iVar9 = 1
                        iVar5 = 0
                        iVar6 = 0
                        pcVar8 = "TEXT_QST_009_HOSTAGE_CLOSE"
                        pCVar7 = quest:GetHero()
                        r2 = me:Speak(pCVar7, pcVar8, iVar6, (iVar5 ~= 0), (iVar9 ~= 0), (iVar10 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar4 = iVar6
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                quest:DeregisterTimer(i_stk_70)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            goto LAB_00d0850e
                        end
                    end
                    quest:SetStateBool("PlayGuardTooCloseCutscene", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                cVar4 = quest:GetStateBool("HostagesRescued")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                resources:PrepareResource(xStack_30)
            end
            ::LAB_00d0850e::
            quest:DeregisterTimer(i_stk_70)
        end
        ::LAB_00d08517::
        resources:ReleaseResource(xStack_30)
    end
end

function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_NEUTRALS")
    quest:EntitySetOpinionReactionEnabled(me, 0x22, false)
    quest:SetThingHasInformation(me, false, false, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

