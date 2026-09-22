-- Generated native draft: PreMeleeWhisper. Review coverage report before use.
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
    local bVar3, cVar4, dist, elem_1, fVar2, f_stk_28, f_stk_94, fret_0, iStack_90, iVar10, iVar11, native_arg_sequence_1, native_arg_switch_2, p1, p4, p5, pCVar5, pCVar6, pThing, puVar8, pvVar7, r1, r2, timerId, uVar9, xStack_1c, xStack_a0, x_stk_c
    local alive = true
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(true)
    cVar4 = quest:GetStateBool("WhisperCutsceneFinished")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar4 = quest:GetStateBool("WhisperCutsceneFinished")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_a0 = resources:NewResource()
        resources:PrepareResource(xStack_a0)
        bVar3 = resources:TryAcquire(xStack_a0, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_a0)
                return
            end
            bVar3 = resources:TryAcquire(xStack_a0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d52582: (native jump target)
            resources:ReleaseResource(xStack_a0)
            return
        end
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        iVar11 = 1
        iVar10 = 1.0
        pCVar5 = quest:GetHero()
        me:FollowThing(pCVar5, iVar10, (iVar11 ~= 0))
        cVar4 = quest:GetStateBool("WhisperStopFollowing")
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(xStack_a0)
                return
            end
            cVar4 = me:IsTalkedToByHero()
            if cVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d52e1b end
                xStack_1c = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                me:ClearCommands()
                x_stk_c = resources:ScriptThing(xStack_a0)
                pCVar5 = x_stk_c
                fret_0 = quest:GetHealth(pCVar5)
                fVar2 = 0.0
                if fVar2 < fret_0 then
                    p5 = 0
                    p4 = 1
                    iVar11 = 0
                    iVar10 = 0
                    p1 = "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT"
                    pCVar5 = quest:GetHero()
                    r1 = me:Speak(pCVar5, p1, iVar10, (iVar11 ~= 0), (p4 ~= 0), (p5 ~= 0))
                    iVar10 = me:IsPerformingScriptTask()
                    cVar4 = iVar10
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1c)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(xStack_a0)
                            return
                        end
                        iVar10 = me:IsPerformingScriptTask()
                        cVar4 = iVar10
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        -- LAB_00d52dc0: (native jump target)
                        resources:DestroyMovie(xStack_1c)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(xStack_a0)
                        return
                    end
                end
                iVar11 = 1
                iVar10 = 1.0
                pCVar5 = quest:GetHero()
                me:FollowThing(pCVar5, iVar10, (iVar11 ~= 0))
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1c)
            end
            r2 = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
            pCVar5 = quest:GetHero()
            if not (r2 ~= nil and not r2:IsNull()) then
                puVar8 = {x = 0, y = 0, z = 0}
            else
                puVar8 = r2:GetPos()
            end
            pCVar6 = pCVar5:GetPos()
            f_stk_94 = puVar8.z - pCVar6.z
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, r2, 7.0)
            if bVar3 then
                dist = 7.0
                pCVar5 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, dist)
                native_arg_sequence_1 = false
                if not bVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    iVar10 = quest:GetTimer(timerId)
                    if 5 < iVar10 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if not native_arg_sequence_1 then
                    if 1.0 < math.abs(f_stk_94) then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00d52d56 end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00d52df2: (native jump target)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(xStack_a0)
                    return
                end
                pvVar7 = r2:GetDataString()
                f_stk_94 = tonumber(pvVar7)
                uVar9 = 0
                iStack_90 = quest:GetAllThingsWithScriptName("PreMeleeChatMarker")
                if #iStack_90 ~= 0 then
                    iVar10 = 0
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(xStack_a0)
                            return
                        end
                        elem_1 = iStack_90[(iVar10) / 0xc + 1]
                        pvVar7 = elem_1:GetDataString()
                        f_stk_28 = tonumber(pvVar7)
                        if f_stk_28 == f_stk_94 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(xStack_a0)
                                return
                            end
                            quest:RemoveThing(iStack_90[(iVar10) / 0xc + 1], false, true)
                        end
                        uVar9 = uVar9 + 1
                        iVar10 = iVar10 + 0xc
                    until not (uVar9 < (#iStack_90))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00d52de9: (native jump target)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(xStack_a0)
                    return
                end
                iVar11 = quest:AddNewConversation(me, false, false)
                pCVar5 = quest:GetHero()
                quest:AddPersonToConversation(iVar11, pCVar5)
                quest:SetTimer(timerId, 10)
                native_arg_switch_2 = f_stk_94
                repeat
                    if native_arg_switch_2 == 2 then
                        pCVar5 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, pCVar5, false)
                        break
                    else
                        if native_arg_switch_2 == 4 then
                            pCVar5 = quest:GetHero()
                            quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, pCVar5, false)
                            break
                        else
                            if native_arg_switch_2 == 5 then
                                pCVar5 = quest:GetHero()
                                quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, pCVar5, false)
                                break
                            else
                                if native_arg_switch_2 == 6 then
                                    pCVar5 = quest:GetHero()
                                    quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, pCVar5, false)
                                    break
                                else
                                    if native_arg_switch_2 == 7 then
                                        pCVar5 = quest:GetHero()
                                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, pCVar5, false)
                                        break
                                    else
                                        if native_arg_switch_2 == 8 then
                                            pCVar5 = quest:GetHero()
                                            quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, pCVar5, false)
                                            break
                                        else
                                            if native_arg_switch_2 == 9 then
                                                pCVar5 = quest:GetHero()
                                                quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, pCVar5, false)
                                                break
                                            else
                                                if native_arg_switch_2 == 10 then
                                                    pCVar5 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, pCVar5, false)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 11 then
                                                        pCVar5 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, pCVar5, false)
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 13 then
                                                            pCVar5 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, pCVar5, false)
                                                            break
                                                        else
                                                            if native_arg_switch_2 == 14 then
                                                                pCVar5 = quest:GetHero()
                                                                quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, pCVar5, false)
                                                                break
                                                            else
                                                                if native_arg_switch_2 == 15 then
                                                                    pCVar5 = quest:GetHero()
                                                                    quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, pCVar5, false)
                                                                    break
                                                                else
                                                                    if native_arg_switch_2 == 16 then
                                                                        pCVar5 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, pCVar5, false)
                                                                        break
                                                                    else
                                                                        if native_arg_switch_2 == 19 then
                                                                            pCVar5 = quest:GetHero()
                                                                            quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, pCVar5, false)
                                                                            break
                                                                        else
                                                                            if native_arg_switch_2 == 20 then
                                                                                pCVar5 = quest:GetHero()
                                                                                quest:AddLineToConversation(iVar11, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, pCVar5, false)
                                                                            else
                                                                                goto FLOW_native_label_1
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                ::FLOW_native_label_1::
            end
            ::LAB_00d52d56::
            cVar4 = quest:GetStateBool("WhisperStopFollowing")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d52d83: (native jump target)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(xStack_a0)
            return
        end
        ::LAB_00d52e1b::
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(xStack_a0)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

