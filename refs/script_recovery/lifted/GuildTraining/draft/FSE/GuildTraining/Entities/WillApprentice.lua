-- Generated native draft: WillApprentice. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, bVar4, cVar5, c_stk_115, c_stk_131, fVar18, fVar3, iVar13, iVar14, iVar16, iVar17, iVar6, native_arg_sequence_1, native_arg_switch_2, p0, pCVar15, pCVar7, pCVar8, pcVar12, pfVar11, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r3, r4, r5, r6, r7, r8, r9, thing, timerId, uVar1, xStack_104, xStack_114, xStack_124, xStack_148, xStack_150, xStack_24, xStack_3c, xStack_4c, xStack_5c, xStack_6c, xStack_c, xStack_d8, x_stk_108, x_stk_18
    local alive = true
    local function __cleanup_LAB_00d504b9()
        quest:DeregisterTimer(xStack_150)
        resources:DestroyMovie(xStack_4c)
    end
    local function __cleanup_LAB_00d504b9_c1()
        quest:DeregisterTimer(xStack_150)
        resources:DestroyMovie(xStack_4c)
    end
    local function __region_LAB_00d50588_c1()
        resources:DestroyMovie(xStack_4c)
    end
    local function __cleanup_LAB_00d505a6()
        resources:DestroyMovie(xStack_4c)
    end
    local function __cleanup_LAB_00d505af_c1()
        resources:DestroyMovie(xStack_4c)
    end
    xStack_148 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_148, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            do return end
            bVar4 = quest:IsQuestActive("Q_GuildTrainingDeparture")
            if not bVar4 then goto LAB_00d4f285_c1 end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                __cleanup_LAB_00d504b9_c1()
                return
            end
            iVar6 = __native_entity_state:GetStateInt("self_0x18")
            if ((3 < *(iVar6 + 0xb4)) or (3 < *(iVar6 + 0xb8))) or (3 < *(iVar6 + 0xbc)) then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    if c_stk_131 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                        quest:ClearThingHasInformation(me)
                        c_stk_131 = 0
                    end
                    goto LAB_00d4f285_c1
                end
                __cleanup_LAB_00d504b9_c1(); return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
            if c_stk_131 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                quest:SetThingHasInformation(me, false, true, false)
                c_stk_131 = '\x01'
            end
            ::LAB_00d4f285_c1::
            bVar4 = quest:IsDistanceBetweenThingsOver(me, xStack_3c, 4.0)
            __native_condition_1 = not bVar4
            if not __native_condition_1 then
                iVar6 = me:IsPerformingScriptTask()
                __native_condition_1 = iVar6
            end
            if __native_condition_1 then
                iVar6 = me:IsPerformingScriptTask()
                if iVar6 then goto LAB_00d4f49e_c1 end
                fVar18 = 10.0
                pCVar7 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar18)
                native_arg_sequence_1 = false
                if not bVar4 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    iVar6 = quest:GetTimer(xStack_150)
                    if 0 < iVar6 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00d4f49e_c1 end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    bVar4 = false
                    pCVar7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar4)
                    quest:SetTimer(xStack_150, 0x14)
                    iVar13 = quest:AddNewConversation(me, false, false)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar13, pCVar7)
                    iVar6 = quest:GetMasterGameState("GlobalWillGrade")
                    if iVar6 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", me, pCVar7, false)
                            -- TODO(native): goto LAB_00d4f499_c1
                        end
                    else
                        if iVar6 == 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                pCVar7 = quest:GetHero()
                                quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", me, pCVar7, false)
                                -- TODO(native): goto LAB_00d4f499_c1
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                pCVar7 = quest:GetHero()
                                quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", me, pCVar7, false)
                                -- LAB_00d4f499_c1: (native jump target)
                                goto LAB_00d4f49e_c1
                            end
                        end
                    end
                end
                -- TODO(native): goto LAB_00d505a6_c1
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d504b9_c1(); return end
            if xStack_130 == nil then
            end
            me:MoveToPosition(nil --[[missing]], p0, 0x40400000, false, false)
            ::LAB_00d4f49e_c1::
            bVar4 = me:IsTalkedToByHero()
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d504b9_c1(); return end
                iVar6 = me:IsPerformingScriptTask()
                if not iVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                    if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            xStack_4c = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            xStack_3c = resources:ScriptThing(0)
                            pCVar8 = xStack_3c
                            r1 = quest:GetHealth(pCVar8)
                            fVar3 = 0.0
                            if fVar3 < fret_00 then
                                iVar16 = 0
                                iVar14 = 1
                                iVar13 = 0
                                iVar6 = 0
                                pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE"
                                pCVar8 = quest:GetHero()
                                r2 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_5c)
                                        -- TODO(native): goto LAB_00d505a6_c1
                                    end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar5 = iVar6
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_104)
                                    -- TODO(native): goto LAB_00d505a6_c1
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_104)
                            goto LAB_00d503cb_c1
                        end
                        -- TODO(native): goto LAB_00d505a6_c1
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                    xStack_5c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar6 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00d50542_c1
                        iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d4fa43_c1
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if iVar6 ~= 1 then
                        if not bVar4 then
                            x_stk_18 = resources:ScriptThing(0)
                            pCVar8 = x_stk_18
                            r3 = quest:GetHealth(pCVar8)
                            fVar3 = 0.0
                            if fVar3 < fret_03 then
                                iVar17 = 0
                                iVar16 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_RETURN"
                                pCVar8 = quest:GetHero()
                                r4 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar5 = iVar13
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then return end  -- TODO(native): goto LAB_00d4fa43_c1
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar5 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then return end  -- TODO(native): goto LAB_00d50542_c1
                            end
                            goto LAB_00d4fb0a_c1
                        end
                        -- LAB_00d50542_c1: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_6c)
                        -- TODO(native): goto LAB_00d505a6_c1
                    end
                    if bVar4 then return end  -- TODO(native): goto LAB_00d50542_c1
                    xStack_c = resources:ScriptThing(0)
                    pCVar8 = xStack_c
                    r5 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_01 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar14 = 0
                        iVar13 = 0
                        pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO"
                        pCVar8 = quest:GetHero()
                        r6 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then return end  -- TODO(native): goto LAB_00d4fa43_c1
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00d50542_c1
                    end
                    if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d4fa43_c1: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_d8)
                            -- TODO(native): goto LAB_00d505a6_c1
                        end
                        xStack_24 = resources:ScriptThing(0)
                        pCVar8 = xStack_24
                        r7 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_02 then
                            iVar17 = 0
                            iVar16 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS"
                            pCVar8 = quest:GetHero()
                            r8 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then return end  -- TODO(native): goto LAB_00d50542_c1
                                iVar13 = me:IsPerformingScriptTask()
                                cVar5 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then return end  -- TODO(native): goto LAB_00d4fa43_c1
                        end
                    end
                    ::LAB_00d4fb0a_c1::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_4c)
                    if iVar6 ~= 1 then goto LAB_00d503cb_c1 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                    quest:SetPlayerUsingWillDummies(true)
                    quest:SetMasterGameState("HeroTakingGuildTest", true)
                    bVar4 = quest:MsgOnHeroCastSpell()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                        bVar4 = quest:MsgOnHeroCastSpell()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00d505a6_c1
                    -- TODO(native): CTimer::CTimer((CTimer *)&xStack_138);
                    -- TODO(native): piVar2 = DAT_0143e8f8;
                    iVar13 = __ftol2()
                    quest:SetTimer(xStack_138, fVar3)
                    quest:SetMasterGameState("WillScore", 0)
                    quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 0)
                    iVar6 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                    x_stk_108 = quest:AddQuestInfoTimer(1.0, "HUD_CLOCK_ICON", fVar18)
                    quest:DisplayQuestInfo(true)
                    cVar5 = 0
                    c_stk_115 = 0
                    -- TODO(native): CTimer::CTimer((CTimer *)&xStack_14c);
                    quest:SetTimer(xStack_14c, 0)
                    iVar13 = quest:GetTimer(xStack_138)
                    while (0 < iVar13 and (cVar5 == 0)) do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594_c1 end
                        iVar6 = quest:GetTimer(0)
                        if iVar6 < 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            bVar4 = false
                            pCVar8 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar4)
                            quest:SetTimer(2, nil --[[missing]])
                        end
                        if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            cVar5 = '\x01'
                            c_stk_115 = '\x01'
                        end
                        iVar6 = quest:GetHeroWillEnergy()
                        __native_condition_2 = iVar6 == 0
                        if __native_condition_2 then
                            iVar6 = quest:GetTimer(quest:GetStateInt("WillHelpTimer"))
                            __native_condition_2 = iVar6 < 1
                        end
                        if __native_condition_2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            iVar13 = quest:AddNewConversation(me, false, false)
                            pCVar8 = quest:GetHero()
                            quest:AddPersonToConversation(iVar13, pCVar8)
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", me, pCVar8, false)
                            quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 8)
                            cVar5 = c_stk_115
                        end
                        fVar18 = 30.0
                        pCVar8 = quest:GetHero()
                        bVar4 = quest:IsDistanceBetweenThingsOver(pCVar8, me, fVar18)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            cVar5 = '\x01'
                            c_stk_115 = '\x01'
                        end
                        iVar6 = iVar6
                        quest:UpdateQuestInfoCounter(iVar6, quest:GetMasterGameState("WillScore"), -1)
                        iVar13 = quest:GetTimer(xStack_138)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        bVar4 = quest:IsHeroControlledByPlayer()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            bVar4 = quest:IsHeroControlledByPlayer()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594_c1 end
                        quest:DisplayQuestInfo(false)
                        quest:RemoveQuestInfoElement(iVar6)
                        quest:RemoveQuestInfoElement(x_stk_108)
                        if cVar5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50594_c1 end
                            x_stk_108 = quest:GetMasterGameState("WillScore")
                            -- TODO(native): pfVar11 = *(float **)(DAT_0143e90c + 0xecc);
                            iVar6 = 0
                            repeat
                                iVar13 = iVar6
                                if *pfVar11 < x_stk_108 ~= (*pfVar11 == x_stk_108) then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d50594_c1 end
                                    break
                                end
                                pfVar11 = pfVar11 + 1
                                iVar6 = iVar13 + 1
                            until not (iVar13 + 1 < 7)
                            xStack_104 = resources:StartMovie("")
                            bVar4 = false
                            if bVar4 ~= 0 then
                            end
                            iVar14 = 4
                            pCVar15 = xStack_104
                            pCVar8 = quest:GetHero()
                            bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                            while not bVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then __region_LAB_00d50588_c1(); goto LAB_00d50594_c1 end
                                iVar14 = 4
                                pCVar8 = quest:GetHero()
                                bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                __region_LAB_00d50588_c1()
                                goto LAB_00d50594_c1
                            end
                            xStack_114 = resources:NewActorMap()
                            resources:SetActor(xStack_114, "ME", xStack_148)
                            resources:SetActor(xStack_114, "HERO", xStack_4c)
                            xStack_d8 = resources:NewResource()
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro(xStack_b4, xStack_114, false, true)
                            resources:SetActor(xStack_114, "ME", xStack_148)
                            native_arg_switch_2 = iVar13
                            repeat
                                if native_arg_switch_2 == 0 then
                                    if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if not bVar4 then
                                            resources:RunMacro(xStack_ac, xStack_114, false, true)
                                            quest:ClearThingHasInformation(me)
                                            goto FLOW_native_label_1_c1
                                        end
                                        -- TODO(native): goto LAB_00d50567_c1
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        resources:RunMacro(xStack_84, xStack_114, false, true)
                                        break
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00d50573_c1
                                else
                                    if native_arg_switch_2 == 1 then
                                        resources:RunMacro(xStack_a4, xStack_114, false, true)
                                        break
                                    else
                                        if native_arg_switch_2 == 2 then
                                            resources:RunMacro(xStack_dc, xStack_114, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 3 then
                                                resources:RunMacro(xStack_c8, xStack_114, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 4 then
                                                    resources:RunMacro(xStack_f0, xStack_114, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 5 then
                                                        resources:RunMacro(xStack_ec, xStack_114, false, true)
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 6 then
                                                            resources:RunMacro(xStack_e4, xStack_114, false, true)
                                                            break
                                                        else
                                                            goto FLOW_native_label_1_c1
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            until not (false)
                            ::FLOW_native_label_1_c1::
                            if quest:GetMasterGameState("GlobalWillGrade") < 7 - iVar13 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    -- LAB_00d50567_c1: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00d50573_c1: (native jump target)
                                    resources:DestroyMovie(xStack_5c)
                                    resources:DestroyActorMap(xStack_114)
                                    __region_LAB_00d50588_c1(); goto LAB_00d50594_c1
                                end
                                quest:SetMasterGameState("GlobalWillGrade", 7 - iVar13)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_5c)
                            resources:DestroyActorMap(xStack_114)
                            resources:DestroyMovie(xStack_104)
                        end
                        quest:SetMasterGameState("HeroTakingGuildTest", false)
                        quest:SetPlayerUsingWillDummies(false)
                        goto LAB_00d503cb_c1
                    end
                    ::LAB_00d50594_c1::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        me:ClearCommands()
                        xStack_6c = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_124 = resources:ScriptThing(xStack_148)
                        pCVar8 = xStack_124
                        r9 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            iVar16 = 0
                            iVar14 = 1
                            iVar13 = 0
                            iVar6 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_EARLY"
                            pCVar8 = quest:GetHero()
                            r10 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_6c)
                                    -- TODO(native): goto LAB_00d505a6_c1
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(xStack_d8)
                                -- TODO(native): goto LAB_00d505a6_c1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(xStack_148)
                        goto LAB_00d503cb_c1
                    end
                end
                -- LAB_00d505a6_c1: (native jump target)
                __cleanup_LAB_00d505af_c1(); return
            end
            ::LAB_00d503cb_c1::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            goto FLOW_after_lab_00d50495
        end
        bVar4 = resources:TryAcquire(xStack_148, pCVar8, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        -- LAB_00d505c1: (native jump target)
        resources:DestroyMovie(xStack_4c)
        return
    end
    uVar1 = __native_entity_state:GetStateInt("self_0xc")
    -- TODO(native): thing._4_4_ = uVar1;
    thing = nil
    -- TODO(native): thing._8_4_ = piVar2;
    quest:SetIsPushableByHero(pCVar8, (thing ~= 0))
    quest:SetThingHasInformation(pCVar8, false, true, false)
    quest:EntitySetAsKillable(pCVar8, false, true)
    r11 = quest:GetNearestWithDefName(pCVar8, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(pCVar8, r11)
    me:SetFriendsWithEverythingFlag(pCVar8)
    c_stk_131 = 0
    r12 = quest:GetThingWithScriptName("WillApprenticeTargetMarker")
    xStack_150 = quest:RegisterTimer()
    quest:SetTimer(xStack_150, 10)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    repeat
        if bVar4 then
            quest:DeregisterTimer(xStack_150)
            r12 = nil
            r11 = nil
            -- LAB_00d50495: (native jump target)
            return
        end
        bVar4 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not bVar4 then goto LAB_00d4f285 end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            __cleanup_LAB_00d504b9()
            return
        end
        iVar6 = __native_entity_state:GetStateInt("self_0x18")
        if ((3 < *(iVar6 + 0xb4)) or (3 < *(iVar6 + 0xb8))) or (3 < *(iVar6 + 0xbc)) then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if c_stk_131 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d505a6(); return end
                    quest:ClearThingHasInformation(me)
                    c_stk_131 = 0
                end
                goto LAB_00d4f285
            end
            __cleanup_LAB_00d504b9(); return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d505a6(); return end
        if c_stk_131 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d505a6(); return end
            quest:SetThingHasInformation(me, false, true, false)
            c_stk_131 = '\x01'
        end
        ::LAB_00d4f285::
        bVar4 = quest:IsDistanceBetweenThingsOver(me, xStack_3c, 4.0)
        __native_condition_3 = not bVar4
        if not __native_condition_3 then
            iVar6 = me:IsPerformingScriptTask()
            __native_condition_3 = iVar6
        end
        if __native_condition_3 then
            iVar6 = me:IsPerformingScriptTask()
            if iVar6 then goto LAB_00d4f49e end
            fVar18 = 10.0
            pCVar7 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar18)
            native_arg_sequence_1 = false
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar6 = quest:GetTimer(xStack_150)
                if 0 < iVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4f49e end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                bVar4 = false
                pCVar7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar4)
                quest:SetTimer(xStack_150, 0x14)
                iVar13 = quest:AddNewConversation(me, false, false)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar13, pCVar7)
                iVar6 = quest:GetMasterGameState("GlobalWillGrade")
                if iVar6 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", me, pCVar7, false)
                        goto LAB_00d4f49e
                    end
                else
                    if iVar6 == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", me, pCVar7, false)
                            goto LAB_00d4f49e
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", me, pCVar7, false)
                            -- LAB_00d4f499: (native jump target)
                            goto LAB_00d4f49e
                        end
                    end
                end
            end
            __cleanup_LAB_00d505a6(); return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d504b9(); return end
        if r12 == nil then
        else
            p0 = (**(*r12 + 0x18))()
        end
        me:MoveToPosition(nil --[[missing]], p0, 0x40400000, false, false)
        ::LAB_00d4f49e::
        bVar4 = me:IsTalkedToByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d504b9(); return end
            iVar6 = me:IsPerformingScriptTask()
            if not iVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        xStack_4c = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_3c = resources:ScriptThing(0)
                        pCVar8 = xStack_3c
                        r13 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar16 = 0
                            iVar14 = 1
                            iVar13 = 0
                            iVar6 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE"
                            pCVar8 = quest:GetHero()
                            r14 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_5c)
                                    __cleanup_LAB_00d505a6(); return
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_104)
                                __cleanup_LAB_00d505a6(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_104)
                        goto LAB_00d503cb
                    end
                    __cleanup_LAB_00d505a6(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                xStack_5c = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar6 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_6c)
                        __cleanup_LAB_00d505a6(); return
                    end
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00d4fa43_c6: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(xStack_d8)
                    __cleanup_LAB_00d505a6(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if iVar6 ~= 1 then
                    if not bVar4 then
                        x_stk_18 = resources:ScriptThing(0)
                        pCVar8 = x_stk_18
                        r15 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            iVar17 = 0
                            iVar16 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_RETURN"
                            pCVar8 = quest:GetHero()
                            r16 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    -- LAB_00d4fa43_c7: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(xStack_d8)
                                    __cleanup_LAB_00d505a6(); return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar5 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50542 end
                        end
                        goto LAB_00d4fb0a
                    end
                    ::LAB_00d50542::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_6c)
                    __cleanup_LAB_00d505a6(); return
                end
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_6c)
                    __cleanup_LAB_00d505a6(); return
                end
                xStack_c = resources:ScriptThing(0)
                pCVar8 = xStack_c
                r17 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_01 then
                    iVar17 = 0
                    iVar16 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO"
                    pCVar8 = quest:GetHero()
                    r18 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d4fa43_c9: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_d8)
                            __cleanup_LAB_00d505a6(); return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_6c)
                        __cleanup_LAB_00d505a6(); return
                    end
                end
                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        -- LAB_00d4fa43: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(xStack_d8)
                        __cleanup_LAB_00d505a6(); return
                    end
                    xStack_24 = resources:ScriptThing(0)
                    pCVar8 = xStack_24
                    r19 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_02 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar14 = 0
                        iVar13 = 0
                        pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS"
                        pCVar8 = quest:GetHero()
                        r20 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_6c)
                                __cleanup_LAB_00d505a6(); return
                            end
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_d8)
                            __cleanup_LAB_00d505a6(); return
                        end
                    end
                end
                ::LAB_00d4fb0a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_4c)
                if iVar6 ~= 1 then goto LAB_00d503cb end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                quest:SetPlayerUsingWillDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                bVar4 = quest:MsgOnHeroCastSpell()
                while not bVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d505a6(); return end
                    bVar4 = quest:MsgOnHeroCastSpell()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_138);
                -- TODO(native): piVar2 = DAT_0143e8f8;
                iVar13 = __ftol2()
                quest:SetTimer(xStack_150, xStack_138)
                quest:SetMasterGameState("WillScore", 0)
                quest:SetTimer(xStack_150, quest:GetStateInt("WillHelpTimer"))
                iVar6 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                x_stk_108 = quest:AddQuestInfoTimer(xStack_150, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                cVar5 = 0
                c_stk_115 = 0
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_14c);
                quest:SetTimer(xStack_150, xStack_14c)
                iVar13 = quest:GetTimer(xStack_150)
                while (0 < iVar13 and (cVar5 == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d50594 end
                    iVar6 = quest:GetTimer(xStack_150)
                    if iVar6 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        bVar4 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar4)
                        quest:SetTimer(xStack_150, 2)
                    end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        cVar5 = '\x01'
                        c_stk_115 = '\x01'
                    end
                    iVar6 = quest:GetHeroWillEnergy()
                    __native_condition_4 = iVar6 == 0
                    if __native_condition_4 then
                        iVar6 = quest:GetTimer(xStack_150)
                        __native_condition_4 = iVar6 < 1
                    end
                    if __native_condition_4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        iVar13 = quest:AddNewConversation(me, false, false)
                        pCVar8 = quest:GetHero()
                        quest:AddPersonToConversation(iVar13, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", me, pCVar8, false)
                        quest:SetTimer(xStack_150, quest:GetStateInt("WillHelpTimer"))
                        cVar5 = c_stk_115
                    end
                    fVar18 = 30.0
                    pCVar8 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsOver(pCVar8, me, fVar18)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        cVar5 = '\x01'
                        c_stk_115 = '\x01'
                    end
                    iVar6 = iVar6
                    quest:UpdateQuestInfoCounter(iVar6, quest:GetMasterGameState("WillScore"), -1)
                    iVar13 = quest:GetTimer(xStack_150)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    bVar4 = quest:IsHeroControlledByPlayer()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        bVar4 = quest:IsHeroControlledByPlayer()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d50594 end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(iVar6)
                    quest:RemoveQuestInfoElement(x_stk_108)
                    if cVar5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        x_stk_108 = quest:GetMasterGameState("WillScore")
                        -- TODO(native): pfVar11 = *(float **)(DAT_0143e90c + 0xecc);
                        iVar6 = 0
                        repeat
                            iVar13 = iVar6
                            if *pfVar11 < x_stk_108 ~= (*pfVar11 == x_stk_108) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d50594 end
                                break
                            end
                            pfVar11 = pfVar11 + 1
                            iVar6 = iVar13 + 1
                        until not (iVar13 + 1 < 7)
                        xStack_104 = resources:StartMovie("")
                        bVar4 = false
                        if bVar4 ~= 0 then
                        end
                        iVar14 = 4
                        pCVar15 = xStack_104
                        pCVar8 = quest:GetHero()
                        bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                resources:DestroyMovie(xStack_4c)
                                goto LAB_00d50594
                            end
                            iVar14 = 4
                            pCVar15 = xStack_d8
                            pCVar8 = quest:GetHero()
                            bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d50588: (native jump target)
                            resources:DestroyMovie(xStack_4c)
                            goto LAB_00d50594
                        end
                        xStack_114 = resources:NewActorMap()
                        resources:SetActor(xStack_114, "ME", 0)
                        resources:SetActor(xStack_114, "HERO", xStack_4c)
                        xStack_d8 = resources:NewResource()
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro(xStack_b4, xStack_114, false, true)
                        resources:SetActor(xStack_114, "ME", 0)
                        native_arg_switch_2 = iVar13
                        repeat
                            if native_arg_switch_2 == 0 then
                                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS", xStack_114, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00d50573_c14: (native jump target)
                                    resources:DestroyMovie(xStack_5c)
                                    resources:DestroyActorMap(xStack_114)
                                    resources:DestroyMovie(xStack_4c)
                                    goto LAB_00d50594
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_A", xStack_114, false, true)
                                    break
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_5c)
                                resources:DestroyActorMap(xStack_114)
                                resources:DestroyMovie(xStack_4c)
                                goto LAB_00d50594
                            else
                                if native_arg_switch_2 == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_B", xStack_114, false, true)
                                    break
                                else
                                    if native_arg_switch_2 == 2 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_C", xStack_114, false, true)
                                        break
                                    else
                                        if native_arg_switch_2 == 3 then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_D", xStack_114, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 4 then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_E", xStack_114, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 5 then
                                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_F", xStack_114, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 6 then
                                                        resources:RunMacro("", xStack_114, false, true)
                                                        break
                                                    else
                                                        goto FLOW_native_label_1
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        until not (false)
                        ::FLOW_native_label_1::
                        if quest:GetMasterGameState("GlobalWillGrade") < 7 - iVar13 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                -- LAB_00d50567: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00d50573: (native jump target)
                                resources:DestroyMovie(xStack_5c)
                                resources:DestroyActorMap(xStack_114)
                                resources:DestroyMovie(xStack_4c)
                                goto LAB_00d50594
                            end
                            quest:SetMasterGameState("GlobalWillGrade", 7 - iVar13)
                        end
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_5c)
                        resources:DestroyActorMap(xStack_114)
                        resources:DestroyMovie(xStack_104)
                    end
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingWillDummies(false)
                    goto LAB_00d503cb
                end
                ::LAB_00d50594::
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    me:ClearCommands()
                    xStack_6c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    r11 = resources:ScriptThing(0)
                    pCVar8 = r11
                    r21 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_0 then
                        iVar16 = 0
                        iVar14 = 1
                        iVar13 = 0
                        iVar6 = 0
                        pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_EARLY"
                        pCVar8 = quest:GetHero()
                        r22 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_6c)
                                __cleanup_LAB_00d505a6(); return
                            end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_d8)
                            __cleanup_LAB_00d505a6(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(0)
                    goto LAB_00d503cb
                end
            end
            __cleanup_LAB_00d505a6()
            return
        end
        ::LAB_00d503cb::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    until false
    ::FLOW_after_lab_00d50495::
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

