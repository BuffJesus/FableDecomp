-- Generated native draft: LookoutPointBeggar. Review coverage report before use.
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
    local __native_condition_1, aC_stk_28, au_stk_3c, bVar3, bVar5, cVar4, c_stk_1f5, fVar15, fVar2, iVar10, iVar17, iVar18, iVar19, iVar9, i_stk_1fc, native_arg_sequence_1, native_arg_sequence_2, native_arg_switch_3, native_arg_switch_4, p0, p0_00, p1, p2, pCVar6, pcVar16, piVar1, r1, r10, r11, r12, r13, r14, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar11, uVar13, uVar7, uVar8, xStack_19c, xStack_1b0, xStack_1c0, xStack_1d0, xStack_1e0, xStack_1e4, xStack_1e8, xStack_1fc, xStack_210, xStack_9c, xStack_a8, xStack_c0, x_stk_1c, x_stk_28, x_stk_4, x_stk_40, x_stk_48, x_stk_60, x_stk_64, x_stk_6c, x_stk_90
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_210 = resources:NewResource()
        resources:PrepareResource(xStack_210)
        p2 = 4
        p1 = xStack_210
        cVar4 = resources:TryAcquire(xStack_210, me, 4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e5b1a4 end
            cVar4 = resources:TryAcquire(xStack_210, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            uVar8 = __native_entity_state:GetStateInt("self_0xc")
            r1 = quest:GetThingWithScriptName("BeggarBully")
            quest:SetThingHasInformation(me, false, false, false)
            quest:EntitySetAsKillable(me, false, true)
            cVar4 = quest:GetStateBool("BeggarHit")
            bVar3 = false
            while (((not cVar4 and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and (not quest:GetStateBool("BullyLeft"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e5b19b end
                cVar4 = me:IsTalkedToByHero()
                if cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e5b19b end
                    bVar3 = true
                end
                uVar7 = 0
                -- TODO(native): xStack_1e4 = xStack_1e4 | 1;
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00e58fce
                else
                    bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar5 then
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00e58fce end
                    end
                    bVar5 = false
                end
                goto FLOW_past_lab_00e58fce
                ::LAB_00e58fce::
                bVar5 = true
                ::FLOW_past_lab_00e58fce::
                    -- TODO(native): xStack_1e4 = uVar13 & 0xfffffffe;
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e5b19b end
                    quest:SetStateBool("BeggarHit", true)
                else
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e5b19b end
                        bVar3 = false
                        if (c_stk_1f5 == 0) and (not quest:GetStateBool("TaughtBattleCry")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b19b end
                            c_stk_1f5 = 1
                            if quest:GetStateInt("BelchedAtBeggar") == 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b19b end
                                xStack_1c0 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                au_stk_3c = resources:ScriptThing(r1)
                                pCVar6 = au_stk_3c
                                fVar15 = quest:GetHealth(pCVar6)
                                fVar2 = 0.0
                                if fVar2 < fVar15 then
                                    iVar19 = 0
                                    iVar18 = 1
                                    iVar17 = 0
                                    iVar10 = 0
                                    pcVar16 = "TEXT_QST_015_BEGGAR_FIRST_SPEAK"
                                    iVar9 = quest:GetHero()
                                    r2 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar4 = iVar9
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e59935 end
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar4 = iVar9
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then goto LAB_00e596a9 end
                                else
                                    goto LAB_00e596a9
                                end
                                goto FLOW_past_lab_00e596a9
                                ::LAB_00e596a9::
                                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                while iVar9 < 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e59935 end
                                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if iVar9 == 1 then
                                        if not bVar5 then
                                            aC_stk_28 = resources:ScriptThing(0x0)
                                            iVar10 = aC_stk_28
                                            fVar15 = quest:GetHealth(iVar10)
                                            fVar2 = 0.0
                                            if fVar2 < fVar15 then
                                                iVar19 = 0
                                                iVar18 = 1
                                                iVar17 = 0
                                                iVar10 = 0
                                                pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                                iVar9 = quest:GetHero()
                                                r3 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar4 = iVar9
                                                while cVar4 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e59935 end
                                                    iVar9 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar9
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e59935 end
                                            end
                                            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                            if quest:GetStateBool("ExpressionTutorialShown") then
                                                goto LAB_00e599d8
                                            end
                                            goto FLOW_past_lab_00e599d8
                                            ::LAB_00e599d8::
                                            quest:SetStateBool("TaughtBattleCry", true)
                                            goto LAB_00e59a93
                                            ::FLOW_past_lab_00e599d8::
                                            cVar4 = quest:IsXbox()
                                            if cVar4 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if not bVar5 then
                                                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                                    cVar4 = quest:MsgIsGameInfoClickedPast()
                                                    while not cVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then goto LAB_00e59935 end
                                                        cVar4 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if not bVar5 then goto LAB_00e599d1 end
                                                end
                                                goto LAB_00e59935
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if not bVar5 then
                                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                                while not cVar4 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e59935 end
                                                    cVar4 = quest:MsgIsGameInfoClickedPast()
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if not bVar5 then
                                                    goto LAB_00e599d1
                                                end
                                            end
                                            goto FLOW_past_lab_00e599d1
                                            ::LAB_00e599d1::
                                            quest:SetStateBool("ExpressionTutorialShown", true)
                                            goto LAB_00e599d8
                                            ::FLOW_past_lab_00e599d1::
                                        end
                                        -- LAB_00e5b1cf: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1b0)
                                        goto LAB_00e5b19b
                                    end
                                    if not bVar5 then
                                        x_stk_1c = resources:ScriptThing(0x0)
                                        iVar10 = x_stk_1c
                                        fVar15 = quest:GetHealth(iVar10)
                                        fVar2 = 0.0
                                        if fVar2 < fVar15 then
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_REFUSED"
                                            iVar9 = quest:GetHero()
                                            r4 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar4 = iVar9
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e59935 end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar4 = iVar9
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e59935 end
                                        end
                                        goto LAB_00e59a93
                                    end
                                    goto FLOW_past_lab_00e59a93
                                    ::LAB_00e59a93::
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e59aa3
                                    ::FLOW_past_lab_00e59a93::
                                end
                                ::FLOW_past_lab_00e596a9::
                                ::LAB_00e59935::
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1b0)
                                goto LAB_00e5b19b
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b19b end
                            xStack_1b0 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_6c = resources:ScriptThing(r1)
                            pCVar6 = x_stk_6c
                            fVar15 = quest:GetHealth(pCVar6)
                            fVar2 = 0.0
                            if fVar2 < fVar15 then
                                iVar19 = 0
                                iVar18 = 1
                                iVar17 = 0
                                iVar10 = 0
                                pcVar16 = "TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH"
                                iVar9 = quest:GetHero()
                                r5 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar4 = iVar9
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e5b1b8 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar4 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then goto LAB_00e591a3 end
                                goto LAB_00e59429
                            end
                            goto FLOW_past_lab_00e59429
                            ::LAB_00e59429::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_19c)
                            goto LAB_00e5b19b
                            ::FLOW_past_lab_00e59429::
                            ::LAB_00e591a3::
                            quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar9 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b1b8 end
                                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e59429 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if iVar9 == 1 then
                                if not bVar5 then
                                    x_stk_40 = resources:ScriptThing(xStack_1fc)
                                    iVar10 = x_stk_40
                                    fVar15 = quest:GetHealth(iVar10)
                                    fVar2 = 0.0
                                    if fVar2 < fVar15 then
                                        iVar19 = 0
                                        iVar18 = 1
                                        iVar17 = 0
                                        iVar10 = 0
                                        pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                        iVar9 = quest:GetHero()
                                        r6 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar4 = iVar9
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e59429 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar4 = iVar9
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e5b1b8 end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                        goto LAB_00e594c9
                                    end
                                    goto FLOW_past_lab_00e594c9
                                    ::LAB_00e594c9::
                                    quest:SetStateBool("TaughtBattleCry", true)
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e59aa3
                                    ::FLOW_past_lab_00e594c9::
                                    cVar4 = quest:IsXbox()
                                    if cVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                            while not cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e5b1b8 end
                                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if not bVar5 then goto LAB_00e594c2 end
                                        end
                                        goto LAB_00e59429
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        cVar4 = quest:MsgIsGameInfoClickedPast()
                                        while not cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e59429 end
                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            goto LAB_00e594c2
                                        end
                                    end
                                    goto FLOW_past_lab_00e594c2
                                    ::LAB_00e594c2::
                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                    goto LAB_00e594c9
                                    ::FLOW_past_lab_00e594c2::
                                end
                                goto LAB_00e5b1b8
                            end
                            goto FLOW_past_lab_00e5b1b8
                            ::LAB_00e5b1b8::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_19c)
                            goto LAB_00e5b19b
                            ::FLOW_past_lab_00e5b1b8::
                            if bVar5 then goto LAB_00e59429 end
                            x_stk_28 = resources:ScriptThing(xStack_1fc)
                            iVar10 = x_stk_28
                            fVar15 = quest:GetHealth(iVar10)
                            fVar2 = 0.0
                            if fVar2 < fVar15 then
                                iVar19 = 0
                                iVar18 = 1
                                iVar17 = 0
                                iVar10 = 0
                                pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_REFUSED"
                                iVar9 = quest:GetHero()
                                r7 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar4 = iVar9
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e5b1b8 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar4 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e59429 end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            ::LAB_00e59aa3::
                            resources:DestroyMovie(this_00)
                            if not quest:GetStateBool("QuestCardGiven") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b19b end
                                uVar8 = quest:GetActiveQuestName()
                                quest:GiveHeroQuestCardDirectly(uVar8, "OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", xStack_c0)
                                quest:SetStateBool("QuestCardGiven", true)
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b19b end
                            if quest:GetStateInt("BelchedAtBeggar") ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b19b end
                                xStack_1e0 = resources:StartMovie("")
                                pCVar6 = 0x1
                                quest:PauseAllNonScriptedEntities(true)
                                if not quest:GetStateBool("TaughtBattleCry") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        x_stk_48 = resources:ScriptThing(xStack_1fc)
                                        iVar10 = x_stk_48
                                        fVar15 = quest:GetHealth(iVar10)
                                        fVar2 = 0.0
                                        if fVar2 < fVar15 then
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN"
                                            iVar9 = quest:GetHero()
                                            r8 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar4 = iVar9
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e5b1e6 end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar4 = iVar9
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e5b1fc end
                                        end
                                        pCVar6 = 0x1
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                        iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while iVar9 < 0 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e5a527 end
                                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if iVar9 == 1 then
                                                if not bVar5 then
                                                    x_stk_64 = resources:ScriptThing(0x0)
                                                    iVar10 = x_stk_64
                                                    fVar15 = quest:GetHealth(iVar10)
                                                    fVar2 = 0.0
                                                    if fVar2 < fVar15 then
                                                        iVar19 = 0
                                                        iVar18 = 1
                                                        iVar17 = 0
                                                        iVar10 = 0
                                                        pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                                        iVar9 = quest:GetHero()
                                                        r9 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                                        iVar9 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar9
                                                        while cVar4 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar5 = not alive
                                                            if bVar5 then goto LAB_00e5a527 end
                                                            iVar9 = me:IsPerformingScriptTask()
                                                            cVar4 = iVar9
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then goto LAB_00e5a527 end
                                                    end
                                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                                        goto LAB_00e5a06d
                                                    end
                                                    goto FLOW_past_lab_00e5a06d
                                                    ::LAB_00e5a06d::
                                                    quest:SetStateBool("TaughtBattleCry", true)
                                                    goto LAB_00e5a126
                                                    ::FLOW_past_lab_00e5a06d::
                                                    cVar4 = quest:IsXbox()
                                                    if cVar4 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if not bVar5 then
                                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                                            while not cVar4 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar5 = not alive
                                                                if bVar5 then goto LAB_00e5a527 end
                                                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar5 = not alive
                                                            if not bVar5 then goto LAB_00e5a066 end
                                                        end
                                                        goto LAB_00e5a527
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if not bVar5 then
                                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                                        cVar4 = quest:MsgIsGameInfoClickedPast()
                                                        while not cVar4 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar5 = not alive
                                                            if bVar5 then goto LAB_00e5a527 end
                                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if not bVar5 then
                                                            goto LAB_00e5a066
                                                        end
                                                    end
                                                    goto FLOW_past_lab_00e5a066
                                                    ::LAB_00e5a066::
                                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                                    goto LAB_00e5a06d
                                                    ::FLOW_past_lab_00e5a066::
                                                end
                                                -- LAB_00e59c7a: (native jump target)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(xStack_1d0)
                                                goto LAB_00e5b19b
                                            end
                                            if not bVar5 then
                                                x_stk_4 = resources:ScriptThing(0x0)
                                                iVar10 = x_stk_4
                                                fVar15 = quest:GetHealth(iVar10)
                                                fVar2 = 0.0
                                                if fVar2 < fVar15 then
                                                    iVar19 = 0
                                                    iVar18 = 1
                                                    iVar17 = 0
                                                    iVar10 = 0
                                                    pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH"
                                                    iVar9 = quest:GetHero()
                                                    r10 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                                    iVar9 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar9
                                                    while cVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then goto LAB_00e5a527 end
                                                        iVar9 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar9
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e5a527 end
                                                end
                                                goto LAB_00e5a126
                                            end
                                            goto FLOW_past_lab_00e5a126
                                            ::LAB_00e5a126::
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_1e0)
                                            goto LAB_00e5a6a9
                                            ::FLOW_past_lab_00e5a126::
                                        end
                                        -- LAB_00e59fcd: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1d0)
                                        goto LAB_00e5b19b
                                    end
                                    ::LAB_00e5b1fc::
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1e0)
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        x_stk_90 = resources:ScriptThing(xStack_1fc)
                                        pCVar6 = x_stk_90
                                        fVar15 = quest:GetHealth(pCVar6)
                                        fVar2 = 0.0
                                        if fVar2 < fVar15 then
                                            iVar19 = 0
                                            iVar18 = 1
                                            iVar17 = 0
                                            iVar10 = 0
                                            pcVar16 = "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY"
                                            iVar9 = quest:GetHero()
                                            r11 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar4 = iVar9
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e5a527 end
                                                iVar9 = me:IsPerformingScriptTask()
                                                cVar4 = iVar9
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e5a527 end
                                        end
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1e0)
                                        goto LAB_00e5a6a9
                                    end
                                    goto LAB_00e5b1e6
                                end
                                goto FLOW_past_lab_00e5b1e6
                                ::LAB_00e5b1e6::
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1e0)
                                ::FLOW_past_lab_00e5b1e6::
                                goto LAB_00e5b19b
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b19b end
                            xStack_1d0 = resources:StartMovie("")
                            pCVar6 = 0x1
                            quest:PauseAllNonScriptedEntities(true)
                            if not quest:GetStateBool("TaughtBattleCry") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    goto LAB_00e5a527
                                end
                                goto FLOW_hoist_lab_00e5a527_1
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1d0)
                                    goto LAB_00e5b19b
                                end
                                x_stk_60 = resources:ScriptThing(r1)
                                iVar10 = x_stk_60
                                fVar15 = quest:GetHealth(iVar10)
                                fVar2 = 0.0
                                if fVar15 <= fVar2 then goto LAB_00e5a683 end
                                iVar19 = 0
                                iVar18 = 1
                                iVar17 = 0
                                iVar10 = 0
                                pcVar16 = "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY"
                                iVar9 = quest:GetHero()
                                r12 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar4 = iVar9
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e5a527 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar4 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1c0)
                                    goto LAB_00e5b19b
                                end
                            end
                            goto FLOW_past_lab_00e5a527
                            ::LAB_00e5a527::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1d0)
                            goto LAB_00e5b19b
                            ::FLOW_hoist_lab_00e5a527_1::
                            quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar9 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b231 end
                                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5a527 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if iVar9 == 1 then
                                if not bVar5 then
                                    xStack_a8 = resources:ScriptThing(xStack_1fc)
                                    iVar10 = xStack_a8
                                    fVar15 = quest:GetHealth(iVar10)
                                    fVar2 = 0.0
                                    if fVar2 < fVar15 then
                                        iVar19 = 0
                                        iVar18 = 1
                                        iVar17 = 0
                                        iVar10 = 0
                                        pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                        iVar9 = quest:GetHero()
                                        r13 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar4 = iVar9
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e5a527 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar4 = iVar9
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e5b231 end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                        goto LAB_00e5a5c8
                                    end
                                    goto FLOW_past_lab_00e5a5c8
                                    ::LAB_00e5a5c8::
                                    quest:SetStateBool("TaughtBattleCry", true)
                                    goto LAB_00e5a683
                                    ::FLOW_past_lab_00e5a5c8::
                                    cVar4 = quest:IsXbox()
                                    if cVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                            while not cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e5b231 end
                                                cVar4 = quest:MsgIsGameInfoClickedPast()
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if not bVar5 then goto LAB_00e5a5c1 end
                                        end
                                        goto LAB_00e5a527
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        cVar4 = quest:MsgIsGameInfoClickedPast()
                                        while not cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e5a527 end
                                            cVar4 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            goto LAB_00e5a5c1
                                        end
                                    end
                                    goto FLOW_past_lab_00e5a5c1
                                    ::LAB_00e5a5c1::
                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                    goto LAB_00e5a5c8
                                    ::FLOW_past_lab_00e5a5c1::
                                end
                                goto LAB_00e5b231
                            end
                            goto FLOW_past_lab_00e5b231
                            ::LAB_00e5b231::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1d0)
                            goto LAB_00e5b19b
                            ::FLOW_past_lab_00e5b231::
                            if bVar5 then goto LAB_00e5a527 end
                            xStack_9c = resources:ScriptThing(r1)
                            iVar10 = xStack_9c
                            fVar15 = quest:GetHealth(iVar10)
                            fVar2 = 0.0
                            if fVar2 < fVar15 then
                                iVar19 = 0
                                iVar18 = 1
                                iVar17 = 0
                                iVar10 = 0
                                pcVar16 = "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL"
                                iVar9 = quest:GetHero()
                                r14 = me:Speak(iVar9, pcVar16, iVar10, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar4 = iVar9
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e5b231 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar4 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5a527 end
                            end
                            ::FLOW_past_lab_00e5a527::
                            ::LAB_00e5a683::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1d0)
                        end
                        ::LAB_00e5a6a9::
                        iVar9 = quest:GetTimer(quest:GetStateInt("TauntTimer"))
                        quest:SetTimer(quest:GetStateInt("TauntTimer"), iVar9 + 5)
                        goto LAB_00e5ac46
                    end
                    native_arg_sequence_1 = false
                    if not quest:GetStateBool("BullyHasTaunted") then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if not native_arg_sequence_1 then
                        iVar9 = quest:GetTimer(quest:GetStateInt("BeggarReplyTimer"))
                        if 0 < iVar9 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00e5ac46 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e5b19b end
                    uVar8 = quest:GetThingWithScriptName("BeggarBully")
                    quest:EntitySetFacingAngleTowardsThing(uVar8, r1)
                    quest:SetStateBool("BullyHasTaunted", false)
                    iVar9 = quest:RegisterTimer()
                    i_stk_1fc = iVar9
                    quest:SetTimer(iVar9, 2)
                    repeat
                        iVar10 = quest:GetTimer(iVar9)
                        if iVar10 < 1 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e5b24b end
                        bVar5 = quest:MsgOnQuestFailed("SCRIPT_NAME_HERO")
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e5b24b end
                            bVar3 = true
                        end
                        -- TODO(native): xStack_1e4 = xStack_1e4 | 8;
                        -- TODO(native): MsgIsRegionUnloaded is not a ForgeFSE binding
                        quest:MsgIsRegionUnloaded("SCRIPT_NAME_HERO")
                        if bVar5 then
                            goto LAB_00e5a8ab
                        else
                            bVar5 = quest:MsgIsActionModeButtonPressed()
                            if bVar5 then
                                bVar5 = quest:MsgIsTutorialClickedPast()
                                if not bVar5 then goto LAB_00e5a8ab end
                            end
                            bVar5 = false
                        end
                        goto FLOW_past_lab_00e5a8ab
                        ::LAB_00e5a8ab::
                        bVar5 = true
                        ::FLOW_past_lab_00e5a8ab::
                            -- TODO(native): xStack_1e4 = uVar13 & 0xfffffff7;
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b24b end
                            quest:SetStateBool("BeggarHit", true)
                        end
                    until not (not bVar3)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e5b24b end
                    if not bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e5b24b end
                        iVar9 = math.random(0, 32767)
                        native_arg_switch_3 = iVar9 % 5
                        repeat
                            if native_arg_switch_3 == 0 then
                                me:PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, true, false, false)
                                break
                            else
                                if native_arg_switch_3 == 1 then
                                    me:PlayAnimation("ST_OPINION_DISAPPROVAL_CALLING_OVER_FOR_FIGHT", false, false, false, true, true, false, false)
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
                        uVar8 = quest:AddNewConversation(nil --[[missing]], false, false)
                        quest:AddPersonToConversation(uVar8, nil --[[missing]])
                        uVar7 = math.random(0, 32767)
                        uVar7 = uVar7 & 0x80000003
                        bVar5 = uVar7 == 0
                        if uVar7 < 0 then
                            bVar5 = (uVar7 - 1 | 0xfffffffc) == 0xffffffff
                        end
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e5b24b end
                            quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_01", nil --[[missing]], nil --[[missing]], false)
                        else
                            uVar7 = math.random(0, 32767)
                            uVar7 = uVar7 & 0x80000003
                            bVar5 = uVar7 == 0
                            if uVar7 < 0 then
                                bVar5 = (uVar7 - 1 | 0xfffffffc) == 0xffffffff
                            end
                            if bVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e5b24b end
                                quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_02", nil --[[missing]], nil --[[missing]], false)
                            else
                                uVar7 = math.random(0, 32767)
                                uVar7 = uVar7 & 0x80000003
                                bVar5 = uVar7 == 0
                                if uVar7 < 0 then
                                    bVar5 = (uVar7 - 1 | 0xfffffffc) == 0xffffffff
                                end
                                if bVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        goto LAB_00e5b24b
                                    end
                                    goto FLOW_hoist_lab_00e5b24b_1
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e5b24b end
                                    quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_04", nil --[[missing]], nil --[[missing]], false)
                                end
                            end
                        end
                    end
                    goto FLOW_past_lab_00e5b24b
                    ::LAB_00e5b24b::
                    quest:DeregisterTimer(i_stk_1fc)
                    goto LAB_00e5b19b
                    ::FLOW_hoist_lab_00e5b24b_1::
                    quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_03", nil --[[missing]], nil --[[missing]], false)
                    ::FLOW_past_lab_00e5b24b::
                    quest:DeregisterTimer(i_stk_1fc)
                end
                ::LAB_00e5ac46::
                xStack_1e8 = me:MsgExpressionPerformedTo()
                cVar4 = xStack_1e8 ~= nil
                if not cVar4 then goto LAB_00e5b163 end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    goto LAB_00e5b29c
                end
                goto FLOW_past_lab_00e5b29c
                ::LAB_00e5b29c::
                goto LAB_00e5b19b
                ::FLOW_past_lab_00e5b29c::
                uVar8 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(uVar8, nil --[[missing]])
                iVar9 = quest:RegisterTimer()
                i_stk_1fc = iVar9
                quest:SetTimer(iVar9, 2)
                while true do
                    __native_condition_1 = not bVar3
                    if __native_condition_1 then
                        iVar10 = quest:GetTimer(iVar9)
                        __native_condition_1 = 0 < iVar10
                    end
                    if not __native_condition_1 then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e5b276 end
                    bVar5 = me:IsTalkedToByHero()
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:DeregisterTimer(iVar9)
                            goto LAB_00e5b19b
                        end
                        bVar3 = true
                    end
                    -- TODO(native): xStack_1e4 = xStack_1e4 | 0x40;
                    bVar5 = me:MsgIsHitByHero()
                    if bVar5 then
                        goto LAB_00e5addd
                    else
                        bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar5 then
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar5 then goto LAB_00e5addd end
                        end
                        bVar5 = false
                    end
                    goto FLOW_past_lab_00e5addd
                    ::LAB_00e5addd::
                    bVar5 = true
                    ::FLOW_past_lab_00e5addd::
                        -- TODO(native): xStack_1e4 = uVar13 & 0xffffffbf;
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            goto LAB_00e5b276
                        end
                        goto FLOW_hoist_lab_00e5b276_1
                    end
                    goto FLOW_past_lab_00e5b276
                    ::LAB_00e5b276::
                    quest:DeregisterTimer(iVar9)
                    goto LAB_00e5b19b
                    ::FLOW_hoist_lab_00e5b276_1::
                    quest:SetStateBool("BeggarHit", true)
                    ::FLOW_past_lab_00e5b276::
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    goto LAB_00e5b293
                end
                goto FLOW_past_lab_00e5b293
                ::LAB_00e5b293::
                quest:DeregisterTimer(i_stk_1fc)
                goto LAB_00e5b29c
                ::FLOW_past_lab_00e5b293::
                if bVar3 then goto FLOW_native_label_2 end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e5b293 end
                if xStack_1e8 ~= nil then
                    -- TODO(native): p0 = *xStack_1e8
                    p0 = nil --[[unresolved native value]]
                    -- TODO(native): iVar9 = CBasicString<char>::Compare(p0,"EXPRESSION_BELCH");
                    bVar3 = false
                    if iVar9 == 0 then goto LAB_00e5aedc end
                    -- TODO(native): iVar9 = CBasicString<char>::Compare(p0,"EXPRESSION_FART");
                    bVar3 = false
                    if iVar9 ~= 0 then goto FLOW_native_label_2 end
                    -- LAB_00e5b036: (native jump target)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e5b293 end
                    uVar8 = quest:AddNewConversation(me, false, false)
                    uVar11 = quest:GetHero()
                    quest:AddPersonToConversation(uVar8, uVar11)
                    if r1 == 1 then
                        uVar11 = quest:GetHero()
                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY", me, uVar11, false)
                    else
                        if r1 == 2 then
                            uVar11 = quest:GetHero()
                            quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY", me, uVar11, false)
                        else
                            uVar11 = quest:GetHero()
                            quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY", me, uVar11, false)
                        end
                    end
                    goto LAB_00e5b14b
                end
                bVar5 = false
                if not bVar5 then
                    bVar5 = false
                    goto FLOW_native_label_2
                end
                ::LAB_00e5aedc::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e5b293 end
                quest:SetStateInt("BelchedAtBeggar", quest:GetStateInt("BelchedAtBeggar") + 1)
                uVar8 = quest:AddNewConversation(me, false, false)
                uVar11 = quest:GetHero()
                quest:AddPersonToConversation(uVar8, uVar11)
                native_arg_switch_4 = quest:GetStateInt("BelchedAtBeggar")
                if not (native_arg_switch_4 == 1 or native_arg_switch_4 == 2 or native_arg_switch_4 == 3 or native_arg_switch_4 == 4) then
                    native_arg_switch_4 = 0x7ffffffe
                end
                repeat
                    if native_arg_switch_4 == 1 then
                        uVar11 = quest:GetHero()
                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY", me, uVar11, false)
                        break
                    end
                    if native_arg_switch_4 == 2 then
                        uVar11 = quest:GetHero()
                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY", me, uVar11, false)
                        break
                    end
                    if native_arg_switch_4 == 3 then
                        uVar11 = quest:GetHero()
                        quest:AddLineToConversation(uVar8, "TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY", me, uVar11, false)
                        break
                    end
                    if native_arg_switch_4 == 4 then
                        quest:SetStateBool("BeggarLeft", true)
                        quest:SetStateBool("BelchedAtBully", true)
                        native_arg_switch_4 = 0x7ffffffe
                    end
                    if native_arg_switch_4 == 0x7ffffffe then goto FLOW_native_label_2 end
                until not (false)
                ::LAB_00e5b14b::
                ::FLOW_native_label_2::
                quest:DeregisterTimer(i_stk_1fc)
                ::LAB_00e5b163::
                cVar4 = quest:GetStateBool("BeggarHit")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            native_arg_sequence_2 = false
            if not bVar3 then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                bVar3 = (not resources:ScriptThing(r1):IsNull())
                if bVar3 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
            if native_arg_sequence_2 then
                resources:PrepareResource(r1)
            end
            ::LAB_00e5b19b::
        end
        ::LAB_00e5b1a4::
        resources:ReleaseResource(r1)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

