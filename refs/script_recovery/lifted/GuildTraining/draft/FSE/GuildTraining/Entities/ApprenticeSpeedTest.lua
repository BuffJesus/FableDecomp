-- Generated native draft: ApprenticeSpeedTest. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, bVar3, cVar4, c_stk_249, fVar15, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, iVar14, iVar16, iVar5, iVar7, i_stk_210, native_arg_switch_5, native_arg_switch_6, native_arg_switch_7, native_arg_switch_8, p0, pCVar11, pCVar6, pcVar13, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, uVar8, uVar9, u_stk_21c, xStack_100, xStack_1fc, xStack_20c, xStack_214, xStack_23c, xStack_250, xStack_254, xStack_258, xStack_c4, xStack_d4, xStack_e0, xStack_f0, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_54, x_stk_6c, x_stk_78, x_stk_90, x_stk_9c, x_stk_b4
    local alive = true
    local function __region_LAB_00d40379_c32()
        pCVar6 = quest:GetThingWithScriptName("RaceMarker")
        quest:MiniMapRemoveMarker(pCVar6)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_f0)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_23c = resources:NewResource()
        bVar3 = false
        if bVar3 ~= 0 then
        end
        bVar3 = resources:TryAcquire(xStack_23c, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d4075b end
            bVar3 = resources:TryAcquire(xStack_23c, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:SetIsPushableByHero(me, false)
            r1 = quest:GetThingWithScriptName("SpeedFriend")
            r2 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EntityAttachToVillage(me, r2)
            quest:EntityAttachToVillage(r1, r2)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsKillable(r1, false, true)
            quest:SetThingHasInformation(me, false, true, false)
            me:SetFriendsWithEverythingFlag(me)
            if (r1 ~= nil and not r1:IsNull()) then
                r1:SetFriendsWithEverythingFlag(1)
            end
            __native_entity_state:SetStateInt("RaceMode", 0)
            u_stk_21c = 0
            xStack_254 = quest:RegisterTimer()
            iVar5 = quest:RegisterTimer()
            xStack_258 = iVar5
            xStack_250 = quest:RegisterTimer()
            quest:SetTimer(iVar5, 1)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            while not bVar3 do
                if __native_entity_state:GetStateInt("RaceMode") == 0 then
                    i_stk_210 = u_stk_21c - 1
                    repeat
                        iVar5 = i_stk_210
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d405fc end
                        if (quest:GetStateInt("GameState") == 3) and (0 == 0) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            quest:SetThingHasInformation(me, false, true, false)
                        end
                        fVar15 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar15)
                        __native_condition_1 = bVar3
                        if __native_condition_1 then
                            iVar7 = quest:GetTimer(xStack_258)
                            __native_condition_1 = iVar7 < 1
                        end
                        if __native_condition_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            iVar7 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar7, r1)
                            if iVar5 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                native_arg_switch_5 = iVar5
                                repeat
                                    if native_arg_switch_5 == 0 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, r1, false)
                                        break
                                    else
                                        if native_arg_switch_5 == 1 then
                                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", r1, me, false)
                                            break
                                        else
                                            if native_arg_switch_5 == 2 then
                                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, r1, false)
                                                break
                                            else
                                                if native_arg_switch_5 == 3 then
                                                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", r1, me, false)
                                                    break
                                                else
                                                    if native_arg_switch_5 == 4 then
                                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, r1, false)
                                                        break
                                                    else
                                                        goto FLOW_native_label_1
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar9 = u_stk_21c & 0x80000001
                                bVar3 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar3 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, r1, false)
                                        goto LAB_00d3e7f7
                                    end
                                    goto LAB_00d405fc
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", r1, me, false)
                            end
                            ::LAB_00d3e7f7::
                            ::FLOW_native_label_1::
                            quest:SetTimer(xStack_258, 3)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar5 + 1
                        end
                        bVar3 = me:IsTalkedToByHero()
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            xStack_20c = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_e0 = resources:ScriptThing(xStack_23c)
                            pCVar6 = xStack_e0
                            fret_0 = quest:GetHealth(pCVar6)
                            c_stk_249 = 0.0 < fret_0
                            xStack_e0 = nil
                            if c_stk_249 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar7 = 0
                                iVar5 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_BOAST"
                                pCVar6 = quest:GetHero()
                                r3 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_20c)
                                        quest:DeregisterTimer(xStack_250)
                                        quest:DeregisterTimer(xStack_258)
                                        quest:DeregisterTimer(xStack_254)
                                        goto LAB_00d40749
                                    end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_20c
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20c)
                                    quest:DeregisterTimer(xStack_250)
                                    quest:DeregisterTimer(xStack_258)
                                    quest:DeregisterTimer(xStack_254)
                                    r2 = nil
                                    r1 = nil
                                    resources:ReleaseResource(xStack_23c)
                                    return
                                end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_20c
                                resources:DestroyMovie(pCVar11)
                                goto LAB_00d405fc
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar5 == 1 then
                                if bVar3 then
                                    -- LAB_00d3ee93: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_20c
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                                x_stk_9c = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_9c
                                fret_00 = quest:GetHealth(pCVar6)
                                c_stk_249 = 0.0 < fret_00
                                if c_stk_249 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar6 = quest:GetHero()
                                    r4 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d405d6 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_20c
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_VIGNETTE")
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xedc)))
                                quest:SetTimer(xStack_254, iVar7)
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xedc) + 20.0))
                                quest:SetTimer(xStack_250, iVar7)
                                xStack_214 = quest:AddQuestInfoTimer(xStack_250, "HUD_CLOCK_ICON", 1.0)
                                quest:DisplayQuestInfo(true)
                                if 1 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_20c
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                    quest:SetThingHasInformation(me, false, true, false)
                                end
                            else
                                if bVar3 then
                                    -- LAB_00d3ee93_c6: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_20c
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                                x_stk_6c = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_6c
                                fret_01 = quest:GetHealth(pCVar6)
                                c_stk_249 = 0.0 < fret_01
                                if c_stk_249 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar6 = quest:GetHero()
                                    r5 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d405d6 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        -- LAB_00d3ee93_c7: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_20c
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20c)
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 0)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00d4072e: (native jump target)
                    quest:DeregisterTimer(xStack_250)
                    quest:DeregisterTimer(xStack_258)
                    quest:DeregisterTimer(xStack_254)
                    goto LAB_00d40749
                end
                iVar5 = __native_entity_state:GetStateInt("RaceMode")
                uVar9 = u_stk_21c
                while true do
                    u_stk_21c = uVar9
                    if not (iVar5 == 1) then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d405fc end
                    fVar15 = 10.0
                    pCVar6 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar15)
                    __native_condition_2 = bVar3
                    if __native_condition_2 then
                        iVar5 = quest:GetTimer(xStack_258)
                        __native_condition_2 = iVar5 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d405fc end
                        iVar5 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(iVar5, r1)
                        if uVar9 < 6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            native_arg_switch_6 = uVar9
                            repeat
                                if native_arg_switch_6 == 1 then
                                    quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, r1, false)
                                    break
                                else
                                    if native_arg_switch_6 == 2 then
                                        quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", r1, me, false)
                                        break
                                    else
                                        if native_arg_switch_6 == 3 then
                                            quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, r1, false)
                                            break
                                        else
                                            if native_arg_switch_6 == 4 then
                                                quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", r1, me, false)
                                                break
                                            else
                                                if native_arg_switch_6 == 5 then
                                                    quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, r1, false)
                                                    break
                                                else
                                                    goto FLOW_native_label_2
                                                end
                                            end
                                        end
                                    end
                                end
                            until not (false)
                        else
                            uVar8 = uVar9 & 0x80000001
                            bVar3 = uVar8 == 0
                            if uVar8 < 0 then
                                bVar3 = (uVar8 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, r1, false)
                                    goto LAB_00d3f069
                                end
                                goto LAB_00d405fc
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            quest:AddLineToConversation(iVar5, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", r1, me, false)
                        end
                        ::LAB_00d3f069::
                        ::FLOW_native_label_2::
                        quest:SetTimer(xStack_258, 3)
                        u_stk_21c = uVar9 + 1
                    end
                    bVar3 = me:IsTalkedToByHero()
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(xStack_214)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            xStack_c4 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities((fVar15 ~= 0))
                            x_stk_54 = resources:ScriptThing(xStack_23c)
                            pCVar6 = x_stk_54
                            fret_02 = quest:GetHealth(pCVar6)
                            fVar2 = 0.0
                            if fVar2 < fret_02 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar7 = 0
                                iVar5 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM"
                                pCVar6 = quest:GetHero()
                                r6 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_c4
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_c4
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                            end
                            __native_entity_state:SetStateInt("RaceMode", 2)
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_c4
                        else
                            iVar5 = quest:GetTimer(xStack_254)
                            if iVar5 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                xStack_100 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_3c = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_3c
                                fret_03 = quest:GetHealth(pCVar6)
                                c_stk_249 = 0.0 < fret_03
                                if c_stk_249 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW"
                                    pCVar6 = quest:GetHero()
                                    r7 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_100
                                            resources:DestroyMovie(pCVar11)
                                            goto LAB_00d405fc
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_100
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 2)
                                if not quest:GetStateBool("ReachedPlatform") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        -- LAB_00d40684: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_100
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                    pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(pCVar6)
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_100
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                xStack_d4 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
                                x_stk_24 = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_24
                                fret_04 = quest:GetHealth(pCVar6)
                                fVar2 = 0.0
                                if fVar2 < fret_04 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED"
                                    pCVar6 = quest:GetHero()
                                    r8 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_d4
                                            resources:DestroyMovie(pCVar11)
                                            goto LAB_00d405fc
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_d4
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 3)
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xee0)))
                                quest:GiveHeroGold(iVar7)
                                quest:ClearThingHasInformation(me)
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_d4
                            end
                        end
                        resources:DestroyMovie(pCVar11)
                    end
                    iVar5 = quest:GetTimer(xStack_250)
                    if iVar5 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(xStack_214)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(pCVar6)
                        end
                        __native_entity_state:SetStateInt("RaceMode", 2)
                    end
                    uVar9 = u_stk_21c
                    iVar5 = __native_entity_state:GetStateInt("RaceMode")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d405fc end
                if __native_entity_state:GetStateInt("RaceMode") == 2 then
                    i_stk_210 = u_stk_21c - 1
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d405fc end
                        fVar15 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar15)
                        __native_condition_3 = bVar3
                        if __native_condition_3 then
                            iVar5 = quest:GetTimer(xStack_258)
                            __native_condition_3 = iVar5 < 1
                        end
                        if __native_condition_3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            iVar7 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar7, r1)
                            iVar5 = i_stk_210
                            if i_stk_210 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                native_arg_switch_7 = iVar5
                                repeat
                                    if native_arg_switch_7 == 0 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, r1, false)
                                        break
                                    else
                                        if native_arg_switch_7 == 1 then
                                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", r1, me, false)
                                            break
                                        else
                                            if native_arg_switch_7 == 2 then
                                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, r1, false)
                                                break
                                            else
                                                if native_arg_switch_7 == 3 then
                                                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", r1, me, false)
                                                    break
                                                else
                                                    if native_arg_switch_7 == 4 then
                                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, r1, false)
                                                        break
                                                    else
                                                        goto FLOW_native_label_3
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar9 = u_stk_21c & 0x80000001
                                bVar3 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar3 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, r1, false)
                                        goto LAB_00d3f894
                                    end
                                    goto LAB_00d405fc
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", r1, me, false)
                            end
                            ::LAB_00d3f894::
                            ::FLOW_native_label_3::
                            quest:SetTimer(xStack_258, 3)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar5 + 1
                        end
                        bVar3 = me:IsTalkedToByHero()
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            xStack_1fc = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_30 = resources:ScriptThing(xStack_23c)
                            pCVar6 = x_stk_30
                            fret_05 = quest:GetHealth(pCVar6)
                            fVar2 = 0.0
                            if fVar2 < fret_05 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar7 = 0
                                iVar5 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL"
                                pCVar6 = quest:GetHero()
                                r9 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_1fc
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d40702: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_1fc
                                resources:DestroyMovie(pCVar11)
                                goto LAB_00d405fc
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar5 == 1 then
                                if bVar3 then
                                    -- LAB_00d406ee: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                                x_stk_90 = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_90
                                fret_06 = quest:GetHealth(pCVar6)
                                fVar2 = 0.0
                                if fVar2 < fret_06 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar6 = quest:GetHero()
                                    r10 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_1fc
                                            resources:DestroyMovie(pCVar11)
                                            goto LAB_00d405fc
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_1fc
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xedc)))
                                quest:SetTimer(xStack_254, iVar7)
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xedc) + 20.0))
                                quest:SetTimer(xStack_250, iVar7)
                                xStack_214 = quest:AddQuestInfoTimer(xStack_250, "HUD_CLOCK_ICON", 1.0)
                                pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d405fc
                                end
                                x_stk_78 = resources:ScriptThing(xStack_23c)
                                pCVar6 = x_stk_78
                                fret_07 = quest:GetHealth(pCVar6)
                                fVar2 = 0.0
                                if fVar2 < fret_07 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar7 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar6 = quest:GetHero()
                                    r11 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_1fc
                                            resources:DestroyMovie(pCVar11)
                                            goto LAB_00d405fc
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_1fc
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d405fc
                                    end
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1fc)
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 2)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d405fc end
                if __native_entity_state:GetStateInt("RaceMode") == 3 then
                    i_stk_210 = u_stk_21c - 1
                    -- LAB_00d3fdc0: (native jump target)
                    iVar5 = i_stk_210
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        fVar15 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar15)
                        __native_condition_4 = bVar3
                        if __native_condition_4 then
                            iVar7 = quest:GetTimer(xStack_258)
                            __native_condition_4 = iVar7 < 1
                        end
                        if __native_condition_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d405fc end
                            iVar7 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar7, r1)
                            if iVar5 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                native_arg_switch_8 = iVar5
                                repeat
                                    if native_arg_switch_8 == 0 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", r1, me, false)
                                        break
                                    else
                                        if native_arg_switch_8 == 1 then
                                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, r1, false)
                                            break
                                        else
                                            if native_arg_switch_8 == 2 then
                                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", r1, me, false)
                                                break
                                            else
                                                if native_arg_switch_8 == 3 then
                                                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, r1, false)
                                                    break
                                                else
                                                    if native_arg_switch_8 == 4 then
                                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", r1, me, false)
                                                        break
                                                    else
                                                        goto FLOW_native_label_4
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar9 = u_stk_21c & 0x80000001
                                bVar3 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar3 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", r1, me, false)
                                        goto LAB_00d40030
                                    end
                                    goto LAB_00d405fc
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d405fc end
                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, r1, false)
                            end
                            ::LAB_00d40030::
                            ::FLOW_native_label_4::
                            quest:SetTimer(xStack_258, 7)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar5 + 1
                        end
                        bVar3 = me:IsTalkedToByHero()
                        if not bVar3 then goto LAB_00d403e1 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            xStack_f0 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d40716: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if iVar5 ~= 1 then
                                    if not bVar3 then
                                        x_stk_b4 = resources:ScriptThing(xStack_23c)
                                        pCVar6 = x_stk_b4
                                        fret_09 = quest:GetHealth(pCVar6)
                                        c_stk_249 = 0.0 < fret_09
                                        if c_stk_249 then
                                            iVar16 = 0
                                            iVar14 = 1
                                            iVar7 = 0
                                            iVar5 = 0
                                            pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                                            pCVar6 = quest:GetHero()
                                            r12 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d402b3 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto FLOW_after_lab_00d402b3
                                            end
                                        end
                                        pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                                        quest:MiniMapRemoveMarker(pCVar6)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_f0)
                                        goto LAB_00d403e1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                if not bVar3 then
                                    x_stk_18 = resources:ScriptThing(xStack_23c)
                                    pCVar6 = x_stk_18
                                    fret_08 = quest:GetHealth(pCVar6)
                                    c_stk_249 = 0.0 < fret_08
                                    if c_stk_249 then
                                        iVar16 = 0
                                        iVar14 = 1
                                        iVar7 = 0
                                        iVar5 = 0
                                        pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                                        pCVar6 = quest:GetHero()
                                        r13 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto FLOW_after_lab_00d402b3
                                            end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d402b3 end
                                    end
                                    -- LAB_00d40379: (native jump target)
                                    pCVar6 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(pCVar6)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_f0)
                                    goto LAB_00d403e1
                                end
                                ::LAB_00d402b3::
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            ::FLOW_after_lab_00d402b3::
                            pCVar11 = xStack_f0
                            -- LAB_00d40729: (native jump target)
                            resources:DestroyMovie(pCVar11)
                        end
                    end
                    goto LAB_00d405fc
                end
                -- LAB_00d403eb: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d405fc end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            end
            ::LAB_00d405fc::
            quest:DeregisterTimer(xStack_250)
            quest:DeregisterTimer(xStack_258)
            quest:DeregisterTimer(xStack_254)
            ::LAB_00d40749::
        end
        ::LAB_00d4075b::
        resources:ReleaseResource(xStack_23c)
    end
    ::FLOW_after_lab_00d405fc::
    do return end
    ::LAB_00d405d6::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_20c)
    quest:DeregisterTimer(xStack_250)
    quest:DeregisterTimer(xStack_258)
    quest:DeregisterTimer(xStack_254)
    -- LAB_00d40749_c30: (native jump target)
    -- LAB_00d4075b_c30: (native jump target)
    resources:ReleaseResource(xStack_23c)
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if __native_entity_state:GetStateInt("RaceMode") ~= 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d405fc_c31 end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        ::LAB_00d405fc_c31::
        quest:DeregisterTimer(xStack_250)
        quest:DeregisterTimer(xStack_258)
        quest:DeregisterTimer(xStack_254)
        -- LAB_00d40749_c31: (native jump target)
        -- LAB_00d4075b_c31: (native jump target)
        resources:ReleaseResource(xStack_23c)
        goto FLOW_after_lab_00d405fc
    end
    iVar5 = i_stk_210
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        fVar15 = 10.0
        pCVar6 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar15)
        __native_condition_5 = bVar3
        if __native_condition_5 then
            iVar7 = quest:GetTimer(xStack_258)
            __native_condition_5 = iVar7 < 1
        end
        if __native_condition_5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d405fc_c32 end
            iVar7 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(iVar7, r1)
            if iVar5 < 5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d405fc_c32 end
                native_arg_switch_8 = iVar5
                repeat
                    if native_arg_switch_8 == 0 then
                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", r1, me, false)
                        break
                    else
                        if native_arg_switch_8 == 1 then
                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, r1, false)
                            break
                        else
                            if native_arg_switch_8 == 2 then
                                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", r1, me, false)
                                break
                            else
                                if native_arg_switch_8 == 3 then
                                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, r1, false)
                                    break
                                else
                                    if native_arg_switch_8 == 4 then
                                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", r1, me, false)
                                        break
                                    else
                                        goto FLOW_native_label_4_c32
                                    end
                                end
                            end
                        end
                    end
                until not (false)
            else
                uVar9 = u_stk_21c & 0x80000001
                bVar3 = uVar9 == 0
                if uVar9 < 0 then
                    bVar3 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", r1, me, false)
                        goto LAB_00d40030_c32
                    end
                    goto LAB_00d405fc_c32
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d405fc_c32 end
                quest:AddLineToConversation(iVar7, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, r1, false)
            end
            ::LAB_00d40030_c32::
            ::FLOW_native_label_4_c32::
            quest:SetTimer(xStack_258, 7)
            u_stk_21c = u_stk_21c + 1
            i_stk_210 = iVar5 + 1
        end
        bVar3 = me:IsTalkedToByHero()
        if not bVar3 then goto LAB_00d403e1 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_f0 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar5 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then return end  -- TODO(native): goto LAB_00d402b3_c32
                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00d40716_c32: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if iVar5 ~= 1 then
                    if not bVar3 then
                        x_stk_b4 = resources:ScriptThing(xStack_23c)
                        pCVar6 = x_stk_b4
                        fret_09 = quest:GetHealth(pCVar6)
                        c_stk_249 = 0.0 < fret_09
                        if c_stk_249 then
                            iVar16 = 0
                            iVar14 = 1
                            iVar7 = 0
                            iVar5 = 0
                            pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                            pCVar6 = quest:GetHero()
                            r14 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00d402b3_c32
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then return end  -- TODO(native): goto LAB_00d40716_c32
                        end
                        __region_LAB_00d40379_c32(); goto LAB_00d403e1
                    end
                    -- TODO(native): goto LAB_00d40716_c32
                end
                if not bVar3 then
                    x_stk_18 = resources:ScriptThing(xStack_23c)
                    pCVar6 = x_stk_18
                    fret_08 = quest:GetHealth(pCVar6)
                    c_stk_249 = 0.0 < fret_08
                    if c_stk_249 then
                        iVar16 = 0
                        iVar14 = 1
                        iVar7 = 0
                        iVar5 = 0
                        pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                        pCVar6 = quest:GetHero()
                        r15 = me:Speak(pCVar6, pcVar13, iVar5, (iVar7 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then return end  -- TODO(native): goto LAB_00d40716_c32
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then return end  -- TODO(native): goto LAB_00d402b3_c32
                    end
                    __region_LAB_00d40379_c32()
                    goto LAB_00d403e1
                end
                -- LAB_00d402b3_c32: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
            end
            pCVar11 = xStack_f0
            -- LAB_00d40729_c32: (native jump target)
            resources:DestroyMovie(pCVar11)
        end
    end
    goto LAB_00d405fc_c32
    -- LAB_00d403eb_c32: (native jump target)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d405fc_c32 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    ::LAB_00d405fc_c32::
    quest:DeregisterTimer(xStack_250)
    quest:DeregisterTimer(xStack_258)
    quest:DeregisterTimer(xStack_254)
    -- LAB_00d40749_c32: (native jump target)
    -- LAB_00d4075b_c32: (native jump target)
    resources:ReleaseResource(xStack_23c)
    goto FLOW_after_lab_00d405fc
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

