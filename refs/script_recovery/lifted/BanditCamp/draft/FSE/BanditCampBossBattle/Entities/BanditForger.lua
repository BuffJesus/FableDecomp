-- Generated native draft: BanditForger. Review coverage report before use.
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
    local CVar8, CVar9, bVar2, bVar3, cVar4, c_stk_69, c_stk_7d, c_stk_85, dist, fVar1, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, iVar12, iVar13, iVar6, iVar7, i_stk_20, i_stk_a0, pCVar5, pThing, pcVar11, piVar10, pi_stk_1c, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, xStack_3c, xStack_68, xStack_7c, xStack_8c, xStack_9c, x_stk_18, x_stk_38, x_stk_c
    local alive = true
    local function __cleanup_LAB_00d0d3c8()
        quest:DeregisterTimer(i_stk_a0)
        resources:ReleaseResource(xStack_9c)
    end
    CVar8 = 0
    c_stk_7d = 1
    c_stk_69 = 0
    c_stk_85 = 1
    xStack_9c = resources:NewResource()
    resources:PrepareResource(xStack_9c)
    bVar2 = resources:TryAcquire(xStack_9c, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_9c)
            return
        end
        bVar2 = resources:TryAcquire(xStack_9c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        resources:ReleaseResource(xStack_9c)
        return
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d0d8d1 end
    i_stk_a0 = quest:RegisterTimer()
    quest:SetTimer(i_stk_a0, 0)
    if not __native_entity_state:GetStateBool("GivenPass") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0d8c8 end
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
        __native_entity_state:SetStateBool("GivenPass", true)
    end
    repeat
        if c_stk_85 == 0 then goto LAB_00d0d04e end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0d8c8 end
        CVar9 = CVar8 | 1
        bVar2 = me:MsgIsHitByHero()
        if bVar2 then
            goto LAB_00d0c68f
        else
            CVar9 = CVar8 | 3
            bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar2 then
                CVar9 = CVar8 | 7
                bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar2 then goto LAB_00d0c68f end
            end
            pCVar5 = quest:GetHero()
            bVar3 = quest:AreEntitiesEnemies(me, pCVar5)
            bVar2 = false
            if bVar3 then goto LAB_00d0c68f end
        end
        goto FLOW_past_lab_00d0c68f
        ::LAB_00d0c68f::
        bVar2 = true
        ::FLOW_past_lab_00d0c68f::
        if (CVar9 & 4) ~= 0 then
            CVar9 = CVar9 & 0xfffffffb
        end
        if (CVar9 & 2) ~= 0 then
            CVar9 = CVar9 & 0xfffffffd
        end
        if (CVar9 & 1) ~= 0 then
            CVar9 = CVar9 & 0xfffffffe
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0d8c8 end
            c_stk_85 = 0
        end
        iVar6 = quest:GetTimer(i_stk_a0)
        if iVar6 < 1 then
            dist = 10.0
            pCVar5 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, dist)
            if (not bVar2) or (quest:GetStateBool("Gate2Open")) then goto LAB_00d0c766 end
            CVar9 = CVar9 | 8
            pCVar5 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
            bVar2 = true
            if bVar3 then goto LAB_00d0c766 end
        else
            goto LAB_00d0c766
        end
        goto FLOW_past_lab_00d0c766
        ::LAB_00d0c766::
        bVar2 = false
        ::FLOW_past_lab_00d0c766::
        if (CVar9 & 8) ~= 0 then
            CVar9 = CVar9 & 0xfffffff7
            xStack_8c = CVar9
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0d8c8 end
            bVar2 = false
            pCVar5 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar2)
            me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
            iVar6 = me:IsPerformingScriptTask()
            cVar4 = iVar6
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then __cleanup_LAB_00d0d3c8(); return end
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0d8c8 end
            if c_stk_69 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0d8c8 end
                iVar7 = quest:AddNewConversation(me, false, false)
                pCVar5 = quest:GetHero()
                quest:AddPersonToConversation(iVar7, pCVar5)
                pCVar5 = quest:GetHero()
                quest:AddLineToConversation(iVar7, "TEXT_QST_009_FORGER_ASIDE_FIRST", me, pCVar5, false)
                c_stk_69 = 1
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0d8c8 end
                iVar7 = quest:AddNewConversation(me, false, false)
                pCVar5 = quest:GetHero()
                quest:AddPersonToConversation(iVar7, pCVar5)
                pCVar5 = quest:GetHero()
                quest:AddLineToConversation(iVar7, "TEXT_QST_009_FORGER_ASIDE_SECOND", me, pCVar5, false)
                c_stk_69 = 0
            end
            quest:SetTimer(i_stk_a0, 10)
        end
        bVar2 = me:IsTalkedToByHero()
    until not (not bVar2)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_7c = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if not quest:GetStateBool("Gate2Open") then
            pCVar5 = quest:GetHero()
            bVar2 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar2 then
                if bVar3 then
                    -- LAB_00d0d377: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                else
                    x_stk_38 = resources:ScriptThing(xStack_9c)
                    pCVar5 = x_stk_38
                    fret_01 = quest:GetHealth(pCVar5)
                    fVar1 = 0.0
                    if fVar1 < fret_01 then
                        iVar13 = 0
                        iVar12 = 1
                        iVar7 = 0
                        iVar6 = 0
                        pcVar11 = "TEXT_QST_009_FORGER_CHAT"
                        pCVar5 = quest:GetHero()
                        r1 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar4 = iVar6
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00d0cb83 end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d0d377
                        end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_CHAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar6 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d0cb83 end
                        iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d0d377
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if iVar6 ~= 1 then
                        if not bVar2 then
                            x_stk_38 = resources:ScriptThing(xStack_9c)
                            pCVar5 = x_stk_38
                            fret_04 = quest:GetHealth(pCVar5)
                            fVar1 = 0.0
                            if fVar1 < fret_04 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar7 = 0
                                iVar6 = 0
                                pcVar11 = "TEXT_QST_009_FORGER_TURNED_DOWN"
                                pCVar5 = quest:GetHero()
                                r2 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00d0cb83 end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                end
                                goto LAB_00d0d014
                            end
                            goto LAB_00d0d023
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d0d377
                    end
                    if not bVar2 then
                        xStack_3c = quest:GetHeroGold()
                        if xStack_3c < quest:ReadGlobalGameDataFloat(0xe84) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d0d377
                            end
                            x_stk_38 = resources:ScriptThing(xStack_9c)
                            pCVar5 = x_stk_38
                            fret_02 = quest:GetHealth(pCVar5)
                            fVar1 = 0.0
                            if fVar1 < fret_02 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar7 = 0
                                iVar6 = 0
                                pcVar11 = "TEXT_QST_009_FORGER_FAILED"
                                pCVar5 = quest:GetHero()
                                r3 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00d0cb83 end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                end
                                goto LAB_00d0d014
                            end
                            goto LAB_00d0d023
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            x_stk_38 = resources:ScriptThing(xStack_9c)
                            pCVar5 = x_stk_38
                            fret_03 = quest:GetHealth(pCVar5)
                            fVar1 = 0.0
                            if fVar1 < fret_03 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar7 = 0
                                iVar6 = 0
                                pcVar11 = "TEXT_QST_009_FORGER_SUCCESS"
                                pCVar5 = quest:GetHero()
                                r4 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto FLOW_after_lab_00d0d377
                                    end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00d0cb83 end
                            end
                            quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                            iVar7 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xe84)))
                            quest:GiveHeroGold(iVar7)
                            iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe84)))
                            quest:EntityGiveGold(me, iVar7)
                            quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                            c_stk_7d = 0
                            quest:GiveHeroExperience(quest:ReadGlobalGameData(0x3c))
                            quest:ClearThingHasInformation(me)
                            goto LAB_00d0d023
                        end
                    end
                    ::LAB_00d0cb83::
                    quest:PauseAllNonScriptedEntities(false)
                end
                ::FLOW_after_lab_00d0d377::
                resources:DestroyMovie(xStack_7c)
                goto LAB_00d0d8c8
            end
            if bVar3 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_7c)
                goto LAB_00d0d8c8
            end
            x_stk_38 = resources:ScriptThing(xStack_9c)
            pCVar5 = x_stk_38
            fret_00 = quest:GetHealth(pCVar5)
            fVar1 = 0.0
            if fVar1 < fret_00 then
                iVar13 = 0
                iVar12 = 1
                iVar7 = 0
                iVar6 = 0
                pcVar11 = "TEXT_QST_009_FORGER_EARLY_GOT_PASS"
                pCVar5 = quest:GetHero()
                r5 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        -- LAB_00d0d377_c7: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_7c)
                        goto LAB_00d0d8c8
                    end
                    iVar6 = me:IsPerformingScriptTask()
                    cVar4 = iVar6
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_7c)
                    goto LAB_00d0d8c8
                end
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00d0d377_c9: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_7c)
                goto LAB_00d0d8c8
            end
            x_stk_38 = resources:ScriptThing(xStack_9c)
            pCVar5 = x_stk_38
            fret_0 = quest:GetHealth(pCVar5)
            fVar1 = 0.0
            if fret_0 <= fVar1 then goto LAB_00d0d023 end
            iVar13 = 0
            iVar12 = 1
            iVar7 = 0
            iVar6 = 0
            pcVar11 = "TEXT_QST_009_FORGER_EARLY_NOT_NEED"
            pCVar5 = quest:GetHero()
            r6 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
            iVar6 = me:IsPerformingScriptTask()
            cVar4 = iVar6
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_7c)
                    __cleanup_LAB_00d0d3c8()
                    return
                end
                iVar6 = me:IsPerformingScriptTask()
                cVar4 = iVar6
            end
            goto LAB_00d0d014
        end
        goto FLOW_past_lab_00d0d014
        ::LAB_00d0d014::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00d0d377_c10: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_7c)
            goto LAB_00d0d8c8
        end
        ::FLOW_past_lab_00d0d014::
        ::LAB_00d0d023::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_7c)
        CVar8 = xStack_8c
        goto LAB_00d0d04e
    end
    goto FLOW_past_lab_00d0d04e
    ::LAB_00d0d04e::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        ::LAB_00d0d063::
        repeat
            if (c_stk_7d == 0) or (c_stk_85 == 0) then goto LAB_00d0d85f end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0d8c8 end
            CVar9 = CVar8 | 0x10
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                goto LAB_00d0d12f
            else
                CVar9 = CVar8 | 0x30
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    CVar9 = CVar8 | 0x70
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar2 then goto LAB_00d0d12f end
                end
                pCVar5 = quest:GetHero()
                bVar3 = quest:AreEntitiesEnemies(me, pCVar5)
                bVar2 = false
                if bVar3 then goto LAB_00d0d12f end
            end
            goto FLOW_past_lab_00d0d12f
            ::LAB_00d0d12f::
            bVar2 = true
            ::FLOW_past_lab_00d0d12f::
            if (CVar9 & 0x40) ~= 0 then
                CVar9 = CVar9 & 0xffffffbf
            end
            if (CVar9 & 0x20) ~= 0 then
                CVar9 = CVar9 & 0xffffffdf
            end
            if (CVar9 & 0x10) ~= 0 then
                CVar9 = CVar9 & 0xffffffef
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0d8c8 end
                c_stk_85 = 0
            end
            bVar2 = me:IsTalkedToByHero()
        until not (not bVar2)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0d8c8 end
        xStack_68 = resources:StartMovie("")
        pi_stk_1c = piVar10
        quest:PauseAllNonScriptedEntities(true)
        if not quest:GetStateBool("Gate2Open") then
            -- TODO(native): xStack_8c = xStack_8c | 0x80;
            pCVar5 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
            bVar2 = false
            if bVar3 then goto LAB_00d0d27c end
        else
            goto LAB_00d0d27c
        end
        goto FLOW_past_lab_00d0d27c
        ::LAB_00d0d27c::
        bVar2 = true
        ::FLOW_past_lab_00d0d27c::
        if ((xStack_8c & 0x80) ~= 0) then
            -- TODO(native): xStack_8c = xStack_8c & 0xffffff7f;
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                x_stk_38 = resources:ScriptThing(xStack_9c)
                pCVar5 = x_stk_38
                fret_05 = quest:GetHealth(pCVar5)
                fVar1 = 0.0
                if fVar1 < fret_05 then
                    iVar13 = 0
                    iVar12 = 1
                    iVar7 = 0
                    iVar6 = 0
                    pcVar11 = "TEXT_QST_009_FORGER_LATE_NOT_NEED"
                    pCVar5 = quest:GetHero()
                    r7 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                    iVar6 = me:IsPerformingScriptTask()
                    cVar4 = iVar6
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d0d8e5
                        end
                        iVar6 = me:IsPerformingScriptTask()
                        cVar4 = iVar6
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0d365 end
                end
                goto LAB_00d0d829
            end
            goto FLOW_hoist_lab_00d0d829_1
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_SECOND_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar6 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0d365 end
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if iVar6 == 1 then
                        if not bVar2 then
                            i_stk_20 = quest:GetHeroGold()
                            if i_stk_20 < quest:ReadGlobalGameDataFloat(0xe84) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    x_stk_18 = resources:ScriptThing(xStack_9c)
                                    pCVar5 = x_stk_18
                                    fret_06 = quest:GetHealth(pCVar5)
                                    fVar1 = 0.0
                                    if fVar1 < fret_06 then
                                        iVar13 = 0
                                        iVar12 = 1
                                        iVar7 = 0
                                        iVar6 = 0
                                        pcVar11 = "TEXT_QST_009_FORGER_SECOND_FAIL"
                                        pCVar5 = quest:GetHero()
                                        r8 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar4 = iVar6
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar2 = not alive
                                            if bVar2 then goto LAB_00d0d365 end
                                            iVar6 = me:IsPerformingScriptTask()
                                            cVar4 = iVar6
                                        end
                                        goto LAB_00d0d81a
                                    end
                                    goto LAB_00d0d829
                                end
                                goto LAB_00d0d8e5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                x_stk_c = resources:ScriptThing(xStack_9c)
                                pCVar5 = x_stk_c
                                fret_07 = quest:GetHealth(pCVar5)
                                fVar1 = 0.0
                                if fVar1 < fret_07 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar7 = 0
                                    iVar6 = 0
                                    pcVar11 = "TEXT_QST_009_FORGER_SUCCESS"
                                    pCVar5 = quest:GetHero()
                                    r9 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d0d8e5 end
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar4 = iVar6
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00d0d365 end
                                end
                                quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                                iVar7 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xe84)))
                                quest:GiveHeroGold(iVar7)
                                iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xe84)))
                                quest:EntityGiveGold(me, iVar7)
                                quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                                quest:GiveHeroExperience(quest:ReadGlobalGameData(0x3c))
                                quest:ClearThingHasInformation(me)
                                c_stk_7d = 0
                                piVar10 = pi_stk_1c
                                goto LAB_00d0d829
                            end
                        end
                        goto LAB_00d0d365
                    end
                    if not bVar2 then
                        xStack_7c = resources:ScriptThing(xStack_9c)
                        pCVar5 = xStack_7c
                        fret_08 = quest:GetHealth(pCVar5)
                        fVar1 = 0.0
                        if fVar1 < fret_08 then
                            iVar13 = 0
                            iVar12 = 1
                            iVar7 = 0
                            iVar6 = 0
                            pcVar11 = "TEXT_QST_009_FORGER_TURNED_DOWN"
                            pCVar5 = quest:GetHero()
                            r10 = me:Speak(pCVar5, pcVar11, iVar6, (iVar7 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00d0d365 end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            goto LAB_00d0d81a
                        end
                        goto FLOW_hoist_lab_00d0d81a_1
                    end
                    goto FLOW_past_lab_00d0d81a
                    ::LAB_00d0d81a::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0d8e5 end
                    ::FLOW_hoist_lab_00d0d81a_1::
                    goto LAB_00d0d829
                    ::FLOW_past_lab_00d0d81a::
                end
            end
            ::LAB_00d0d8e5::
            quest:PauseAllNonScriptedEntities(false)
        end
        ::FLOW_after_lab_00d0d8e5::
        goto FLOW_past_lab_00d0d829
        ::LAB_00d0d829::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_68)
        goto LAB_00d0d063
        ::FLOW_hoist_lab_00d0d829_1::
        ::LAB_00d0d365::
        quest:PauseAllNonScriptedEntities(false)
        ::FLOW_past_lab_00d0d829::
        resources:DestroyMovie(xStack_68)
    end
    ::FLOW_past_lab_00d0d04e::
    ::LAB_00d0d8c8::
    quest:DeregisterTimer(i_stk_a0)
    ::LAB_00d0d8d1::
    resources:ReleaseResource(xStack_9c)
    do return end
    ::LAB_00d0d85f::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        if c_stk_85 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0d8c8 end
            quest:ClearThingHasInformation(me)
            pCVar5 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pCVar5)
        end
        resources:PrepareResource(xStack_9c)
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        until not (not bVar2)
    end
    goto LAB_00d0d8c8
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GivenPass", false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
    local givenPass = quest:GetStateBool("GivenPass") or false
    givenPass = quest:PersistTransferBool(context, "GivenPass", givenPass)
    quest:SetStateBool("GivenPass", givenPass)
end

function OnPredicateFail(quest, me)
end

