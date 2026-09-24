-- Generated native draft: BordelloClient. Review coverage report before use.
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
    local __push1, __push2, __push3, __push4, __push5, __push6, bVar4, cVar5, fVar16, fVar2, iVar12, iVar17, iVar18, iVar19, iVar21, iVar6, native_arg_sequence_1, p0, pCVar10, pCVar7, pCVar9, pcVar20, piVar15, puVar8, pvVar11, r1, r10, r11, r2, r3, r4, r5, r6, r7, r8, r9, xStack_118, xStack_120, xStack_30, xStack_40, xStack_8c, xStack_c4_b3, xStack_dc, xStack_e0_b3, xStack_e4, xStack_e8_2_b3, x_stk_108, x_stk_18
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    xStack_118 = resources:NewResource()
    resources:PrepareResource(xStack_118)
    xStack_118 = xStack_118
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e469e6 end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00e469e6 end
    r1 = quest:GetThingWithScriptName("M_ClientStand")
    r2 = quest:GetThingWithScriptName("M_ClientSpawn")
    iVar6 = quest:RegisterTimer()
    pCVar7 = me:GetDefName()
    iVar12 = ((pCVar7 == "CREATURE_BS_VILLAGER_FEMALE") and 0 or 1)
    cVar5 = not (iVar12 ~= 0)
    if not cVar5 then
        iVar12 = __native_entity_state:GetStateInt("BrainState")
        while iVar12 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e469cb end
            iVar12 = me:IsPerformingScriptTask()
            if not iVar12 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                if not (xStack_118 ~= nil and not xStack_118:IsNull()) then
                    puVar8 = {x = 0, y = 0, z = 0}
                else
                    puVar8 = xStack_118:GetPos()
                end
                me:MoveToPosition(puVar8, 1.0, 0, false, true)
            end
            bVar4 = quest:IsDistanceBetweenThingsUnder(0x0, me, 5.0)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                __native_entity_state:SetStateInt("BrainState", 2)
            end
            bVar4 = me:IsTalkedToByHero()
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                me:ClearCommands()
                xStack_dc = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 3 < __native_entity_state:GetStateInt("NextLine") then
                    __native_entity_state:SetStateInt("NextLine", 1)
                end
                pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                pcVar20 = "_ENTER_0"
                pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar20)
                (pCVar9 .. pCVar7)
                xStack_40 = resources:ScriptThing(xStack_118)
                pCVar10 = xStack_40
                fVar16 = quest:GetHealth(pCVar10)
                fVar2 = 0.0
                if fVar2 < fVar16 then
                    iVar21 = 0
                    iVar19 = 1
                    iVar18 = 0
                    iVar17 = 0
                    pvVar11 = x_stk_108
                    iVar12 = quest:GetHero()
                    r3 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                    iVar12 = me:IsPerformingScriptTask()
                    cVar5 = iVar12
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_dc)
                            quest:DeregisterTimer(iVar6)
                            goto LAB_00e469d4
                        end
                        iVar12 = me:IsPerformingScriptTask()
                        cVar5 = iVar12
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_dc)
                        goto LAB_00e469cb
                    end
                end
                __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_dc)
            end
            iVar12 = __native_entity_state:GetStateInt("BrainState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            resources:PrepareResource(xStack_118)
            quest:SetTimer(iVar6, 0x78)
            iVar12 = __native_entity_state:GetStateInt("BrainState")
            while iVar12 == 2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                iVar12 = quest:GetTimer(iVar6)
                if (iVar12 == 0) or (quest:GetStateBool("BecomeNunnery")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    __native_entity_state:SetStateInt("BrainState", 3)
                end
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    resources:PrepareResource(xStack_118)
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e469cb end
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    me:ClearCommands()
                    xStack_dc = resources:StartMovie("")
                    pCVar10 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    native_arg_sequence_1 = false
                    -- TODO(native): if (*(this + 0x14))[0x49] ~= nil then
                    if false then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        bVar4 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                        if bVar4 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            if __native_entity_state:GetStateInt("SexChance") ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00e466b8 end
                                if 3 < __native_entity_state:GetStateInt("NextLine") then
                                    __native_entity_state:SetStateInt("NextLine", 1)
                                end
                                pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                                pcVar20 = "_DISGUSTED_0"
                                pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                                pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                                pCVar9 = (pCVar9 .. pcVar20)
                                (pCVar9 .. pCVar7)
                                x_stk_18 = resources:ScriptThing(xStack_118)
                                __push1 = x_stk_18
                                fVar16 = quest:GetHealth(__push1)
                                fVar2 = 0.0
                                if fVar2 < fVar16 then
                                    iVar21 = 0
                                    iVar19 = 1
                                    iVar18 = 0
                                    iVar17 = 0
                                    iVar12 = quest:GetHero()
                                    r4 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                                    iVar12 = me:IsPerformingScriptTask()
                                    cVar5 = iVar12
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_dc)
                                            goto LAB_00e469cb
                                        end
                                        iVar12 = me:IsPerformingScriptTask()
                                        cVar5 = iVar12
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_dc)
                                        goto LAB_00e469cb
                                    end
                                end
                                goto LAB_00e46498
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e466db end
                            if 3 < __native_entity_state:GetStateInt("NextLine") then
                                __native_entity_state:SetStateInt("NextLine", 1)
                            end
                            pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                            pcVar20 = "_INTERESTED_0"
                            pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                            pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                            pCVar9 = (pCVar9 .. pcVar20)
                            (pCVar9 .. pCVar7)
                            xStack_40 = resources:ScriptThing(xStack_118)
                            __push2 = xStack_40
                            fVar16 = quest:GetHealth(__push2)
                            fVar2 = 0.0
                            if fVar2 < fVar16 then
                                iVar21 = 0
                                iVar19 = 1
                                iVar18 = 0
                                iVar17 = 0
                                iVar12 = quest:GetHero()
                                r5 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                                iVar12 = me:IsPerformingScriptTask()
                                cVar5 = iVar12
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00e46644 end
                                    iVar12 = me:IsPerformingScriptTask()
                                    cVar5 = iVar12
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then goto LAB_00e45d07 end
                            else
                                goto LAB_00e45d07
                            end
                            goto FLOW_past_lab_00e45d07
                            ::LAB_00e45d07::
                            __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                            pcVar20 = "_SEX_QUESTION"
                            pCVar7 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                            pCVar7 = ("TEXT_QST_B13_CLIENT" .. pCVar7)
                            __push3 = (pCVar7 .. pcVar20)
                            quest:GiveHeroYesNoQuestion(__push3, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar12 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00e46644 end
                                iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if iVar12 == 1 then
                                    if bVar4 then
                                        -- LAB_00e46616: (native jump target)
                                        -- TODO(native): (**(code **)(*unaff_EBX + 0x5ec))();
                                        resources:DestroyMovie(xStack_dc)
                                        goto LAB_00e469cb
                                    end
                                    if 3 < __native_entity_state:GetStateInt("NextLine") then
                                        __native_entity_state:SetStateInt("NextLine", 1)
                                    end
                                    pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                                    pcVar20 = "_ACCEPTED_0"
                                    pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                                    pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                                    pCVar9 = (pCVar9 .. pcVar20)
                                    (pCVar9 .. pCVar7)
                                    xStack_30 = resources:ScriptThing(xStack_118)
                                    pCVar10 = xStack_30
                                    fVar16 = quest:GetHealth(pCVar10)
                                    fVar2 = 0.0
                                    if fVar2 < fVar16 then
                                        iVar21 = 0
                                        iVar19 = 1
                                        iVar18 = 0
                                        iVar17 = 0
                                        pvVar11 = ""
                                        iVar12 = quest:GetHero()
                                        r6 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                                        iVar12 = me:IsPerformingScriptTask()
                                        cVar5 = iVar12
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                -- TODO(native): (**(code **)(*unaff_EBP + 0x5ec))();
                                                resources:DestroyMovie(xStack_e4)
                                                goto LAB_00e469cb
                                            end
                                            iVar12 = me:IsPerformingScriptTask()
                                            cVar5 = iVar12
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            -- TODO(native): (**(code **)(*unaff_EBP + 0x5ec))();
                                            resources:DestroyMovie(xStack_e4)
                                            goto LAB_00e469cb
                                        end
                                    end
                                    __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                                    quest:GiveHeroGold(0xc8)
                                    __push4 = quest:GetNumberOfTimesHeroHasHadSex()
                                    quest:SetNumberOfTimesHeroHasHadSex(__push4)
                                    quest:SetCutsceneSkippable(false)
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)(*(int *)(this + 0x14) + 0xa4),&xStack_120);
                                    quest:SetStateBool("HeroPartying", true)
                                    pcVar20 = "_LEAVE_01"
                                    pCVar7 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                                    pCVar7 = ("TEXT_QST_B13_CLIENT" .. pCVar7)
                                    pCVar7 = (pCVar7 .. pcVar20)
                                    -- TODO(native): pCVar9 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 0xb4),&xStack_e8_2);
                                    pCVar9 = pCVar7
                                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                    quest:SetCutsceneSkippable(true)
                                    __native_entity_state:SetStateInt("BrainState", 3)
                                    quest:SetHeroAsHavingHadGaySex(true)
                                else
                                    if bVar4 then goto LAB_00e46644 end
                                    if 3 < __native_entity_state:GetStateInt("NextLine") then
                                        __native_entity_state:SetStateInt("NextLine", 1)
                                    end
                                    pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                                    pcVar20 = "_DECLINED_0"
                                    pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                                    pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                                    pCVar9 = (pCVar9 .. pcVar20)
                                    (pCVar9 .. pCVar7)
                                    x_stk_18 = resources:ScriptThing(xStack_120)
                                    __push5 = x_stk_18
                                    fVar16 = quest:GetHealth(__push5)
                                    fVar2 = 0.0
                                    if fVar2 < fVar16 then
                                        iVar21 = 0
                                        iVar19 = 1
                                        iVar18 = 0
                                        iVar17 = 0
                                        iVar12 = quest:GetHero()
                                        r7 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                                        iVar12 = me:IsPerformingScriptTask()
                                        cVar5 = iVar12
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                goto LAB_00e46644
                                            end
                                            iVar12 = me:IsPerformingScriptTask()
                                            cVar5 = iVar12
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            goto LAB_00e46644
                                        end
                                    end
                                    __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                                end
                                piVar15 = unaff_EBX
                                goto LAB_00e464a0
                            end
                            ::FLOW_past_lab_00e45d07::
                            ::LAB_00e46644::
                            -- TODO(native): (**(code **)(*unaff_EBX + 0x5ec))();
                            resources:DestroyMovie(xStack_dc)
                            goto LAB_00e469cb
                        end
                        goto LAB_00e466b8
                    end
                    goto FLOW_past_lab_00e466b8
                    ::LAB_00e466b8::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_dc)
                    goto LAB_00e469cb
                    ::FLOW_past_lab_00e466b8::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        goto LAB_00e466db
                    end
                    goto FLOW_past_lab_00e466db
                    ::LAB_00e466db::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_dc)
                    goto LAB_00e469cb
                    ::FLOW_past_lab_00e466db::
                    if 3 < __native_entity_state:GetStateInt("NextLine") then
                        __native_entity_state:SetStateInt("NextLine", 1)
                    end
                    pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                    pcVar20 = "_BROWSE_0"
                    pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                    pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                    pCVar9 = (pCVar9 .. pcVar20)
                    (pCVar9 .. pCVar7)
                    xStack_8c = resources:ScriptThing(xStack_118)
                    __push6 = xStack_8c
                    fVar16 = quest:GetHealth(__push6)
                    fVar2 = 0.0
                    if fVar2 < fVar16 then
                        iVar21 = 0
                        iVar19 = 1
                        iVar18 = 0
                        iVar17 = 0
                        pvVar11 = "$ENDLINE"
                        iVar12 = quest:GetHero()
                        r8 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                        iVar12 = me:IsPerformingScriptTask()
                        cVar5 = iVar12
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                goto LAB_00e466b8
                            end
                            iVar12 = me:IsPerformingScriptTask()
                            cVar5 = iVar12
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            goto LAB_00e466db
                        end
                    end
                    ::LAB_00e46498::
                    __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                    ::LAB_00e464a0::
                    resources:Reset((__native_entity_state:GetStateInt("self_0x14") + 0xa4))
                    resources:PrepareResource(xStack_118)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_dc)
                end
                iVar12 = __native_entity_state:GetStateInt("BrainState")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                resources:PrepareResource(xStack_118)
                cVar5 = me:AcquireControl(4)
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    cVar5 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    iVar6 = __native_entity_state:GetStateInt("BrainState")
                    while iVar6 == 3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e469cb end
                        iVar6 = me:IsPerformingScriptTask()
                        if not iVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e469cb end
                            if xStack_e4 == nil then
                                puVar8 = {x = 0, y = 0, z = 0}
                            else
                                -- TODO(native): puVar8 = (**(*xStack_e4 + 0x18))()
                                puVar8 = nil --[[unresolved native value]]
                            end
                            me:MoveToPosition(puVar8, 1.0, 0, false, true)
                        end
                        bVar4 = quest:IsDistanceBetweenThingsUnder(r2, me, 5.0)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e469cb end
                            __native_entity_state:SetStateInt("BrainState", 4)
                        end
                        cVar5 = me:IsTalkedToByHero()
                        if cVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e469cb end
                            me:ClearCommands()
                            xStack_dc = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            if 3 < __native_entity_state:GetStateInt("NextLine") then
                                __native_entity_state:SetStateInt("NextLine", 1)
                            end
                            pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                            pcVar20 = "_LEAVE_0"
                            pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                            pCVar9 = ("TEXT_QST_B13_CLIENT" .. pCVar9)
                            pCVar9 = (pCVar9 .. pcVar20)
                            (pCVar9 .. pCVar7)
                            xStack_8c = resources:ScriptThing(xStack_118)
                            pCVar10 = xStack_8c
                            fVar16 = quest:GetHealth(pCVar10)
                            fVar2 = 0.0
                            if fVar2 < fVar16 then
                                iVar19 = 0
                                iVar18 = 1
                                iVar17 = 0
                                iVar12 = 0
                                pvVar11 = "$ENDLINE"
                                iVar6 = quest:GetHero()
                                r9 = me:Speak(iVar6, pvVar11, iVar12, (iVar17 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_dc)
                                        goto LAB_00e469cb
                                    end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar5 = iVar6
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_dc)
                                    goto LAB_00e469cb
                                end
                            end
                            __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_dc)
                        end
                        iVar6 = __native_entity_state:GetStateInt("BrainState")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        if __native_entity_state:GetStateInt("BrainState") == 4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                quest:FadeOutAndKillEntity(me, true, 2.0, true)
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                until not (not bVar4)
                            end
                        else
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                            until not (not bVar4)
                        end
                    end
                end
            end
        end
        goto LAB_00e469cb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e469cb end
        xStack_e0_b3 = bVar4
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        while not bVar4 do
            if not xStack_e0_b3 then
                iVar12 = me:IsPerformingScriptTask()
                if not iVar12 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    if not (xStack_118 ~= nil and not xStack_118:IsNull()) then
                        puVar8 = {x = 0, y = 0, z = 0}
                    else
                        puVar8 = xStack_118:GetPos()
                    end
                    me:MoveToPosition(puVar8, 1.0, 0, false, true)
                end
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, 0x0, 5.0)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    -- TODO(native): xStack_e0_b3 = '\x01';
                    resources:PrepareResource(xStack_118)
                end
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                me:ClearCommands()
                resources:PrepareResource(xStack_118)
                cVar5 = me:AcquireControl(4)
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    cVar5 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                xStack_dc = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 2 < __native_entity_state:GetStateInt("NextLine") then
                    __native_entity_state:SetStateInt("NextLine", 1)
                end
                pCVar7 = tostring(__native_entity_state:GetStateInt("NextLine"))
                pcVar20 = "_CHAT_0"
                pCVar9 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                pCVar9 = ("TEXT_QST_B13_WOMAN" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar20)
                (pCVar9 .. pCVar7)
                xStack_30 = resources:ScriptThing(xStack_118)
                pCVar10 = xStack_30
                fVar16 = quest:GetHealth(pCVar10)
                fVar2 = 0.0
                if fVar2 < fVar16 then
                    iVar21 = 0
                    iVar19 = 1
                    iVar18 = 0
                    iVar17 = 0
                    iVar12 = quest:GetHero()
                    r10 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                    iVar12 = me:IsPerformingScriptTask()
                    cVar5 = iVar12
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e4568a end
                        iVar12 = me:IsPerformingScriptTask()
                        cVar5 = iVar12
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_dc)
                        goto LAB_00e469cb
                    end
                end
                __native_entity_state:SetStateInt("NextLine", __native_entity_state:GetStateInt("NextLine") + 1)
                if xStack_e8_2_b3 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        goto LAB_00e4568a
                    end
                    goto FLOW_hoist_lab_00e4568a_1
                end
                goto FLOW_past_lab_00e4568a
                ::LAB_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_dc)
                goto LAB_00e469cb
                ::FLOW_hoist_lab_00e4568a_1::
                resources:PrepareResource(xStack_118)
                ::FLOW_past_lab_00e4568a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_dc)
            end
            cVar5 = me:MsgIsHitByHero()
            if not cVar5 then
                -- TODO(native): bVar4 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                bVar4 = nil --[[unresolved native value]]
                if bVar4 then
                    -- TODO(native): bVar4 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                    bVar4 = nil --[[unresolved native value]]
                    if not bVar4 then goto LAB_00e453d2 end
                end
                -- TODO(native): xStack_c4_b3 = '\0';
            else
                goto LAB_00e453d2
            end
            goto FLOW_past_lab_00e453d2
            ::LAB_00e453d2::
            -- TODO(native): xStack_c4_b3 = '\x01';
            ::FLOW_past_lab_00e453d2::
            if xStack_c4_b3 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                me:ClearCommands()
                resources:PrepareResource(xStack_118)
                cVar5 = me:AcquireControl(4)
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e469cb end
                    cVar5 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e469cb end
                xStack_8c = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                xStack_40 = resources:ScriptThing(xStack_118)
                pCVar10 = xStack_40
                fVar16 = quest:GetHealth(pCVar10)
                fVar2 = 0.0
                if fVar2 < fVar16 then
                    iVar21 = 0
                    iVar19 = 1
                    iVar18 = 0
                    iVar17 = 0
                    pcVar20 = "_ON_HIT"
                    pCVar7 = tostring(__native_entity_state:GetStateInt("ClientID") + 1)
                    pCVar7 = ("TEXT_QST_B13_WOMAN" .. pCVar7)
                    pCVar7 = (pCVar7 .. pcVar20)
                    pvVar11 = pCVar7
                    iVar12 = quest:GetHero()
                    r11 = me:Speak(iVar12, pvVar11, iVar17, (iVar18 ~= 0), (iVar19 ~= 0), (iVar21 ~= 0))
                    iVar12 = me:IsPerformingScriptTask()
                    cVar5 = iVar12
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e456ca end
                        iVar12 = me:IsPerformingScriptTask()
                        cVar5 = iVar12
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_8c)
                        goto LAB_00e469cb
                    end
                end
                if xStack_e8_2_b3 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        goto LAB_00e456ca
                    end
                    goto FLOW_hoist_lab_00e456ca_1
                end
                goto FLOW_past_lab_00e456ca
                ::LAB_00e456ca::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_8c)
                goto LAB_00e469cb
                ::FLOW_hoist_lab_00e456ca_1::
                resources:PrepareResource(xStack_118)
                ::FLOW_past_lab_00e456ca::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_8c)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
        end
        quest:DeregisterTimer(iVar6)
    end
    goto FLOW_past_lab_00e469cb
    ::LAB_00e469cb::
    quest:DeregisterTimer(iVar6)
    ::FLOW_past_lab_00e469cb::
    ::LAB_00e469d4::
    ::LAB_00e469e6::
    resources:ReleaseResource(xStack_118)
