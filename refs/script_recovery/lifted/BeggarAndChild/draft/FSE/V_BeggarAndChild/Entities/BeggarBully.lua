-- Generated native draft: BeggarBully. Review coverage report before use.
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
    local __native_condition_1, aC_stk_28, au_stk_54, bVar4, bVar6, belchCount, cVar5, fVar15, friendReplySelected, hb_stk_f001, hb_stk_f002, hb_stk_f003, hb_stk_f004, hb_stk_f005, hb_stk_f006, iVar10, iVar17, iVar18, iVar19, iVar9, i_stk_154, initialReplySelected, native_arg_switch_3, native_arg_switch_4, p0, p0_01, p0_01_b3, p1, p2, pCVar7, pcVar16, pppuVar20, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, uVar11, uVar8, xStack_124, xStack_138, xStack_13c, xStack_164, x_stk_10, x_stk_18, x_stk_30, x_stk_48, x_stk_58, x_stk_60, x_stk_64
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_164 = resources:NewResource()
        local entityControl = xStack_164
        resources:PrepareResource(entityControl)
        p2 = 4
        p1 = xStack_164
        cVar5 = resources:TryAcquire(entityControl, me, 4)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e5ce43 end
            cVar5 = resources:TryAcquire(entityControl, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            uVar8 = __native_entity_state:GetStateInt("self_0xc")
            quest:SetIsPushableByHero(me, false)
            r1 = quest:GetThingWithScriptName("LookoutPointBeggar")
            friendReplySelected = false
            belchCount = 0; initialReplySelected = false
            quest:SetThingHasInformation(me, false, false, false)
            quest:EntitySetAsKillable(me, false, true)
            cVar5 = quest:GetStateBool("BeggarHit")
            bVar4 = false
            while (((not cVar5 and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and (not quest:GetStateBool("BullyLeft"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e5ce3a end
                bVar6 = me:IsTalkedToByHero()
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e5ce3a end
                    bVar4 = true
                end
                bVar6 = me:MsgIsHitByHero()
                if bVar6 then
                    goto LAB_00e5b649
                else
                    bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar6 then
                        bVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar6 then goto LAB_00e5b649 end
                    end
                    bVar6 = false
                end
                goto FLOW_past_lab_00e5b649
                ::LAB_00e5b649::
                bVar6 = true
                ::FLOW_past_lab_00e5b649::
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5ce3a end
                    quest:SetStateBool("BullyHit", true)
                else
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e5ce3a end
                        bVar4 = false
                        if (initialReplySelected) or (quest:GetStateBool("TaughtBelch")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5ce3a end
                            xStack_138 = resources:StartMovie("")
                            pCVar7 = 0x1
                            quest:PauseAllNonScriptedEntities(true)
                            if quest:GetStateBool("TaughtBelch") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then
                                    if quest:GetStateInt("BelchedAtBeggar") == 0 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5c30a end
                                        x_stk_30 = resources:ScriptThing(entityControl)
                                        iVar10 = x_stk_30
                                        fVar15 = quest:GetHealth(iVar10)
                                        hb_stk_f001 = true
                                        if not (fVar15 > 0.0) then
                                            hb_stk_f001 = false
                                        end
                                        if hb_stk_f001 then
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BULLY_FRIEND_BELCH_COMMAND"
                                            iVar9 = quest:GetHero()
                                            r2 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            if not r2 then
                                                goto LAB_00e5c30a
                                            end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar5 = iVar9
                                            while cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5c30a end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar5 = iVar9
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then goto LAB_00e5c469 end
                                            -- LAB_00e5be23: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_138)
                                            goto LAB_00e5ce3a
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5c30a end
                                        if not friendReplySelected then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e5c30a end
                                            friendReplySelected = true
                                            x_stk_18 = resources:ScriptThing(entityControl)
                                            iVar10 = x_stk_18
                                            fVar15 = quest:GetHealth(iVar10)
                                            hb_stk_f002 = true
                                            if not (fVar15 > 0.0) then
                                                hb_stk_f002 = false
                                            end
                                            if hb_stk_f002 then
                                                iVar19 = 0
                                                iVar18 = 1
                                                iVar17 = 0
                                                iVar10 = 0
                                                pcVar16 = "TEXT_QST_015_BULLY_FRIEND_TALK"
                                                iVar9 = quest:GetHero()
                                                r3 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                                if not r3 then
                                                    goto LAB_00e5c30a
                                                end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar5 = iVar9
                                                while cVar5 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e5c30a end
                                                    iVar9 = me:IsPerformingScriptTask()
                                                    cVar5 = iVar9
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5c30a end
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e5c30a end
                                            x_stk_60 = resources:ScriptThing(entityControl)
                                            iVar10 = x_stk_60
                                            fVar15 = quest:GetHealth(iVar10)
                                            hb_stk_f003 = true
                                            if not (fVar15 > 0.0) then
                                                hb_stk_f003 = false
                                            end
                                            if not hb_stk_f003 then goto LAB_00e5c469 end
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BULLY_FRIEND_SNEER_OFFER"
                                            iVar9 = quest:GetHero()
                                            r4 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            if not r4 then
                                                goto LAB_00e5c30a
                                            end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar5 = iVar9
                                            while cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5c30a end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar5 = iVar9
                                            end
                                            goto LAB_00e5c45a
                                        end
                                    end
                                    goto FLOW_hoist_lab_00e5c45a_2
                                end
                                goto FLOW_hoist_lab_00e5c45a_3
                            end
                            goto FLOW_past_lab_00e5c45a
                            ::LAB_00e5c45a::
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5c30a end
                            ::FLOW_hoist_lab_00e5c45a_2::
                            goto LAB_00e5c469
                            ::FLOW_hoist_lab_00e5c45a_3::
                            goto FLOW_hoist_lab_00e5c469_1
                            ::FLOW_past_lab_00e5c45a::
                            goto FLOW_past_lab_00e5c469
                            ::LAB_00e5c469::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_138)
                            -- LAB_00e5c48f: (native jump target)
                            iVar9 = quest:GetTimer(quest:GetStateInt("TauntTimer"))
                            quest:SetTimer(quest:GetStateInt("TauntTimer"), iVar9 + 5)
                            p1 = pppuVar20
                            goto LAB_00e5c948
                            ::FLOW_hoist_lab_00e5c469_1::
                            -- LAB_00e5ce6e: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_138)
                            goto LAB_00e5ce3a
                            ::FLOW_past_lab_00e5c469::
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                x_stk_48 = resources:ScriptThing(entityControl)
                                iVar10 = x_stk_48
                                fVar15 = quest:GetHealth(iVar10)
                                hb_stk_f004 = true
                                if not (fVar15 > 0.0) then
                                    hb_stk_f004 = false
                                end
                                if hb_stk_f004 then
                                    iVar19 = 0
                                    iVar18 = 1
                                    iVar17 = 0
                                    iVar10 = 0
                                    pcVar16 = "TEXT_QST_015_BULLY_FRIEND_TALK_20"
                                    iVar9 = quest:GetHero()
                                    r5 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                    if not r5 then
                                        goto LAB_00e5c30a
                                    end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar5 = iVar9
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5c30a end
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar5 = iVar9
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5c30a end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                while iVar9 < 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5c30a end
                                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if iVar9 == 1 then
                                        if not bVar6 then
                                            x_stk_64 = resources:ScriptThing(entityControl)
                                            iVar10 = x_stk_64
                                            fVar15 = quest:GetHealth(iVar10)
                                            hb_stk_f005 = true
                                            if not (fVar15 > 0.0) then
                                                hb_stk_f005 = false
                                            end
                                            if hb_stk_f005 then
                                                iVar19 = 0
                                                iVar18 = 1
                                                iVar17 = 0
                                                iVar10 = 0
                                                pcVar16 = "TEXT_QST_015_BULLY_TEACH_BELCH"
                                                iVar9 = quest:GetHero()
                                                r6 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                                if not r6 then
                                                    goto LAB_00e5c30a
                                                end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar5 = iVar9
                                                while cVar5 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e5c30a end
                                                    iVar9 = me:IsPerformingScriptTask()
                                                    cVar5 = iVar9
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5c30a end
                                            end
                                            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                                            if quest:GetStateBool("ExpressionTutorialShown") then
                                                goto LAB_00e5c3a8
                                            end
                                            goto FLOW_past_lab_00e5c3a8
                                            ::LAB_00e5c3a8::
                                            quest:SetStateBool("TaughtBelch", true)
                                            goto LAB_00e5c469
                                            ::FLOW_past_lab_00e5c3a8::
                                            cVar5 = quest:IsXbox()
                                            if cVar5 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if not bVar6 then
                                                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                                    cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    while not cVar5 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then goto LAB_00e5c30a end
                                                        cVar5 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then goto LAB_00e5c3a1 end
                                                end
                                                goto LAB_00e5c30a
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                                cVar5 = quest:MsgIsGameInfoClickedPast()
                                                while not cVar5 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e5c30a end
                                                    cVar5 = quest:MsgIsGameInfoClickedPast()
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if not bVar6 then
                                                    goto LAB_00e5c3a1
                                                end
                                            end
                                            goto FLOW_past_lab_00e5c3a1
                                            ::LAB_00e5c3a1::
                                            quest:SetStateBool("ExpressionTutorialShown", true)
                                            goto LAB_00e5c3a8
                                            ::FLOW_past_lab_00e5c3a1::
                                        end
                                        -- LAB_00e5ce8a: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_138)
                                        goto LAB_00e5ce3a
                                    end
                                    if not bVar6 then
                                        x_stk_58 = resources:ScriptThing(entityControl)
                                        iVar10 = x_stk_58
                                        fVar15 = quest:GetHealth(iVar10)
                                        hb_stk_f006 = true
                                        if not (fVar15 > 0.0) then
                                            hb_stk_f006 = false
                                        end
                                        if hb_stk_f006 then
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL"
                                            iVar9 = quest:GetHero()
                                            r7 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            if not r7 then
                                                goto LAB_00e5c30a
                                            end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar5 = iVar9
                                            while cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5c30a end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar5 = iVar9
                                            end
                                            goto LAB_00e5c45a
                                        end
                                        goto LAB_00e5c469
                                    end
                                end
                            end
                            ::LAB_00e5c30a::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_138)
                            goto LAB_00e5ce3a
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5ce3a end
                        initialReplySelected = true
                        xStack_124 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        au_stk_54 = resources:ScriptThing(entityControl)
                        pCVar7 = au_stk_54
                        fVar15 = quest:GetHealth(pCVar7)
                        p0_01_b3 = not (not (fVar15 > 0.0))
                        if not p0_01_b3 then
                            goto LAB_00e5b824
                        else
                            iVar19 = 0
                            iVar18 = 1
                            iVar17 = 0
                            iVar10 = 0
                            pcVar16 = "TEXT_QST_015_BULLY_FIRST_CHAT"
                            iVar9 = quest:GetHero()
                            r8 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                            if not r8 then
                                goto LAB_00e5bab8
                            end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar5 = iVar9
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5bab8 end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar5 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then goto LAB_00e5b824 end
                        end
                        goto FLOW_past_lab_00e5b824
                        ::LAB_00e5b824::
                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        pppuVar20 = p1
                        while iVar9 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5bab8 end
                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if iVar9 == 1 then
                                if not bVar6 then
                                    x_stk_10 = resources:ScriptThing(entityControl)
                                    iVar10 = x_stk_10
                                    fVar15 = quest:GetHealth(iVar10)
                                    p0_01_b3 = not (not (fVar15 > 0.0))
                                    if p0_01_b3 then
                                        iVar19 = 0
                                        iVar18 = 1
                                        iVar17 = 0
                                        iVar10 = 0
                                        pcVar16 = "TEXT_QST_015_BULLY_TEACH_BELCH"
                                        iVar9 = quest:GetHero()
                                        r9 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                        if not r9 then
                                            goto LAB_00e5bab8
                                        end
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar5 = iVar9
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e5bab8 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar5 = iVar9
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5bab8 end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                        goto LAB_00e5bb58
                                    end
                                    goto FLOW_past_lab_00e5bb58
                                    ::LAB_00e5bb58::
                                    quest:SetStateBool("TaughtBelch", true)
                                    goto LAB_00e5bc19
                                    ::FLOW_past_lab_00e5bb58::
                                    cVar5 = quest:IsXbox()
                                    if cVar5 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            cVar5 = quest:MsgIsGameInfoClickedPast()
                                            while not cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e5bab8 end
                                                cVar5 = quest:MsgIsGameInfoClickedPast()
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then goto LAB_00e5bb51 end
                                        end
                                        goto LAB_00e5bab8
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        cVar5 = quest:MsgIsGameInfoClickedPast()
                                        while not cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e5bab8 end
                                            cVar5 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            goto LAB_00e5bb51
                                        end
                                    end
                                    goto FLOW_past_lab_00e5bb51
                                    ::LAB_00e5bb51::
                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                    goto LAB_00e5bb58
                                    ::FLOW_past_lab_00e5bb51::
                                end
                                -- LAB_00e5ce57: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_124)
                                goto LAB_00e5ce3a
                            end
                            if not bVar6 then
                                aC_stk_28 = resources:ScriptThing(entityControl)
                                iVar10 = aC_stk_28
                                fVar15 = quest:GetHealth(iVar10)
                                p0_01_b3 = not (not (fVar15 > 0.0))
                                if p0_01_b3 then
                                    iVar19 = 0
                                    iVar18 = 1
                                    iVar17 = 0
                                    iVar10 = 0
                                    pcVar16 = "TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED"
                                    iVar9 = quest:GetHero()
                                    r10 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                    if not r10 then
                                        goto LAB_00e5bab8
                                    end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar5 = iVar9
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5bab8 end
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar5 = iVar9
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5bab8 end
                                end
                                goto LAB_00e5bc19
                            end
                            goto FLOW_past_lab_00e5bc19
                            ::LAB_00e5bc19::
                            if not quest:GetStateBool("QuestCardGiven") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5bab8 end
                                uVar8 = quest:GetActiveQuestName()
                                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", uVar8, false)
                                quest:SetStateBool("QuestCardGiven", true)
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_124)
                            iVar9 = quest:GetTimer(quest:GetStateInt("TauntTimer"))
                            quest:SetTimer(quest:GetStateInt("TauntTimer"), iVar9 + 5)
                            p1 = pppuVar20
                            goto LAB_00e5c948
                            ::FLOW_past_lab_00e5bc19::
                        end
                        ::FLOW_past_lab_00e5b824::
                        ::LAB_00e5bab8::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_124)
                        goto LAB_00e5ce3a
                    end
                    iVar9 = quest:GetTimer(quest:GetStateInt("TauntTimer"))
                    if 0 < iVar9 then goto LAB_00e5c948 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5ce3a end
                    uVar8 = quest:GetThingWithScriptName("LookoutPointBeggar")
                    quest:EntitySetFacingAngleTowardsThing(me, uVar8)
                    quest:SetTimer(quest:GetStateInt("TauntTimer"), 10)
                    iVar9 = quest:RegisterTimer()
                    i_stk_154 = iVar9
                    quest:SetTimer(iVar9, 2)
                    repeat
                        iVar10 = quest:GetTimer(iVar9)
                        if iVar10 < 1 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5cea1 end
                        bVar6 = me:IsTalkedToByHero()
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e5cea1 end
                            bVar4 = true
                        end
                        bVar6 = me:MsgIsHitByHero()
                        if bVar6 then
                            goto LAB_00e5c69b
                        else
                            bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if bVar6 then
                                bVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not bVar6 then goto LAB_00e5c69b end
                            end
                            bVar6 = false
                        end
                        goto FLOW_past_lab_00e5c69b
                        ::LAB_00e5c69b::
                        bVar6 = true
                        ::FLOW_past_lab_00e5c69b::
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5cea1 end
                            quest:SetStateBool("BullyHit", true)
                        end
                    until not (not bVar4)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        goto LAB_00e5cea1
                    end
                    goto FLOW_past_lab_00e5cea1
                    ::LAB_00e5cea1::
                    quest:DeregisterTimer(i_stk_154)
                    goto LAB_00e5ce3a
                    ::FLOW_past_lab_00e5cea1::
                    if not bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5cea1 end
                        iVar10 = quest:RetailRandModulo(32768)
                        native_arg_switch_3 = iVar10 % 5
                        repeat
                            if native_arg_switch_3 == 0 then
                                me:PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, true, false, false)
                                break
                            else
                                if native_arg_switch_3 == 1 then
                                    me:PlayAnimation("ST_OPINION_DISAPPROVAL_NYAH_NYAH", false, false, false, true, true, false, false)
                                    break
                                else
                                    if native_arg_switch_3 == 2 then
                                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_POINTING_AWAY_GET_OUT", false, false, false, true, true, false, false)
                                        break
                                    else
                                        if native_arg_switch_3 == 3 then
                                            me:PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST", false, false, false, true, true, false, false)
                                            break
                                        else
                                            if native_arg_switch_3 == 4 then
                                                me:PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT", false, false, false, true, true, false, false)
                                                break
                                            else
                                                goto FLOW_native_label_1
                                            end
                                        end
                                    end
                                end
                            end
                        until not (false)
                        ::FLOW_native_label_1::
                        uVar8 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(uVar8, r1)
                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BULLY_TAUNT", me, r1, false)
                        quest:Pause(0.20000000298023224)
                        quest:SetStateBool("BullyHasTaunted", true)
                        quest:SetTimer(quest:GetStateInt("TauntTimer"), 0xc)
                        quest:SetTimer(quest:GetStateInt("BeggarReplyTimer"), 5)
                    end
                    quest:DeregisterTimer(iVar9)
                end
                ::LAB_00e5c948::
                xStack_13c = me:MsgExpressionPerformedTo()
                bVar6 = xStack_13c ~= nil
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        uVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, uVar8)
                        iVar9 = quest:RegisterTimer()
                        i_stk_154 = iVar9
                        quest:SetTimer(iVar9, 2)
                        while true do
                            __native_condition_1 = not bVar4
                            if __native_condition_1 then
                                iVar10 = quest:GetTimer(iVar9)
                                __native_condition_1 = 0 < iVar10
                            end
                            if not __native_condition_1 then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5cec4 end
                            bVar6 = me:IsTalkedToByHero()
                            if bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(iVar9)
                                    goto LAB_00e5ce35
                                end
                                bVar4 = true
                            end
                            bVar6 = me:MsgIsHitByHero()
                            if bVar6 then
                                goto LAB_00e5caf0
                            else
                                bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                if bVar6 then
                                    bVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                    if not bVar6 then goto LAB_00e5caf0 end
                                end
                                bVar6 = false
                            end
                            goto FLOW_past_lab_00e5caf0
                            ::LAB_00e5caf0::
                            bVar6 = true
                            ::FLOW_past_lab_00e5caf0::
                            if bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    goto LAB_00e5cec4
                                end
                                goto FLOW_hoist_lab_00e5cec4_1
                            end
                            goto FLOW_past_lab_00e5cec4
                            ::LAB_00e5cec4::
                            quest:DeregisterTimer(iVar9)
                            goto LAB_00e5ce35
                            ::FLOW_hoist_lab_00e5cec4_1::
                            quest:SetStateBool("BullyHit", true)
                            ::FLOW_past_lab_00e5cec4::
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            if bVar4 then goto FLOW_native_label_2 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5cedc end
                            if xStack_13c == nil then
                                bVar6 = false
                                bVar6 = false
                                if not bVar6 then goto FLOW_native_label_2 end
                                goto LAB_00e5cd44
                            else
                                iVar9 = ((xStack_13c == "EXPRESSION_BELCH") and 0 or 1)
                                if iVar9 ~= 0 then
                                    iVar9 = ((xStack_13c == "EXPRESSION_FART") and 0 or 1)
                                    if iVar9 == 0 then goto LAB_00e5cd44 end
                                    goto FLOW_native_label_2
                                end
                                -- LAB_00e5cbfe: (native jump target)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5cedc end
                                belchCount = belchCount + 1
                                uVar8 = quest:AddNewConversation(me, false, false)
                                uVar11 = quest:GetHero()
                                quest:AddPersonToConversation(uVar8, uVar11)
                                native_arg_switch_4 = belchCount
                                repeat
                                    if native_arg_switch_4 == 1 then
                                        uVar11 = quest:GetHero()
                                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BULLY_FIRST_BELCH", me, uVar11, false)
                                        break
                                    else
                                        if native_arg_switch_4 == 2 then
                                            uVar11 = quest:GetHero()
                                            quest:AddLineToConversation(uVar8, "TEXT_QST_015_BULLY_SECOND_BELCH", me, uVar11, false)
                                            break
                                        else
                                            if native_arg_switch_4 == 3 then
                                                uVar11 = quest:GetHero()
                                                quest:AddLineToConversation(uVar8, "TEXT_QST_015_BULLY_THIRD_BELCH", me, uVar11, false)
                                                break
                                            else
                                                if native_arg_switch_4 == 4 then
                                                    quest:SetStateBool("BelchedAtBully", true)
                                                    goto LAB_00e5cd53
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            end
                            goto FLOW_past_lab_00e5cd44
                            ::LAB_00e5cd44::
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5cedc end
                            ::LAB_00e5cd53::
                            quest:SetStateBool("BullyLeft", true)
                            ::FLOW_past_lab_00e5cd44::
                            ::FLOW_native_label_2::
                            quest:DeregisterTimer(i_stk_154)
                            goto LAB_00e5cd74
                        end
                        ::LAB_00e5cedc::
                        quest:DeregisterTimer(i_stk_154)
                    end
                    goto LAB_00e5ce35
                end
                ::LAB_00e5cd74::
                cVar5 = quest:GetStateBool("BeggarHit")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                resources:PrepareResource(entityControl)
                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, -1)
                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 2, -1)
                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 2, -1)
            end
            ::LAB_00e5ce35::
            ::LAB_00e5ce3a::
        end
        ::LAB_00e5ce43::
        resources:ReleaseResource(entityControl)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

