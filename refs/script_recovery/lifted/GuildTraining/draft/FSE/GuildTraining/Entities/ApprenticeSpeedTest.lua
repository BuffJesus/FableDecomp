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
    local __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, bVar2, cVar3, c_stk_249, fVar1, fVar15, fret_0, fret_00, fret_01, fret_03, fret_08, fret_09, iVar14, iVar16, iVar4, iVar6, i_stk_210, native_arg_switch_5, native_arg_switch_6, native_arg_switch_7, native_arg_switch_8, p0, pCVar11, pCVar17, pCVar5, pcVar13, piVar12, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r2, r3, r4, r5, r6, r7, r8, r9, thing, uVar7, uVar8, uVar9, u_stk_21c, u_stk_220, xStack_100, xStack_1fc, xStack_20c, xStack_214, xStack_23c, xStack_24, xStack_250, xStack_254, xStack_30, xStack_54, xStack_78, xStack_90, xStack_c4, xStack_d4, xStack_e0, xStack_f0, x_stk_18, x_stk_3c, x_stk_6c, x_stk_9c, x_stk_b4
    local alive = true
    local function __region_LAB_00d3ee93()
        quest:PauseAllNonScriptedEntities(false)
        pCVar11 = xStack_20c
        resources:DestroyMovie(pCVar11)
    end
    local function __region_LAB_00d40684()
        quest:PauseAllNonScriptedEntities(false)
        pCVar11 = xStack_100
        resources:DestroyMovie(pCVar11)
    end
    local function __region_LAB_00d40702()
        pCVar11 = xStack_1fc
        resources:DestroyMovie(pCVar11)
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_23c = resources:NewResource()
        bVar2 = false
        if bVar2 ~= 0 then
        end
        bVar2 = resources:TryAcquire(xStack_23c, me, 4)
        while not bVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d4075b end
            bVar2 = resources:TryAcquire(xStack_23c, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            uVar7 = __native_entity_state:GetStateInt("self_0xc")
            -- TODO(native): thing._4_4_ = uVar7;
            thing = nil
            -- TODO(native): thing._8_4_ = piVar12;
            quest:SetIsPushableByHero(nil --[[missing]], (thing ~= 0))
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
            iVar4 = quest:RegisterTimer()
            xStack_250 = quest:RegisterTimer()
            quest:SetTimer(xStack_250, iVar4)
            u_stk_220 = u_stk_220 & 0xffffff
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            while not bVar2 do
                if __native_entity_state:GetStateInt("RaceMode") == 0 then
                    i_stk_210 = u_stk_21c - 1
                    repeat
                        iVar4 = i_stk_210
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00d40749
                        end
                        if (quest:GetStateInt("GameState") == 3) and (uStack_220_b3 == 0) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            quest:SetThingHasInformation(me, false, true, false)
                            u_stk_220 = CONCAT13(1,u_stk_220)
                        end
                        fVar15 = 10.0
                        pCVar5 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar15)
                        __native_condition_1 = bVar2
                        if __native_condition_1 then
                            iVar6 = quest:GetTimer(xStack_250)
                            __native_condition_1 = iVar6 < 1
                        end
                        if __native_condition_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            iVar6 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar6, r1)
                            if iVar4 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                native_arg_switch_5 = iVar4
                                repeat
                                    if native_arg_switch_5 == 0 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, r1, false)
                                        break
                                    else
                                        if native_arg_switch_5 == 1 then
                                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", r1, me, false)
                                            break
                                        else
                                            if native_arg_switch_5 == 2 then
                                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, r1, false)
                                                break
                                            else
                                                if native_arg_switch_5 == 3 then
                                                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", r1, me, false)
                                                    break
                                                else
                                                    if native_arg_switch_5 == 4 then
                                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, r1, false)
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
                                bVar2 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar2 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, r1, false)
                                        goto LAB_00d3e7f7
                                    end
                                    goto LAB_00d40749
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", r1, me, false)
                            end
                            ::LAB_00d3e7f7::
                            ::FLOW_native_label_1::
                            quest:SetTimer(xStack_250, xStack_258)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar4 + 1
                        end
                        bVar2 = me:IsTalkedToByHero()
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            xStack_20c = resources:StartMovie("")
                            quest:StartMovieSequence()
                            -- TODO(native): piStack_218 = piVar12;
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_e0 = resources:ScriptThing(xStack_23c)
                            pCVar5 = xStack_e0
                            fret_0 = quest:GetHealth(pCVar5)
                            c_stk_249 = 0.0 < fret_0
                            xStack_e0 = nil
                            if c_stk_249 ~= 0 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar6 = 0
                                iVar4 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_BOAST"
                                pCVar5 = quest:GetHero()
                                r3 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar4 = me:IsPerformingScriptTask()
                                cVar3 = iVar4
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_20c)
                                        quest:DeregisterTimer(xStack_250)
                                        quest:DeregisterTimer(xStack_250)
                                        quest:DeregisterTimer(xStack_250)
                                        goto LAB_00d40749
                                    end
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_20c
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d40749
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar4 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20c)
                                    quest:DeregisterTimer(xStack_250)
                                    quest:DeregisterTimer(xStack_250)
                                    quest:DeregisterTimer(xStack_250)
                                    r2 = nil
                                    r1 = nil
                                    return
                                end
                                iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_20c
                                resources:DestroyMovie(pCVar11)
                                goto LAB_00d40749
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if iVar4 == 1 then
                                if bVar2 then
                                    __region_LAB_00d3ee93()
                                    goto LAB_00d40749
                                end
                                x_stk_9c = resources:ScriptThing(xStack_20c)
                                pCVar5 = x_stk_9c
                                fret_00 = quest:GetHealth(pCVar5)
                                c_stk_249 = 0.0 < fret_00
                                if c_stk_249 ~= 0 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar5 = quest:GetHero()
                                    r4 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d405d6 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then __region_LAB_00d3ee93(); goto LAB_00d40749 end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(pCVar5, "HUD_ORB_QUEST_VIGNETTE")
                                uVar7 = __ftol2()
                                quest:SetTimer(xStack_250, xStack_254)
                                uVar7 = __ftol2()
                                quest:SetTimer(xStack_250, 0)
                                xStack_214 = quest:AddQuestInfoTimer(xStack_250, "HUD_CLOCK_ICON", 1.0)
                                quest:DisplayQuestInfo(true)
                                if uStack_220_b3 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        -- TODO(native): (**(code **)(*piStack_218 + 0x5ec))(piStack_218,false);
                                        pCVar11 = xStack_23c
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d40749
                                    end
                                    quest:SetThingHasInformation(me, false, true, false)
                                    u_stk_220 = CONCAT13(1,u_stk_220)
                                end
                            else
                                if bVar2 then __region_LAB_00d3ee93(); goto LAB_00d40749 end
                                x_stk_6c = resources:ScriptThing(xStack_23c)
                                pCVar5 = x_stk_6c
                                fret_01 = quest:GetHealth(pCVar5)
                                c_stk_249 = 0.0 < fret_01
                                if c_stk_249 ~= 0 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar5 = quest:GetHero()
                                    r5 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d405d6 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then __region_LAB_00d3ee93(); goto LAB_00d40749 end
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_23c)
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 0)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    -- LAB_00d4072e: (native jump target)
                    goto LAB_00d40749
                end
                iVar4 = __native_entity_state:GetStateInt("RaceMode")
                uVar9 = u_stk_21c
                while true do
                    u_stk_21c = uVar9
                    if not (iVar4 == 1) then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        goto LAB_00d40749
                    end
                    fVar15 = 10.0
                    pCVar5 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar15)
                    __native_condition_2 = bVar2
                    if __native_condition_2 then
                        iVar4 = quest:GetTimer(xStack_250)
                        __native_condition_2 = iVar4 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00d40749
                        end
                        iVar4 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(iVar4, pCVar5)
                        if uVar9 < 6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            native_arg_switch_6 = uVar9
                            repeat
                                if native_arg_switch_6 == 1 then
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, nil --[[missing]], false)
                                    break
                                else
                                    if native_arg_switch_6 == 2 then
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", me, nil --[[missing]], false)
                                        break
                                    else
                                        if native_arg_switch_6 == 3 then
                                            quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, nil --[[missing]], false)
                                            break
                                        else
                                            if native_arg_switch_6 == 4 then
                                                quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", me, nil --[[missing]], false)
                                                break
                                            else
                                                if native_arg_switch_6 == 5 then
                                                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, nil --[[missing]], false)
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
                            bVar2 = uVar8 == 0
                            if uVar8 < 0 then
                                bVar2 = (uVar8 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, nil --[[missing]], false)
                                    goto LAB_00d3f069
                                end
                                goto LAB_00d40749
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            quest:AddLineToConversation(iVar4, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", me, nil --[[missing]], false)
                        end
                        ::LAB_00d3f069::
                        ::FLOW_native_label_2::
                        quest:SetTimer(xStack_250, xStack_258)
                        u_stk_21c = uVar9 + 1
                    end
                    bVar2 = me:IsTalkedToByHero()
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00d40749
                        end
                        quest:RemoveQuestInfoElement(xStack_214)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            xStack_c4 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            pCVar17 = 0x1
                            quest:PauseAllNonScriptedEntities((pCVar17 ~= 0))
                            xStack_54 = resources:ScriptThing(xStack_23c)
                            pCVar5 = xStack_54
                            r6 = quest:GetHealth(pCVar5)
                            fVar1 = 0.0
                            if fVar1 < fret_02 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar6 = 0
                                iVar4 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM"
                                pCVar5 = quest:GetHero()
                                r7 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar4 = me:IsPerformingScriptTask()
                                cVar3 = iVar4
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_c4
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d40749
                                    end
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_c4
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d40749
                                end
                            end
                            __native_entity_state:SetStateInt("RaceMode", 2)
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_c4
                        else
                            iVar4 = quest:GetTimer(xStack_250)
                            if iVar4 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                xStack_100 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_3c = resources:ScriptThing(xStack_23c)
                                pCVar5 = x_stk_3c
                                fret_03 = quest:GetHealth(pCVar5)
                                c_stk_249 = 0.0 < fret_03
                                if c_stk_249 ~= 0 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW"
                                    pCVar5 = quest:GetHero()
                                    r8 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then __region_LAB_00d40684(); goto LAB_00d40749 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_100
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d40749
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 2)
                                if not quest:GetStateBool("ReachedPlatform") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        __region_LAB_00d40684()
                                        goto LAB_00d40749
                                    end
                                    pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(pCVar5)
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_100
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                xStack_d4 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                pCVar17 = 0x1
                                quest:PauseAllNonScriptedEntities((pCVar17 ~= 0))
                                xStack_24 = resources:ScriptThing(xStack_23c)
                                pCVar5 = xStack_24
                                r9 = quest:GetHealth(pCVar5)
                                fVar1 = 0.0
                                if fVar1 < fret_04 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED"
                                    pCVar5 = quest:GetHero()
                                    r10 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_d4
                                            resources:DestroyMovie(pCVar11)
                                            goto LAB_00d40749
                                        end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_d4
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d40749
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 3)
                                iVar6 = __ftol2()
                                quest:GiveHeroGold(fVar1)
                                quest:ClearThingHasInformation(me)
                                u_stk_220 = 0
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_d4
                            end
                        end
                        resources:DestroyMovie(pCVar11)
                    end
                    iVar4 = quest:GetTimer(xStack_250)
                    if iVar4 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00d40749
                        end
                        quest:RemoveQuestInfoElement(xStack_214)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(pCVar5)
                        end
                        __native_entity_state:SetStateInt("RaceMode", 2)
                    end
                    uVar9 = u_stk_21c
                    iVar4 = __native_entity_state:GetStateInt("RaceMode")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto LAB_00d40749
                end
                if __native_entity_state:GetStateInt("RaceMode") == 2 then
                    i_stk_210 = u_stk_21c - 1
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00d40749
                        end
                        fVar15 = 10.0
                        pCVar5 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar15)
                        __native_condition_3 = bVar2
                        if __native_condition_3 then
                            iVar4 = quest:GetTimer(xStack_250)
                            __native_condition_3 = iVar4 < 1
                        end
                        if __native_condition_3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            iVar6 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar6, pCVar5)
                            iVar4 = i_stk_210
                            if i_stk_210 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                native_arg_switch_7 = iVar4
                                repeat
                                    if native_arg_switch_7 == 0 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, nil --[[missing]], false)
                                        break
                                    else
                                        if native_arg_switch_7 == 1 then
                                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", me, nil --[[missing]], false)
                                            break
                                        else
                                            if native_arg_switch_7 == 2 then
                                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, nil --[[missing]], false)
                                                break
                                            else
                                                if native_arg_switch_7 == 3 then
                                                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", me, nil --[[missing]], false)
                                                    break
                                                else
                                                    if native_arg_switch_7 == 4 then
                                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, nil --[[missing]], false)
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
                                bVar2 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar2 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, nil --[[missing]], false)
                                        goto LAB_00d3f894
                                    end
                                    goto LAB_00d40749
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", me, nil --[[missing]], false)
                            end
                            ::LAB_00d3f894::
                            ::FLOW_native_label_3::
                            quest:SetTimer(xStack_250, xStack_258)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar4 + 1
                        end
                        bVar2 = me:IsTalkedToByHero()
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            xStack_1fc = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_30 = resources:ScriptThing(xStack_23c)
                            pCVar5 = xStack_30
                            r11 = quest:GetHealth(pCVar5)
                            fVar1 = 0.0
                            if fVar1 < fret_05 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar6 = 0
                                iVar4 = 0
                                pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL"
                                pCVar5 = quest:GetHero()
                                r12 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar4 = me:IsPerformingScriptTask()
                                cVar3 = iVar4
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_1fc
                                        resources:DestroyMovie(pCVar11)
                                        goto LAB_00d40749
                                    end
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d40749
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar4 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                __region_LAB_00d40702()
                                goto LAB_00d40749
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if iVar4 == 1 then
                                if bVar2 then
                                    -- LAB_00d406ee: (native jump target)
                                    -- TODO(native): (**(code **)(*piStack_218 + 0x5ec))(piStack_218,false);
                                    pCVar11 = xStack_1fc
                                    resources:DestroyMovie(pCVar11)
                                    goto LAB_00d40749
                                end
                                xStack_90 = resources:ScriptThing(xStack_23c)
                                pCVar5 = xStack_90
                                r13 = quest:GetHealth(pCVar5)
                                fVar1 = 0.0
                                if fVar1 < fret_06 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar5 = quest:GetHero()
                                    r14 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                uVar7 = __ftol2()
                                quest:SetTimer(xStack_250, xStack_254)
                                uVar7 = __ftol2()
                                quest:SetTimer(xStack_250, fVar1)
                                xStack_214 = quest:AddQuestInfoTimer(xStack_250, "HUD_CLOCK_ICON", 1.0)
                                pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(pCVar5, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                xStack_78 = resources:ScriptThing(xStack_23c)
                                pCVar5 = xStack_78
                                r15 = quest:GetHealth(pCVar5)
                                fVar1 = 0.0
                                if fVar1 < fret_07 then
                                    iVar16 = 0
                                    iVar14 = 1
                                    iVar6 = 0
                                    iVar4 = 0
                                    pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar5 = quest:GetHero()
                                    r16 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar3 = iVar4
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then __region_LAB_00d40702(); goto LAB_00d40749 end
                                end
                            end
                            -- TODO(native): (**(code **)(*piStack_218 + 0x5ec))(piStack_218,false);
                            resources:DestroyMovie(xStack_1fc)
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 2)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto LAB_00d40749
                end
                if __native_entity_state:GetStateInt("RaceMode") == 3 then
                    i_stk_210 = u_stk_21c - 1
                    -- LAB_00d3fdc0: (native jump target)
                    iVar4 = i_stk_210
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        fVar15 = 10.0
                        pCVar5 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar15)
                        __native_condition_4 = bVar2
                        if __native_condition_4 then
                            iVar6 = quest:GetTimer(xStack_250)
                            __native_condition_4 = iVar6 < 1
                        end
                        if __native_condition_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                goto LAB_00d40749
                            end
                            iVar6 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar6, pCVar5)
                            if iVar4 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                native_arg_switch_8 = iVar4
                                repeat
                                    if native_arg_switch_8 == 0 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", me, nil --[[missing]], false)
                                        break
                                    else
                                        if native_arg_switch_8 == 1 then
                                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, nil --[[missing]], false)
                                            break
                                        else
                                            if native_arg_switch_8 == 2 then
                                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", me, nil --[[missing]], false)
                                                break
                                            else
                                                if native_arg_switch_8 == 3 then
                                                    quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, nil --[[missing]], false)
                                                    break
                                                else
                                                    if native_arg_switch_8 == 4 then
                                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", me, nil --[[missing]], false)
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
                                bVar2 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar2 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if not bVar2 then
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", me, nil --[[missing]], false)
                                        goto LAB_00d40030
                                    end
                                    goto LAB_00d40749
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    goto LAB_00d40749
                                end
                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, nil --[[missing]], false)
                            end
                            ::LAB_00d40030::
                            ::FLOW_native_label_4::
                            quest:SetTimer(xStack_250, xStack_258)
                            u_stk_21c = u_stk_21c + 1
                            i_stk_210 = iVar4 + 1
                        end
                        bVar2 = me:IsTalkedToByHero()
                        if not bVar2 then goto LAB_00d403e1 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            xStack_f0 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar4 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                -- LAB_00d40716: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if iVar4 ~= 1 then
                                    if not bVar2 then
                                        x_stk_18 = resources:ScriptThing(xStack_23c)
                                        pCVar5 = x_stk_18
                                        fret_09 = quest:GetHealth(pCVar5)
                                        c_stk_249 = 0.0 < fret_09
                                        if c_stk_249 ~= 0 then
                                            iVar16 = 0
                                            iVar14 = 1
                                            iVar6 = 0
                                            iVar4 = 0
                                            pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                                            pCVar5 = quest:GetHero()
                                            r17 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                            iVar4 = me:IsPerformingScriptTask()
                                            cVar3 = iVar4
                                            while cVar3 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                                if bVar2 then goto LAB_00d402b3 end
                                                iVar4 = me:IsPerformingScriptTask()
                                                cVar3 = iVar4
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto FLOW_after_lab_00d402b3
                                            end
                                        end
                                        pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                                        quest:MiniMapRemoveMarker(pCVar5)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_f0)
                                        goto LAB_00d403e1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                if not bVar2 then
                                    x_stk_b4 = resources:ScriptThing(xStack_23c)
                                    pCVar5 = x_stk_b4
                                    fret_08 = quest:GetHealth(pCVar5)
                                    c_stk_249 = 0.0 < fret_08
                                    if c_stk_249 ~= 0 then
                                        iVar16 = 0
                                        iVar14 = 1
                                        iVar6 = 0
                                        iVar4 = 0
                                        pcVar13 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                                        pCVar5 = quest:GetHero()
                                        r18 = me:Speak(pCVar5, pcVar13, iVar4, (iVar6 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar3 = iVar4
                                        while cVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                goto FLOW_after_lab_00d402b3
                                            end
                                            iVar4 = me:IsPerformingScriptTask()
                                            cVar3 = iVar4
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d402b3 end
                                    end
                                    -- LAB_00d40379: (native jump target)
                                    pCVar5 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(pCVar5)
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
                    goto LAB_00d40749
                end
                -- LAB_00d403eb: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto LAB_00d40749
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
            end
            -- LAB_00d405fc: (native jump target)
            quest:DeregisterTimer(xStack_250)
            quest:DeregisterTimer(xStack_250)
            quest:DeregisterTimer(xStack_250)
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
    quest:DeregisterTimer(xStack_250)
    quest:DeregisterTimer(xStack_250)
    -- LAB_00d40749_c52: (native jump target)
    -- LAB_00d4075b_c52: (native jump target)
    resources:ReleaseResource(xStack_23c)
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if __native_entity_state:GetStateInt("RaceMode") ~= 3 then return end  -- TODO(native): goto LAB_00d403eb
    -- TODO(native): goto LAB_00d3fdc0
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