end

function Init(quest, me)
    local iVar3
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + 1)
    __native_entity_state:SetStateInt("BrainState", 1)
    local iVar2 = math.random(0, 32767)
    iVar3 = 0
    __native_entity_state:SetStateInt("NextLine", 1)
    __native_entity_state:SetStateInt("ClientID", 0)
    __native_entity_state:SetStateInt("SexChance", iVar2 % 5)
    local pcVar4 = __element("ClientInUse", 0)
    repeat
        -- TODO(native): if *pcVar4 == 0 then
        if false then
            __native_entity_state:SetStateInt("ClientID", iVar3)
            -- TODO(native): *(undefined1 *)(*(int *)(this + 0x14) + 0x58 + iVar3) = 1;
            break
        end
        iVar3 = iVar3 + 1
        pcVar4 = pcVar4 + 1
    until not (iVar3 < 3)
    quest:SetThingPersistent(me, true)
    quest:EntitySetOpinionReactionMask(me, "OPINION_REACTION_MASK_DONT_SCREAM")
end

function OnPersist(quest, me, context)
    local brainState = quest:GetStateInt("BrainState") or 0
    brainState = quest:PersistTransferInt(context, "BrainState", brainState)
    quest:SetStateInt("BrainState", brainState)
    local clientID = quest:GetStateInt("ClientID") or 0
    clientID = quest:PersistTransferInt(context, "ClientID", clientID)
    quest:SetStateInt("ClientID", clientID)
    local sexChance = quest:GetStateInt("SexChance") or 0
    sexChance = quest:PersistTransferInt(context, "SexChance", sexChance)
    quest:SetStateInt("SexChance", sexChance)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + -1)
    -- TODO(native): *(undefined1 *)(*(int *)(this + 0x20) + 0x58 + *(int *)(this + 0x14)) = 0;
end

