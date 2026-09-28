-- Generated native draft: BordelloLady. Review coverage report before use.
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
    local CVar3, __native_condition_1, bVar5, cVar6, fVar17, fVar18, iVar19, iVar21, iVar24, iVar25, iVar9, native_arg_sequence_1, p0, pCVar11, pCVar12, pCVar7, pcVar26, piVar1, pppuVar20, pppuVar28, ppuVar13, pvVar8, r1, r10, r11, r12, r13, r2, r3, r4, r5, r6, r7, r8, r9, uVar10, uVar15, uVar4, xStack_15c, xStack_190, xStack_1a0, xStack_1a8, xStack_b4, xStack_d4, x_stk_188
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        x_stk_188 = resources:NewResource()
        quest:SetCreatureBrain(nil --[[missing]], "BRAIN_PASSIVE_OVERRIDE")
        r1 = me:GetHomePos()
        r1 = nil
        xStack_15c = __native_entity_state:GetStateInt("self_0xc")
        quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])
        xStack_d4 = nil
        quest:SetWanderMinDistance(nil --[[missing]], 0)
        x_stk_188 = nil
        xStack_1a0 = __native_entity_state:GetStateInt("self_0xc")
        -- TODO(native): xStack_1a0 = *(undefined ***)(this + 0x10);
        if xStack_1a0 ~= nil then
            -- TODO(native): *xStack_1a0 = *xStack_1a0 + 1;
        end
        quest:SetWanderMaxDistance(nil --[[missing]], 16.0)
        -- TODO(native): xStack_1a0 = *(undefined ***)(this + 0x10);
        if xStack_1a0 ~= nil then
            -- TODO(native): *xStack_1a0 = (undefined *)((int)*xStack_1a0 + 1);
        end
        quest:SetScriptingStateGroup(nil --[[missing]], 0x0)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while true do
            if bVar5 then
                return
            end
            cVar6 = me:IsTalkedToByHero()
            __native_condition_1 = not cVar6
            if not __native_condition_1 then
                cVar6 = quest:IsReportedOrUnreportedCrimeKnown(nil --[[missing]])
                __native_condition_1 = cVar6
            end
            if __native_condition_1 then
                bVar5 = false
            else
                bVar5 = true
            end
                -- TODO(native): xStack_15c = xStack_15c & 0xfffffffe;
            if bVar5 then break end
            -- LAB_00e40085: (native jump target)
            if (not __native_entity_state:GetStateBool("NoLongerWorking")) and (quest:GetStateBool("BecomeNunnery")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e3f744 end
                __native_entity_state:SetStateBool("NoLongerWorking", true)
                quest:ClearThingHasInformation(nil --[[missing]])
                -- TODO(native): EntitySetPersonalityOverride is not a ForgeFSE binding
                quest:EntitySetPersonalityOverride()
            end
            -- TODO(native): xStack_15c = xStack_15c | 2;
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                -- TODO(native): bVar5 = (**(*me + 0xa8))(me,&xStack_120)
                bVar5 = nil --[[unresolved native value]]
                if bVar5 then
                    -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,&xStack_128)
                    bVar5 = nil --[[unresolved native value]]
                    if not bVar5 then goto LAB_00e40183 end
                end
            else
                goto LAB_00e40183
            end
            goto FLOW_past_lab_00e40183
            ::LAB_00e40183::
            ::FLOW_past_lab_00e40183::
                -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
            if 1 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    resources:PrepareResource(resources:MemberResource("seh_Whore"))
                    resources:PrepareResource(xStack_1a0)
                    cVar6 = me:AcquireControl(4)
                    while not cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e3f744 end
                        cVar6 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        me:ClearCommands()
                        me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e3f744 end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            resources:PrepareResource(xStack_1a0)
                            goto LAB_00e402f9
                        end
                    end
                end
                goto LAB_00e3f744
            end
            ::LAB_00e402f9::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        end
        ::FLOW_after_lab_00e40085::
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            resources:PrepareResource(resources:MemberResource("seh_Whore"))
            xStack_190 = resources:StartMovie("")
            pppuVar28 = xStack_190
            quest:StartMovieSequence()
            xStack_1a0 = piVar1
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(xStack_1a0)
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_190)
                    resources:DestroyMovie(xStack_190)
                    return
                end
                cVar6 = me:AcquireControl(8)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- LAB_00e40364: (native jump target)
                resources:DestroyMovie(xStack_190)
                resources:DestroyMovie(xStack_190)
                return
            end
            me:ClearCommands()
            if quest:GetStateBool("BecomeNunnery") then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4039f end
                xStack_b4 = resources:ScriptThing(xStack_1a0)
                fVar17 = quest:GetHealth(nil --[[missing]])
                fVar18 = 0.0
                if fVar18 < fVar17 then
                    iVar25 = 0
                    iVar24 = 1
                    iVar21 = 0
                    iVar19 = 0
                    pCVar7 = helper_E403D0(quest, me, "HAPPY")
                    pvVar8 = pCVar7
                    iVar9 = quest:GetHero()
                    r2 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e3f729 end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e3eee8 end
                end
                goto LAB_00e3ffe3
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                goto LAB_00e403af
            else
                native_arg_sequence_1 = false
                if quest:GetStateBool("HeroTricking") then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                    if bVar5 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    if not quest:GetStateBool("PlayerOwned") then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        fVar17 = quest:GetHealth(nil --[[missing]])
                        fVar18 = 0.0
                        if fVar18 < fVar17 then
                            iVar25 = 0
                            iVar24 = 1
                            iVar21 = 0
                            iVar19 = 0
                            pCVar7 = helper_E403D0(quest, me, "MAGICIAN_HINT")
                            pvVar8 = pCVar7
                            iVar9 = quest:GetHero()
                            r3 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e3f729 end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e3eee8 end
                        end
                        goto LAB_00e3ffe3
                    end
                    goto LAB_00e4039f
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e403af end
                if not quest:GetStateBool("PlayerOwned") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e403af end
                    if not __native_entity_state:GetStateBool("HaveTalked") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e403af end
                        fVar17 = quest:GetHealth(nil --[[missing]])
                        fVar18 = 0.0
                        if fVar17 <= fVar18 then
                            goto LAB_00e3f953
                        end
                        goto FLOW_past_lab_00e3f953
                        ::LAB_00e3f953::
                        __native_entity_state:SetStateBool("HaveTalked", true)
                        goto LAB_00e3f957
                        ::FLOW_past_lab_00e3f953::
                        iVar25 = 0
                        iVar24 = 1
                        iVar21 = 0
                        iVar19 = 0
                        pCVar7 = helper_E403D0(quest, me, "PARTY_PAID_INTRODUCTION")
                        pvVar8 = pCVar7
                        iVar9 = quest:GetHero()
                        r4 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e3eee8 end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then goto LAB_00e3f953 end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e4039f end
                        if not __native_entity_state:GetStateBool("PartiedAlready") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e4039f end
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar18 = 0.0
                            if fVar18 < fVar17 then
                                iVar25 = 0
                                iVar24 = 1
                                iVar21 = 0
                                iVar19 = 0
                                pCVar7 = helper_E403D0(quest, me, "PARTY_PAID_REMINDER")
                                pvVar8 = pCVar7
                                iVar9 = quest:GetHero()
                                r5 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e3f729 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar6 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e3eee8 end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e403af end
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar18 = 0.0
                            if fVar18 < fVar17 then
                                iVar25 = 0
                                iVar24 = 1
                                iVar21 = 0
                                iVar19 = 0
                                pCVar7 = helper_E403D0(quest, me, "PARTY_AGAIN")
                                pvVar8 = pCVar7
                                iVar9 = quest:GetHero()
                                r6 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e3eee8 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar6 = iVar9
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e3f729 end
                            end
                        end
                        goto LAB_00e3f957
                    end
                    goto FLOW_past_lab_00e3f957
                    ::LAB_00e3f957::
                    uVar10 = helper_E403D0(quest, me, "PARTY_PAID_QUESTION")
                    quest:GiveHeroYesNoQuestion(uVar10, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_PAID_QUESTION")
                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar9 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e3eee8 end
                        iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if iVar9 == 1 then
                            if not bVar5 then
                                iVar9 = quest:GetHeroGold()
                                if __native_entity_state:GetStateInt("GoldRequired") <= iVar9 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e3f729 end
                                    fVar17 = quest:GetHealth(nil --[[missing]])
                                    fVar18 = 0.0
                                    if fVar18 < fVar17 then
                                        iVar25 = 0
                                        iVar24 = 1
                                        iVar21 = 0
                                        iVar19 = 0
                                        pCVar7 = helper_E403D0(quest, me, "PARTY_PAID_FOLLOW_ME")
                                        pvVar8 = pCVar7
                                        iVar9 = quest:GetHero()
                                        r7 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar6 = iVar9
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e3eee8 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar6 = iVar9
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3f729 end
                                    end
                                    quest:GiveHeroGold(fVar18)
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (resources:MemberResource("seh_Whore"),&xStack_1a8);
                                    pCVar11 = __native_entity_state:GetStateString("Name")
                                    pCVar12 = ("TEXT_CS_B13_SEX_" .. pCVar11)
                                    resources:SetString(resources:MemberStringMap("csargs"), pCVar7, pCVar12)
                                    pCVar12 = helper_E403D0(quest, me, "PARTY_PAID_PLEASED")
                                    resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", pCVar12)
                                    quest:SetCutsceneSkippable(nil --[[missing]])
                                    if not quest:GetStateBool("HadSex") then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3f729 end
                                        if pCVar11 == nil then
                                            bVar5 = false
                                            goto LAB_00e3fd35
                                        else
                                            iVar9 = ((pCVar11 == "HEDWIG") and 0 or 1)
                                            if iVar9 ~= 0 then goto LAB_00e3fd35 end
                                            -- LAB_00e3fdc4: (native jump target)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e3eee8 end
                                            pcVar26 = "CS_BORDELLO_PAYINGFORSEX_HEDWIG"
                                        end
                                        goto FLOW_past_lab_00e3fd35
                                        ::LAB_00e3fd35::
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3f729 end
                                        pcVar26 = "CS_BORDELLO_PAYINGFORSEX"
                                        ::FLOW_past_lab_00e3fd35::
                                        require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, pcVar26, true)
                                        quest:SetStateBool("HadSex", true)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3eee8 end
                                        if pCVar11 == nil then
                                            bVar5 = false
                                            if bVar5 then
                                                goto LAB_00e3fcdb
                                            end
                                        else
                                            iVar9 = ((pCVar11 == "HEDWIG") and 0 or 1)
                                            if iVar9 == 0 then goto LAB_00e3fcdb end
                                        end
                                        goto FLOW_past_lab_00e3fcdb
                                        ::LAB_00e3fcdb::
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3f729 end
                                        require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG", true)
                                        goto LAB_00e3fd64
                                        ::FLOW_past_lab_00e3fcdb::
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3eee8 end
                                        require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_PAYINGFORSEX_QUICKIE", true)
                                    end
                                    ::LAB_00e3fd64::
                                    quest:SetCutsceneSkippable(nil --[[missing]])
                                    __native_entity_state:SetStateBool("PartiedAlready", true)
                                    quest:SetStateBool("HeroPartying", true)
                                    goto LAB_00e3fd7c
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    fVar17 = quest:GetHealth(nil --[[missing]])
                                    fVar18 = 0.0
                                    if fVar18 < fVar17 then
                                        iVar25 = 0
                                        iVar24 = 1
                                        iVar21 = 0
                                        iVar19 = 0
                                        pCVar7 = helper_E403D0(quest, me, "PARTY_PAID_TOO_EXPENSIVE")
                                        pvVar8 = pCVar7
                                        iVar9 = quest:GetHero()
                                        r8 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar6 = iVar9
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e3f729 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar6 = iVar9
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e3eee8 end
                                    end
                                    goto LAB_00e3ffe3
                                end
                            end
                            goto LAB_00e3eee8
                        end
                        goto FLOW_hoist_lab_00e3eee8_1
                    end
                    ::FLOW_past_lab_00e3f957::
                    goto FLOW_hoist_lab_00e3eee8_3
                end
                goto FLOW_hoist_lab_00e3eee8_4
            end
            goto FLOW_past_lab_00e403af
            ::LAB_00e403af::
            -- TODO(native): ppuVar13 = *piVar1
            ppuVar13 = nil --[[unresolved native value]]
            ::LAB_00e3f731::
            -- TODO(native): (*(code *)ppuVar13[0x17b])();
            ::FLOW_past_lab_00e403af::
            goto FLOW_past_lab_00e3eee8
            ::LAB_00e3eee8::
            -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();
            goto LAB_00e3f737
            ::FLOW_hoist_lab_00e3eee8_1::
            if bVar5 then goto LAB_00e3f729 end
            fVar17 = quest:GetHealth(nil --[[missing]])
            fVar18 = 0.0
            if fVar18 < fVar17 then
                iVar25 = 0
                iVar24 = 1
                iVar21 = 0
                iVar19 = 0
                pCVar7 = helper_E403D0(quest, me, "PARTY_PAID_DECLINED")
                pvVar8 = pCVar7
                iVar9 = quest:GetHero()
                r9 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                iVar9 = me:IsPerformingScriptTask()
                cVar6 = iVar9
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e3eee8 end
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e3f729 end
            end
            ::LAB_00e3ffe3::
            if not __native_entity_state:GetStateBool("WalkingDownstairs") then
                goto LAB_00e40044
            else
                iVar9 = 2.0
                pvVar8 = me:GetHomePos()
                iVar9 = (me ~= nil and me:IsDistanceFromPositionOver(pvVar8, iVar9))
                if not iVar9 then goto LAB_00e40044 end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e3eee8 end
                iVar24 = 1
                iVar21 = 0
                iVar19 = 0
                iVar9 = 1.0
                -- TODO(native): p0 = (**(*me + 0x1c))(me,xStack_30)
                p0 = nil --[[unresolved native value]]
                me:MoveToPosition(p0, iVar9, iVar19, (iVar21 ~= 0), (iVar24 ~= 0))
            end
            goto FLOW_past_lab_00e40044
            ::LAB_00e40044::
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e3f729 end
            __native_entity_state:SetStateBool("WalkingDownstairs", false)
            bVar5 = (not resources:ScriptThing(xStack_1a0):IsNull())
            if bVar5 then
                resources:PrepareResource(xStack_1a8)
            end
            ::FLOW_past_lab_00e40044::
            -- TODO(native): (*(code *)(*pppuVar28)[0x17b])();
            resources:ReleaseResource(xStack_1a0)
            if (not __native_entity_state:GetStateBool("NoLongerWorking")) and (quest:GetStateBool("BecomeNunnery")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e3f744 end
                __native_entity_state:SetStateBool("NoLongerWorking", true)
                quest:ClearThingHasInformation(nil --[[missing]])
                -- TODO(native): EntitySetPersonalityOverride is not a ForgeFSE binding
                quest:EntitySetPersonalityOverride()
            end
            uVar4 = ""
            -- TODO(native): xStack_15c = xStack_15c | 2;
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                -- TODO(native): bVar5 = (**(*me + 0xa8))(me,&xStack_120)
                bVar5 = nil --[[unresolved native value]]
                if bVar5 then
                    -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,&xStack_128)
                    bVar5 = nil --[[unresolved native value]]
                    if not bVar5 then goto LAB_00e40183_c2 end
                end
            end
            goto FLOW_past_lab_00e40183_c2
            ::LAB_00e40183_c2::
            ::FLOW_past_lab_00e40183_c2::
                -- TODO(native): xStack_15c = uVar15 & 0xfffffffd;
            if 1 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    resources:PrepareResource(resources:MemberResource("seh_Whore"))
                    resources:PrepareResource(xStack_1a0)
                    cVar6 = me:AcquireControl(4)
                    while not cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e3f744 end
                        cVar6 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        me:ClearCommands()
                        me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, true, false, false)
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e3f744 end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            resources:PrepareResource(xStack_1a0)
                            goto LAB_00e402f9_c2
                        end
                    end
                end
                goto LAB_00e3f744
            end
            ::LAB_00e402f9_c2::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            goto FLOW_after_lab_00e40085
            ::FLOW_hoist_lab_00e3eee8_3::
            goto LAB_00e3f729
            ::FLOW_hoist_lab_00e3eee8_4::
            goto FLOW_hoist_lab_00e3f729_1
            ::FLOW_past_lab_00e3eee8::
            goto FLOW_past_lab_00e3f729
            ::LAB_00e3f729::
            -- TODO(native): ppuVar13 = *pppuVar28
            ppuVar13 = nil --[[unresolved native value]]
            goto LAB_00e3f731
            ::FLOW_hoist_lab_00e3f729_1::
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                xStack_b4 = resources:ScriptThing(xStack_1a8)
                fVar18 = quest:GetHealth(nil --[[missing]])
                cVar6 = 0.0 < fVar18
                if cVar6 then
                    iVar25 = 0
                    iVar24 = 1
                    iVar21 = 0
                    iVar19 = 0
                    pCVar7 = helper_E403D0(quest, me, "PARTY_FREE")
                    pvVar8 = pCVar7
                    iVar9 = quest:GetHero()
                    r10 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e403af end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e4039f end
                end
                pppuVar20 = "TEXT_OBJECT_HERO_ANSWER_YES"
                uVar10 = helper_E403D0(quest, me, "PARTY_FREE_QUESTION")
                quest:GiveHeroYesNoQuestion(uVar10, pppuVar20, "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_FREE_QUESTION")
                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar9 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e403af end
                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if iVar9 == 1 then
                        if bVar5 then goto LAB_00e403af end
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar18
                        if cVar6 then
                            iVar25 = 0
                            iVar24 = 1
                            iVar21 = 0
                            iVar19 = 0
                            pCVar7 = helper_E403D0(quest, me, "PARTY_FREE_FOLLOW_ME")
                            pvVar8 = pCVar7
                            iVar9 = quest:GetHero()
                            r11 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e4039f end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e403af end
                        end
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (resources:MemberResource("seh_Whore"),&xStack_1a8);
                        pcVar26 = "_BOSS"
                        pCVar11 = ("TEXT_CS_B13_SEX_" .. __native_entity_state:GetStateString("Name"))
                        pCVar11 = (pCVar11 .. pcVar26)
                        resources:SetString(resources:MemberStringMap("csargs"), "$SEXTALK", pCVar11)
                        pCVar11 = helper_E403D0(quest, me, "PARTY_FREE_FINISHING_UP")
                        resources:SetString(resources:MemberStringMap("csargs"), "$ENDLINE", pCVar11)
                        quest:SetCutsceneSkippable(true)
                        -- TODO(native): CVar3 = *__native_entity_state:GetStateString("Name")
                        CVar3 = nil --[[unresolved native value]]
                        if CVar3 == nil then
                            bVar5 = false
                            goto LAB_00e3f48a
                        end
                        goto FLOW_hoist_lab_00e3f48a_1
                    end
                    goto FLOW_hoist_lab_00e3f48a_2
                end
            end
            goto FLOW_past_lab_00e3f48a
            ::LAB_00e3f48a::
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e403af end
            pcVar26 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE"
            ::LAB_00e3f4a5::
            require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, pcVar26, true)
            quest:SetCutsceneSkippable(nil --[[missing]])
            __native_entity_state:SetStateBool("PartiedAlready", true)
            quest:SetStateBool("HeroPartying", true)
            goto LAB_00e3fd7c
            ::FLOW_hoist_lab_00e3f48a_1::
            goto FLOW_hoist_lab_00e3fd7c_1
            ::FLOW_hoist_lab_00e3f48a_2::
            goto FLOW_hoist_lab_00e3fd7c_2
            ::FLOW_past_lab_00e3f48a::
            goto FLOW_past_lab_00e3fd7c
            ::LAB_00e3fd7c::
            r12 = quest:GetNumberOfTimesHeroHasHadSex()
            quest:SetNumberOfTimesHeroHasHadSex(nil --[[missing]])
            quest:SetHeroAsHavingHadSex(true)
            __native_entity_state:SetStateBool("WalkingDownstairs", true)
            goto LAB_00e3ffe3
            ::FLOW_hoist_lab_00e3fd7c_1::
            iVar9 = ((CVar3 == "HEDWIG") and 0 or 1)
            if iVar9 ~= 0 then goto LAB_00e3f48a end
            -- LAB_00e3f4e7: (native jump target)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                pcVar26 = "CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG"
                goto LAB_00e3f4a5
            end
            -- TODO(native): (**(code **)(*piVar1 + 0x5ec))();
            goto LAB_00e3f737
            ::FLOW_hoist_lab_00e3fd7c_2::
            if not bVar5 then
                fVar17 = quest:GetHealth(nil --[[missing]])
                fVar18 = 0.0
                if fVar18 < fVar17 then
                    iVar25 = 0
                    iVar24 = 1
                    iVar21 = 0
                    iVar19 = 0
                    pCVar7 = helper_E403D0(quest, me, "PARTY_FREE_DECLINED")
                    pvVar8 = pCVar7
                    iVar9 = quest:GetHero()
                    r13 = me:Speak(iVar9, pvVar8, iVar19, (iVar21 ~= 0), (iVar24 ~= 0), (iVar25 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e3f729 end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e3eee8 end
                end
                goto LAB_00e3ffe3
            end
            ::FLOW_past_lab_00e3fd7c::
            goto LAB_00e4039f
            ::FLOW_past_lab_00e3f729::
            goto FLOW_past_lab_00e4039f
            ::LAB_00e4039f::
            quest:PauseAllNonScriptedEntities((fVar18 ~= 0))
            ::FLOW_past_lab_00e4039f::
            ::LAB_00e3f737::
            resources:DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")
        end
        ::LAB_00e3f744::
        resources:ReleaseResource(xStack_1a0)
    end
end

function Init(quest, me)
    local bVar4, iVar1, pOther, this_00
    -- TODO(native): puStack_1c = v_stk_4;
    __native_entity_state:SetStateBool("WalkingDownstairs", false)
    __native_entity_state:SetStateBool("HaveTalked", false)
    __native_entity_state:SetStateBool("PartiedAlready", false)
    __native_entity_state:SetStateBool("NoLongerWorking", false)
    this_00 = __native_entity_state:GetStateString("Name")
    pOther = me:GetDataString()
    this_00 = pOther
    __native_entity_state:SetStateString("Name", pOther)
    if not quest:GetStateBool("BecomeNunnery") then
        quest:SetThingHasInformation(me, true)
    end
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
    __native_entity_state:SetStateInt("GoldRequired", 0)
    if this_00 == nil then
        bVar4 = false
        if bVar4 then
            goto LAB_00e3ad4d
        end
    else
        iVar1 = ((this_00 == "POLLY") and 0 or 1)
        if iVar1 == 0 then goto LAB_00e3ad4d end
    end
    goto FLOW_past_lab_00e3ad4d
    ::LAB_00e3ad4d::
    __native_entity_state:SetStateInt("GoldRequired", 0x32)
    ::FLOW_past_lab_00e3ad4d::
    if this_00 == nil then
        bVar4 = false
        if bVar4 then
            goto LAB_00e3ad89
        end
    else
        iVar1 = ((this_00 == "AMELIA") and 0 or 1)
        if iVar1 == 0 then goto LAB_00e3ad89 end
    end
    goto FLOW_past_lab_00e3ad89
    ::LAB_00e3ad89::
    __native_entity_state:SetStateInt("GoldRequired", 100)
    ::FLOW_past_lab_00e3ad89::
    if this_00 == nil then
        bVar4 = false
        if bVar4 then
            goto LAB_00e3adc5
        end
    else
        iVar1 = ((this_00 == "LUCREZIA") and 0 or 1)
        if iVar1 == 0 then goto LAB_00e3adc5 end
    end
    goto FLOW_past_lab_00e3adc5
    ::LAB_00e3adc5::
    __native_entity_state:SetStateInt("GoldRequired", 200)
    ::FLOW_past_lab_00e3adc5::
    if this_00 == nil then
        bVar4 = false
        if not bVar4 then goto LAB_00e3ae08 end
    else
        iVar1 = ((this_00 == "SOPHIA") and 0 or 1)
        if iVar1 ~= 0 then goto LAB_00e3ae08 end
    end
    __native_entity_state:SetStateInt("GoldRequired", 1000)
    ::LAB_00e3ae08::
    if this_00 == nil then
        bVar4 = false
        if not bVar4 then
            return
        end
    else
        iVar1 = ((this_00 == "HEDWIG") and 0 or 1)
        if iVar1 ~= 0 then
            return
        end
    end
    __native_entity_state:SetStateInt("GoldRequired", 2000)
end

function OnPersist(quest, me, context)
    local partiedAlready = quest:GetStateBool("PartiedAlready") or false
    partiedAlready = quest:PersistTransferBool(context, "PartiedAlready", partiedAlready)
    quest:SetStateBool("PartiedAlready", partiedAlready)
    local haveTalked = quest:GetStateBool("HaveTalked") or false
    haveTalked = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
    quest:SetStateBool("HaveTalked", haveTalked)
end

function OnPredicateFail(quest, me)
end

function helper_E403D0(quest, me, native_arg_dialogueSuffix)
    local pCVar1 = ("TEXT_QST_B13_" .. __native_entity_state:GetStateString("Name"))
    pCVar1 = (pCVar1 .. "_")
    local hiddenStringResult = (pCVar1 .. native_arg_dialogueSuffix)
    return hiddenStringResult
end

