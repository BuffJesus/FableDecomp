-- Generated native draft: MansLover. Review coverage report before use.
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
    local __native_condition_1, bVar2, bVar4, cVar3, c_stk_10d, fVar1, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, iVar10, iVar11, iVar5, iVar9, native_arg_sequence_1, p0, pCVar6, pcVar8, r1, r10, r11, r12, r13, r14, r2, r3, r4, r5, r6, r7, r8, r9, uVar7, xStack_10c, xStack_120, xStack_124, xStack_128, xStack_12c, xStack_bc, xStack_cc, xStack_d0, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_6c, x_stk_78, x_stk_84, x_stk_90, x_stk_9c, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_120 = resources:NewResource()
        resources:PrepareResource(xStack_120)
        bVar2 = resources:TryAcquire(xStack_120, me, 4)
        while not bVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                resources:ReleaseResource(xStack_120)
                return
            end
            bVar2 = resources:TryAcquire(xStack_120, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00ec9e0a: (native jump target)
            resources:ReleaseResource(xStack_120)
            return
        end
        if quest:GetStateInt("MansLoverState") == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                resources:ReleaseResource(xStack_120)
                return
            end
            quest:SetThingHasInformation(me, true, true, false)
        end
        c_stk_10d = 0
        xStack_124 = quest:ReadGlobalGameDataString(0x73c)
        xStack_128 = xStack_124
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while not bVar2 do
            xStack_12c = ""
            cVar3 = me:MsgIsPresentedWithItem()
            if cVar3 then xStack_12c = _G.g_PresentedItemName end
            native_arg_sequence_1 = false
            if cVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                if xStack_128 == xStack_12c then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    if xStack_128 ~= nil then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        if xStack_12c ~= nil then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        if xStack_128[1] == xStack_12c[1] then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        -- TODO(native): iVar5 = CBasicString<char>::Compare((void *)*xStack_128,(void *)*xStack_12c);
                        if iVar5 == 0 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                end
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                c_stk_10d = 1
            end
            cVar3 = me:IsTalkedToByHero()
            if (not cVar3) and (c_stk_10d == 0) then
                bVar2 = false
            else
                bVar2 = true
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                xStack_10c = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if c_stk_10d == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10c)
                        break
                    end
                    quest:FixMovieSequenceCamera(true)
                    bVar2 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar2)
                    quest:Pause(1.0)
                    bVar2 = true
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar2)
                    alive = quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10c)
                        break
                    end
                    quest:FixMovieSequenceCamera(true)
                    bVar2 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar2)
                    quest:Pause(1.0)
                    bVar2 = true
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar2)
                    alive = quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                end
                xStack_bc = quest:ReadGlobalGameDataString(0x73c)
                pCVar6 = quest:GetHero()
                bVar2 = quest:IsObjectInThingsPossession(xStack_bc, pCVar6)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar2 then
                    if bVar4 then goto LAB_00ecaf49 end
                    if c_stk_10d == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            x_stk_24 = resources:ScriptThing(xStack_120)
                            pCVar6 = x_stk_24
                            fret_0 = quest:GetHealth(pCVar6)
                            fVar1 = 0.0
                            if fVar1 < fret_0 then
                                iVar11 = 0
                                iVar10 = 1
                                iVar9 = 0
                                iVar5 = 0
                                pcVar8 = "TEXT_QST_B10_MANS_LOVER_INTRO"
                                pCVar6 = quest:GetHero()
                                r1 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar3 = iVar5
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00ecaf49 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00eca325 end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_GIVE_LETTER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00ecaf49 end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if iVar5 ~= 1 then
                                if bVar2 then goto LAB_00ecaf49 end
                                quest:CameraUseCameraPoint(me, pCVar6, -1.0, 0, -1)
                                x_stk_c = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_c
                                fret_00 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fret_00 <= fVar1 then goto LAB_00ecae14 end
                                iVar11 = 0
                                iVar10 = 1
                                iVar9 = 0
                                iVar5 = 0
                                pcVar8 = "TEXT_QST_B10_MANS_LOVER_MAN_NOT_SEEN_10"
                                pCVar6 = quest:GetHero()
                                r2 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar3 = iVar5
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00eca325 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                end
                                goto LAB_00ecae05
                            end
                            if not bVar2 then goto LAB_00eca2ed end
                            goto LAB_00ecaf49
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            goto LAB_00eca2ed
                        end
                    end
                    goto FLOW_past_lab_00eca2ed
                    ::LAB_00eca2ed::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        if c_stk_10d == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            quest:CameraUseCameraPoint(me, pCVar6, -1.0, 0, -1)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00ecaf49 end
                            c_stk_10d = 0
                        end
                        xStack_d0 = quest:ReadGlobalGameDataString(0x73c)
                        quest:TakeObjectFromHero(xStack_d0)
                        x_stk_90 = resources:ScriptThing(xStack_120)
                        pCVar6 = x_stk_90
                        fret_01 = quest:GetHealth(pCVar6)
                        fVar1 = 0.0
                        if fVar1 < fret_01 then
                            iVar11 = 0
                            iVar10 = 1
                            iVar9 = 0
                            iVar5 = 0
                            pcVar8 = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_10"
                            pCVar6 = quest:GetHero()
                            r3 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00ecaf49 end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar3 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MANS_LOVER_LETTER_FROM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar5 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00ecaf49 end
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            quest:CameraUseCameraPoint(me, pCVar6, -1.0, 0, -1)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if iVar5 == 0 then
                                if bVar2 then goto LAB_00ecaf49 end
                                x_stk_48 = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_48
                                fret_02 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_02 then
                                    iVar11 = 0
                                    iVar10 = 1
                                    iVar9 = 0
                                    iVar5 = 0
                                    pcVar8 = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_10"
                                    pCVar6 = quest:GetHero()
                                    r4 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00eca325 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00ecaf49 end
                                end
                                x_stk_78 = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_78
                                fret_03 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_03 then
                                    iVar11 = 0
                                    iVar10 = 1
                                    iVar9 = 0
                                    iVar5 = 0
                                    pcVar8 = "TEXT_QST_B10_MANS_LOVER_MAN_LIKES_ME_20"
                                    pCVar6 = quest:GetHero()
                                    r5 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00eca325 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00ecaf49 end
                                end
                                xStack_cc = quest:ReadGlobalGameDataString(0x740)
                                quest:GiveHeroObject(xStack_cc, -1, false)
                                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x758))
                                quest:SetStateInt("MansLoverState", 2)
                            else
                                if bVar2 then goto LAB_00eca325 end
                                x_stk_18 = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_18
                                fret_04 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_04 then
                                    iVar11 = 0
                                    iVar10 = 1
                                    iVar9 = 0
                                    iVar5 = 0
                                    pcVar8 = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_20"
                                    pCVar6 = quest:GetHero()
                                    r6 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00ecaf49 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00eca325 end
                                end
                                x_stk_60 = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_60
                                fret_05 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_05 then
                                    iVar11 = 0
                                    iVar10 = 1
                                    iVar9 = 0
                                    iVar5 = 0
                                    pcVar8 = "TEXT_QST_B10_MANS_LOVER_FROM_YOU_30"
                                    pCVar6 = quest:GetHero()
                                    r7 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00ecaf49 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00eca325 end
                                end
                                quest:GiveHeroObject("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", -1, false)
                                x_stk_30 = resources:ScriptThing(xStack_120)
                                pCVar6 = x_stk_30
                                fret_06 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_06 then
                                    iVar11 = 0
                                    iVar10 = 1
                                    iVar9 = 0
                                    iVar5 = 0
                                    pcVar8 = "TEXT_QST_B10_MANS_LOVER_LIKES_YOU_10"
                                    pCVar6 = quest:GetHero()
                                    r8 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar3 = iVar5
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00ecaf49 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar3 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00eca325 end
                                end
                                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x75c))
                                quest:SetStateInt("MansLoverState", 1)
                            end
                            quest:ClearThingHasInformation(me)
                            __native_entity_state:SetStateBool("LeavingRegion", true)
                            goto LAB_00ecae14
                        end
                    end
                    ::FLOW_past_lab_00eca2ed::
                    goto LAB_00eca325
                end
                goto FLOW_past_lab_00eca325
                ::LAB_00eca325::
                -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
                resources:DestroyMovie(xStack_10c)
                break
                ::FLOW_past_lab_00eca325::
                if bVar4 then goto LAB_00ecaf49 end
                me:ClearCommands()
                if not __native_entity_state:GetStateBool("LeavingRegion") then
                    goto LAB_00ecab59
                else
                    uVar7 = math.random(0, 32767)
                    uVar7 = uVar7 & 0x80000001
                    bVar2 = uVar7 == 0
                    if uVar7 < 0 then
                        bVar2 = (uVar7 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not bVar2 then goto LAB_00ecab59 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00eca325 end
                    x_stk_9c = resources:ScriptThing(xStack_120)
                    pCVar6 = x_stk_9c
                    fret_07 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_07 then
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar5 = 0
                        pcVar8 = "TEXT_QST_B10_MANS_LOVER_LEAVING_REGION_10"
                        pCVar6 = quest:GetHero()
                        r9 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00ecaf49 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00eca325 end
                    end
                end
                goto FLOW_past_lab_00ecab59
                ::LAB_00ecab59::
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    goto LAB_00ecaf49
                end
                goto FLOW_hoist_lab_00ecaf49_1
                ::FLOW_past_lab_00ecab59::
                goto FLOW_past_lab_00ecaf49
                ::LAB_00ecaf49::
                -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
                resources:DestroyMovie(xStack_10c)
                break
                ::FLOW_hoist_lab_00ecaf49_1::
                iVar5 = quest:GetStateInt("MansLoverState")
                if iVar5 == 0 then
                    x_stk_3c = resources:ScriptThing(xStack_120)
                    pCVar6 = x_stk_3c
                    fret_11 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_11 then
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar5 = 0
                        pcVar8 = "TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER"
                        pCVar6 = quest:GetHero()
                        r10 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        goto LAB_00ecae05
                    end
                elseif iVar5 then
                    x_stk_54 = resources:ScriptThing(xStack_120)
                    pCVar6 = x_stk_54
                    fret_10 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_10 then
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar5 = 0
                        pcVar8 = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                        pCVar6 = quest:GetHero()
                        r11 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        goto LAB_00ecae05
                    end
                elseif iVar5 == 2 then
                    x_stk_6c = resources:ScriptThing(xStack_120)
                    pCVar6 = x_stk_6c
                    fret_09 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_09 then
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar5 = 0
                        pcVar8 = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN"
                        pCVar6 = quest:GetHero()
                        r12 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        goto LAB_00ecae05
                    end
                else
                    x_stk_84 = resources:ScriptThing(xStack_120)
                    pCVar6 = x_stk_84
                    fret_08 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_08 then
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar5 = 0
                        pcVar8 = "TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10"
                        pCVar6 = quest:GetHero()
                        r13 = me:Speak(pCVar6, pcVar8, iVar5, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar3 = iVar5
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eca325 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar3 = iVar5
                        end
                        goto LAB_00ecae05
                    end
                end
                goto FLOW_past_lab_00ecae05
                ::LAB_00ecae05::
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00ecaf49 end
                ::FLOW_past_lab_00ecae05::
                ::FLOW_past_lab_00ecaf49::
                ::LAB_00ecae14::
                quest:FixMovieSequenceCamera(false)
                -- TODO(native): (**(code **)(*(int *)xStack_124 + 0x5ec))((void *)xStack_124,false);
                resources:DestroyMovie(xStack_10c)
            end
            __native_condition_1 = __native_entity_state:GetStateBool("LeavingRegion")
            if __native_condition_1 then
                iVar5 = me:IsPerformingScriptTask()
                __native_condition_1 = not iVar5
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                r14 = quest:GetThingWithScriptName("MansLoverLeavesHere")
                bVar2 = quest:IsDistanceBetweenThingsOver(me, r14, 2.0)
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        goto LAB_00ecaf62
                    end
                    goto FLOW_hoist_lab_00ecaf62_1
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00ecaf62 end
                    quest:FadeOutAndKillEntity(me, true, 1.5, true)
                end
                goto FLOW_past_lab_00ecaf62
                ::LAB_00ecaf62::
                break
                ::FLOW_hoist_lab_00ecaf62_1::
                me:MoveToThing(r14, 1.0, 0)
                ::FLOW_past_lab_00ecaf62::
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
        resources:ReleaseResource(xStack_120)
    end
end

function Init(quest, me)
    quest:SetThingPersistent(me, true)
    __native_entity_state:SetStateBool("LeavingRegion", false)
end

function OnPersist(quest, me, context)
    local leavingRegion = quest:GetStateBool("LeavingRegion") or false
    leavingRegion = quest:PersistTransferBool(context, "LeavingRegion", leavingRegion)
    quest:SetStateBool("LeavingRegion", leavingRegion)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetStateInt("MansLoverState", 3)
    end
end

