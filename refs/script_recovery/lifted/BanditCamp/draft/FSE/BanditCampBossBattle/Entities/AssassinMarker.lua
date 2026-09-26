-- Generated native draft: AssassinMarker. Review coverage report before use.
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
    local bVar3, bVar5, cVar4, ctr_c8, dist, fVar2, fret_0, fret_00, fret_01, iVar11, iVar12, iVar13, iVar6, iVar9, i_stk_c4, native_arg_sequence_1, native_arg_switch_2, pCVar7, pScriptObject, pcVar10, r1, r2, r3, r4, r5, r6, xStack_10, xStack_20, xStack_30, xStack_58, xStack_64, xStack_70, xStack_84, xStack_94, xStack_a4, xStack_b4
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    r1 = quest:GetThingWithScriptName("Assassin1")
    r2 = quest:GetThingWithScriptName("Assassin2")
    r3 = quest:GetThingWithScriptName("Assassin3")
    quest:EntitySetAsKillable(r1, false, true)
    quest:EntitySetAsKillable(r2, false, true)
    quest:EntitySetAsKillable(r3, false, true)
    xStack_a4 = resources:NewResource()
    xStack_84 = resources:NewResource()
    xStack_94 = resources:NewResource()
    bVar3 = false
    iVar9 = 0
    ctr_c8 = 0
    iVar6 = quest:RegisterTimer()
    i_stk_c4 = iVar6
    quest:SetTimer(i_stk_c4, 0)
    cVar4 = quest:GetStateBool("AssassinCutsceneTriggered")
    while (not cVar4 and (not bVar3)) do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d1248e: (native jump target)
            quest:DeregisterTimer(i_stk_c4)
            goto LAB_00d128bf
        end
        if not quest:GetStateBool("AssassinsUnderAttack") then
            dist = 4.0
            pCVar7 = quest:GetHero()
            bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, dist)
            bVar3 = false
            native_arg_sequence_1 = false
            if not bVar5 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                bVar5 = quest:IsConversationActive(iVar9)
                if bVar5 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if not native_arg_sequence_1 then
                if quest:GetStateBool("TalkedToAssassin") then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d12226 end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d12245 end
            native_arg_switch_2 = ctr_c8
            repeat
                if native_arg_switch_2 == 0 then
                    iVar9 = quest:AddNewConversation(r1, false, false)
                    quest:AddPersonToConversation(iVar9, r2)
                    quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN1_LISTENED_ONE", r1, r2, false)
                    break
                else
                    if native_arg_switch_2 == 1 then
                        iVar9 = quest:AddNewConversation(r2, false, false)
                        quest:AddPersonToConversation(iVar9, r1)
                        quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN2_LISTENED_ONE", r2, r1, false)
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            iVar9 = quest:AddNewConversation(r1, false, false)
                            quest:AddPersonToConversation(iVar9, r2)
                            quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN1_LISTENED_TWO", r1, r2, false)
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                iVar9 = quest:AddNewConversation(r2, false, false)
                                quest:AddPersonToConversation(iVar9, r1)
                                quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN2_LISTENED_TWO", r2, r1, false)
                                break
                            else
                                if native_arg_switch_2 == 4 then
                                    iVar9 = quest:AddNewConversation(r1, false, false)
                                    pCVar7 = quest:GetHero()
                                    quest:AddPersonToConversation(iVar9, pCVar7)
                                    pCVar7 = quest:GetHero()
                                    quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN1_LISTENED_THREE", r1, pCVar7, false)
                                    break
                                else
                                    if native_arg_switch_2 == 5 then
                                        iVar9 = quest:AddNewConversation(r1, false, false)
                                        pCVar7 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar9, pCVar7)
                                        pCVar7 = quest:GetHero()
                                        quest:AddLineToConversation(iVar9, "TEXT_QST_009_ASSASSIN1_LISTENED_FOUR", r1, pCVar7, false)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                end
                            end
                        end
                    end
                end
            until not (false)
            ::FLOW_native_label_1::
            ctr_c8 = ctr_c8 + 1
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d12245 end
            resources:TryAcquire(xStack_a4, r1, 4)
            resources:TryAcquire(xStack_84, r2, 4)
            resources:TryAcquire(xStack_94, r3, 4)
            xStack_b4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            xStack_58 = resources:ScriptThing(xStack_a4)
            pCVar7 = xStack_58
            fret_0 = quest:GetHealth(pCVar7)
            fVar2 = 0.0
            xStack_58 = nil
            if fVar2 < fret_0 then
                iVar13 = 0
                iVar12 = 1
                iVar11 = 0
                iVar6 = 0
                pcVar10 = "TEXT_QST_009_ASSASSIN1_ATTACKED"
                pCVar7 = quest:GetHero()
                r4 = me:Speak(pCVar7, pcVar10, iVar6, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b4)
                        quest:DeregisterTimer(i_stk_c4)
                        resources:ReleaseResource(xStack_94)
                        resources:ReleaseResource(xStack_84)
                        resources:ReleaseResource(xStack_a4)
                        r3 = nil
                        r2 = nil
                        r1 = nil
                        return
                    end
                    iVar6 = me:IsPerformingScriptTask()
                    cVar4 = iVar6
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then goto LAB_00d11c9b end
                goto LAB_00d1244c
            end
            goto FLOW_past_lab_00d1244c
            ::LAB_00d1244c::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_b4)
            quest:DeregisterTimer(i_stk_c4)
            goto LAB_00d128bf
            ::FLOW_past_lab_00d1244c::
            ::LAB_00d11c9b::
            xStack_64 = resources:ScriptThing(xStack_84)
            pCVar7 = xStack_64
            fret_00 = quest:GetHealth(pCVar7)
            fVar2 = 0.0
            xStack_64 = nil
            if fVar2 < fret_00 then
                iVar13 = 0
                iVar12 = 1
                iVar11 = 0
                iVar6 = 0
                pcVar10 = "TEXT_QST_009_ASSASSIN2_ATTACKED"
                pCVar7 = quest:GetHero()
                r5 = me:Speak(pCVar7, pcVar10, iVar6, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00d12259: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b4)
                        quest:DeregisterTimer(i_stk_c4)
                        resources:ReleaseResource(xStack_94)
                        resources:ReleaseResource(xStack_84)
                        resources:ReleaseResource(xStack_a4)
                        r3 = nil
                        r2 = nil
                        r1 = nil
                        return
                    end
                    iVar6 = me:IsPerformingScriptTask()
                    cVar4 = iVar6
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d1244c end
            end
            xStack_70 = resources:ScriptThing(xStack_94)
            pCVar7 = xStack_70
            fret_01 = quest:GetHealth(pCVar7)
            fVar2 = 0.0
            xStack_70 = nil
            if fVar2 < fret_01 then
                iVar13 = 0
                iVar12 = 1
                iVar11 = 0
                iVar6 = 0
                pcVar10 = "TEXT_QST_009_ASSASSIN3_ATTACKED"
                pCVar7 = quest:GetHero()
                r6 = me:Speak(pCVar7, pcVar10, iVar6, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b4)
                        quest:DeregisterTimer(i_stk_c4)
                        goto LAB_00d128bf
                    end
                    iVar6 = me:IsPerformingScriptTask()
                    cVar4 = iVar6
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d1244c end
            end
            pCVar7 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(r1, pCVar7)
            pCVar7 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(r2, pCVar7)
            pCVar7 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(r3, pCVar7)
            bVar3 = true
            resources:PrepareResource(xStack_a4)
            resources:PrepareResource(xStack_84)
            resources:PrepareResource(xStack_94)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_b4)
        end
        ::LAB_00d12226::
        cVar4 = quest:GetStateBool("AssassinCutsceneTriggered")
        iVar6 = i_stk_c4
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        goto LAB_00d12245
    end
    goto FLOW_past_lab_00d12245
    ::LAB_00d12245::
    quest:DeregisterTimer(i_stk_c4)
    goto LAB_00d128bf
    ::FLOW_past_lab_00d12245::
    quest:EntitySetAsKillable(r1, true, true)
    quest:EntitySetAsKillable(r2, true, true)
    quest:EntitySetAsKillable(r3, true, true)
    if not quest:GetStateBool("AssassinCutsceneTriggered") then
        goto LAB_00d128a3
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_10 = resources:NewResource()
            xStack_20 = resources:NewResource()
            xStack_30 = resources:NewResource()
            xStack_58 = resources:NewResource()
            iVar6 = 4
            pScriptObject = xStack_10
            pCVar7 = quest:GetHero()
            resources:TryAcquire(pScriptObject, pCVar7, iVar6)
            resources:TryAcquire(xStack_20, r1, 4)
            resources:TryAcquire(xStack_30, r2, 4)
            resources:TryAcquire(xStack_58, r3, 4)
            quest:EntitySetInFaction(r1, "FACTION_TWINBLADE_CAMP_BANDITS")
            quest:EntitySetInFaction(r2, "FACTION_TWINBLADE_CAMP_BANDITS")
            quest:EntitySetInFaction(r3, "FACTION_TWINBLADE_CAMP_BANDITS")
            xStack_70 = resources:NewActorMap()
            resources:SetActor(xStack_70, "HERO", xStack_10)
            resources:SetActor(xStack_70, "ASS1", xStack_20)
            resources:SetActor(xStack_70, "ASS2", xStack_30)
            resources:SetActor(xStack_70, "ASS3", xStack_58)
            xStack_b4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_BANDITCAMP_ASSATTACK", xStack_70, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("Gate3Open", true)
            bVar5 = true
            bVar3 = false
            pCVar7 = quest:GetThingWithScriptName("Gate3Guard")
            quest:RemoveThing(pCVar7, bVar3, bVar5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_b4)
            resources:DestroyActorMap(xStack_70)
            resources:ReleaseResource(xStack_58)
            resources:ReleaseResource(xStack_30)
            resources:ReleaseResource(xStack_20)
            resources:ReleaseResource(xStack_10)
            goto LAB_00d128a3
        end
    end
    goto FLOW_past_lab_00d128a3
    ::LAB_00d128a3::
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until not (not bVar3)
    ::FLOW_past_lab_00d128a3::
    quest:DeregisterTimer(i_stk_c4)
    ::LAB_00d128bf::
    resources:ReleaseResource(xStack_94)
    resources:ReleaseResource(xStack_84)
    resources:ReleaseResource(xStack_a4)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

