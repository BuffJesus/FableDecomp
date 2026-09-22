-- Generated native draft: IngredientOwner. Review coverage report before use.
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
    local bUnknown, bVar3, cVar4, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar10, iVar11, iVar13, iVar8, native_arg_sequence_1, pCVar14, pCVar5, pCVar6, pCVar9, pcVar7, r1, r2, r3, r4, r5, r6, r7, r8, xStack_70, xStack_80, xStack_90, xStack_cc, xStack_e8, xStack_ec, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_e8 = resources:NewResource()
    resources:PrepareResource(xStack_e8)
    bVar3 = resources:TryAcquire(xStack_e8, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00ecbc5f end
        bVar3 = resources:TryAcquire(xStack_e8, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00ecbc5f end
    r1 = quest:GetThingWithScriptName("Ingredient")
    if not __native_entity_state:GetStateBool("BoughtSpecialStuff") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pCVar5 = 0x0
            quest:SetThingHasInformation(me, false, true, false)
            goto LAB_00ecb222
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pCVar5 = (0x0 + 4)
            quest:RemoveThing(r1, (pCVar5 ~= 0), false)
            goto LAB_00ecb222
        end
    end
    goto FLOW_past_lab_00ecb222
    ::LAB_00ecb222::
    bVar3 = false
    quest:SetIsPushableByHero(me, bVar3)
    quest:SetIsThingForcePushable(me, false)
    xStack_ec = quest:RegisterTimer()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    while not bVar3 do
        cVar4 = me:IsTalkedToByHero()
        if cVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if not __native_entity_state:GetStateBool("BoughtSpecialStuff") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00ecbc4d end
                    xStack_cc = resources:StartMovie("")
                    pCVar14 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_30 = resources:ScriptThing(xStack_e8)
                    pCVar5 = x_stk_30
                    fret_0 = quest:GetHealth(pCVar5)
                    fVar2 = 0.0
                    if fret_0 <= fVar2 then
                        goto LAB_00ecb3b6
                    else
                        iVar13 = 0
                        iVar11 = 1
                        iVar10 = 0
                        iVar8 = 0
                        pcVar7 = "TEXT_QST_B10_TRADER_INTRO"
                        pCVar5 = quest:GetHero()
                        r2 = me:Speak(pCVar5, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar4 = iVar8
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ecbbc9 end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar4 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then goto LAB_00ecb3b6 end
                    end
                    goto FLOW_past_lab_00ecb3b6
                    ::LAB_00ecb3b6::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar8 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ecbbc9 end
                        iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        if iVar8 == 1 then
                            iVar8 = quest:GetHeroGold()
                            if iVar8 < 0x5dc then
                                iVar8 = quest:GetHeroGold()
                                if 0x5db < iVar8 then goto LAB_00ecb6d9 end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00ecb6c3 end
                                x_stk_54 = resources:ScriptThing(xStack_e8)
                                pCVar5 = x_stk_54
                                fret_01 = quest:GetHealth(pCVar5)
                                fVar2 = 0.0
                                if fVar2 < fret_01 then
                                    iVar13 = 0
                                    iVar11 = 1
                                    iVar10 = 0
                                    iVar8 = 0
                                    pcVar7 = "TEXT_QST_B10_TRADER_NOT_ENOUGH_MONEY_10"
                                    pCVar14 = quest:GetHero()
                                    r3 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                                    iVar8 = me:IsPerformingScriptTask()
                                    cVar4 = iVar8
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00ecbbc9 end
                                        iVar8 = me:IsPerformingScriptTask()
                                        cVar4 = iVar8
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00ecb6c3 end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00ecbbc9 end
                                -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)&xStack_ac);
                                quest:GiveHeroObject(pcVar7, -1, false)
                                quest:RemoveThing(pCVar14, false, false)
                                quest:GiveHeroGold(-0x5dc)
                                quest:EntityGiveGold(me, 0x5dc)
                                x_stk_60 = resources:ScriptThing(xStack_e8)
                                pCVar5 = x_stk_60
                                fret_00 = quest:GetHealth(pCVar5)
                                fVar2 = 0.0
                                if fVar2 < fret_00 then
                                    iVar13 = 0
                                    iVar11 = 1
                                    iVar10 = 0
                                    iVar8 = 0
                                    pcVar7 = "TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10"
                                    pCVar14 = quest:GetHero()
                                    r4 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                                    iVar8 = me:IsPerformingScriptTask()
                                    cVar4 = iVar8
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00ecb6c3 end
                                        iVar8 = me:IsPerformingScriptTask()
                                        cVar4 = iVar8
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00ecbbc9 end
                                end
                                __native_entity_state:SetStateBool("BoughtSpecialStuff", true)
                                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x770))
                                quest:ClearThingHasInformation(me)
                            end
                        else
                            goto LAB_00ecb6d9
                        end
                        goto FLOW_past_lab_00ecb6d9
                        ::LAB_00ecb6d9::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            goto LAB_00ecbbc9
                        end
                        goto FLOW_hoist_lab_00ecbbc9_1
                        ::FLOW_past_lab_00ecb6d9::
                        goto FLOW_hoist_lab_00ecbbc9_2
                    end
                    goto FLOW_past_lab_00ecbbc9
                    ::LAB_00ecbbc9::
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar6 = xStack_cc
                    goto LAB_00ecbc48
                    ::FLOW_hoist_lab_00ecbbc9_1::
                    x_stk_3c = resources:ScriptThing(xStack_e8)
                    pCVar5 = x_stk_3c
                    fret_02 = quest:GetHealth(pCVar5)
                    fVar2 = 0.0
                    if fVar2 < fret_02 then
                        iVar13 = 0
                        iVar11 = 1
                        iVar10 = 0
                        iVar8 = 0
                        pcVar7 = "TEXT_QST_B10_TRADER_NOT_BOUGHT_MUSHROOM_10"
                        pCVar14 = quest:GetHero()
                        r5 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar4 = iVar8
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00ecb6c3 end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar4 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ecbbc9 end
                    end
                    ::FLOW_hoist_lab_00ecbbc9_2::
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar6 = xStack_cc
                    goto LAB_00ecb9e6
                    ::FLOW_past_lab_00ecbbc9::
                    ::FLOW_past_lab_00ecb3b6::
                    ::LAB_00ecb6c3::
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar6 = xStack_cc
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00ecbc4d end
                    if __native_entity_state:GetStateBool("StoleSpecialStuff") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00ecbc4d end
                        xStack_70 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_c = resources:ScriptThing(xStack_e8)
                        pCVar14 = x_stk_c
                        fret_04 = quest:GetHealth(pCVar14)
                        fVar2 = 0.0
                        if fVar2 < fret_04 then
                            iVar13 = 0
                            iVar11 = 1
                            iVar10 = 0
                            iVar8 = 0
                            pcVar7 = "TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10"
                            pCVar14 = quest:GetHero()
                            r6 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                            iVar8 = me:IsPerformingScriptTask()
                            cVar4 = iVar8
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar6 = xStack_70
                                    goto LAB_00ecbc48
                                end
                                iVar8 = me:IsPerformingScriptTask()
                                cVar4 = iVar8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar6 = xStack_70
                                goto LAB_00ecbc48
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        pCVar6 = xStack_70
                        goto LAB_00ecb9e6
                    end
                    goto FLOW_hoist_lab_00ecb9e6_1
                end
                goto FLOW_past_lab_00ecb9e6
                ::LAB_00ecb9e6::
                resources:DestroyMovie(pCVar6)
                goto LAB_00ecb9eb
                ::FLOW_hoist_lab_00ecb9e6_1::
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00ecbc4d end
                xStack_80 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_24 = resources:ScriptThing(xStack_e8)
                pCVar14 = x_stk_24
                fret_03 = quest:GetHealth(pCVar14)
                fVar2 = 0.0
                if fret_03 <= fVar2 then
                    goto LAB_00ecb8bb
                end
                goto FLOW_past_lab_00ecb8bb
                ::LAB_00ecb8bb::
                quest:PauseAllNonScriptedEntities(false)
                pCVar6 = xStack_80
                goto LAB_00ecb9e6
                ::FLOW_past_lab_00ecb8bb::
                iVar13 = 0
                iVar11 = 1
                iVar10 = 0
                iVar8 = 0
                pcVar7 = "TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10"
                pCVar14 = quest:GetHero()
                r7 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                iVar8 = me:IsPerformingScriptTask()
                cVar4 = iVar8
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        pCVar6 = xStack_80
                        goto LAB_00ecbc48
                    end
                    iVar8 = me:IsPerformingScriptTask()
                    cVar4 = iVar8
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then goto LAB_00ecb8bb end
                quest:PauseAllNonScriptedEntities(false)
                pCVar6 = xStack_80
                ::FLOW_past_lab_00ecb9e6::
                goto LAB_00ecbc48
            end
            goto FLOW_hoist_lab_00ecbc48_1
        end
        goto FLOW_past_lab_00ecbc48
        ::LAB_00ecbc48::
        resources:DestroyMovie(pCVar6)
        goto LAB_00ecbc4d
        ::FLOW_hoist_lab_00ecbc48_1::
        goto FLOW_hoist_lab_00ecbc4d_1
        ::FLOW_past_lab_00ecbc48::
        goto FLOW_past_lab_00ecbc4d
        ::LAB_00ecbc4d::
        quest:DeregisterTimer(xStack_ec)
        goto LAB_00ecbc56
        ::FLOW_hoist_lab_00ecbc4d_1::
        break
        ::FLOW_past_lab_00ecbc4d::
        ::LAB_00ecb9eb::
        native_arg_sequence_1 = false
        if not __native_entity_state:GetStateBool("BoughtSpecialStuff") then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            -- TODO(native): cVar4 = (**(CStack_d8._0_4_ + 0x12c))()
            cVar4 = nil --[[unresolved native value]]
            if cVar4 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ecbc4d end
            xStack_90 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            x_stk_48 = resources:ScriptThing(xStack_e8)
            pCVar14 = x_stk_48
            fret_05 = quest:GetHealth(pCVar14)
            fVar2 = 0.0
            if fVar2 < fret_05 then
                iVar13 = 0
                iVar11 = 1
                iVar10 = 0
                iVar8 = 0
                pcVar7 = "TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10"
                pCVar14 = quest:GetHero()
                r8 = me:Speak(pCVar14, pcVar7, iVar8, (iVar10 ~= 0), (iVar11 ~= 0), (iVar13 ~= 0))
                iVar8 = me:IsPerformingScriptTask()
                cVar4 = iVar8
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00ecbc44
                    end
                    iVar8 = me:IsPerformingScriptTask()
                    cVar4 = iVar8
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00ecbc44
                end
                goto FLOW_past_lab_00ecbc44
                ::LAB_00ecbc44::
                pCVar6 = xStack_90
                goto LAB_00ecbc48
                ::FLOW_past_lab_00ecbc44::
            end
            __native_entity_state:SetStateBool("BoughtSpecialStuff", true)
            __native_entity_state:SetStateBool("StoleSpecialStuff", true)
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0x76c))
            quest:ClearThingHasInformation(me)
            iVar11 = 7
            pCVar9 = 0x0
            pCVar5 = 0x0
            bUnknown = 0x1
            iVar10 = 6
            pCVar14 = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
            quest:AddCrimeCommitted(pCVar14, iVar10, (bUnknown ~= 0), nil --[[missing]], nil --[[missing]], pCVar5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_90)
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    end
    quest:DeregisterTimer(xStack_ec)
    ::FLOW_past_lab_00ecb222::
    ::LAB_00ecbc56::
    ::LAB_00ecbc5f::
    resources:ReleaseResource(xStack_e8)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("BoughtSpecialStuff", false)
    __native_entity_state:SetStateBool("StoleSpecialStuff", false)
    if not quest:GetStateBool("OwnerAlive") then
        quest:RemoveThing(me, false, true)
    end
end

function OnPersist(quest, me, context)
    local boughtSpecialStuff = quest:GetStateBool("BoughtSpecialStuff") or false
    boughtSpecialStuff = quest:PersistTransferBool(context, "BoughtSpecialStuff", boughtSpecialStuff)
    quest:SetStateBool("BoughtSpecialStuff", boughtSpecialStuff)
    local stoleSpecialStuff = quest:GetStateBool("StoleSpecialStuff") or false
    stoleSpecialStuff = quest:PersistTransferBool(context, "StoleSpecialStuff", stoleSpecialStuff)
    quest:SetStateBool("StoleSpecialStuff", stoleSpecialStuff)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetStateBool("OwnerAlive", false)
    end
end

