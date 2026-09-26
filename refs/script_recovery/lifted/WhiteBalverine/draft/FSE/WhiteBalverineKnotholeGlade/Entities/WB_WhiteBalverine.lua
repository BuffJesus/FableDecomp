-- Generated native draft: WB_WhiteBalverine. Review coverage report before use.
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
    local bVar1, bVar10, cVar2, fVar11, iVar4, iVar8, p0, pCVar3, pCVar5, r1, r2, r3, r4, r5, r6, r7, timerId, xStack_104, xStack_58, xStack_64, xStack_f0, x_stk_30, x_stk_3c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        xStack_104 = resources:NewResource()
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAsDamageable(me, false)
        quest:OpinionSourceSetAsExclusive(me, true)
        quest:EntitySetTargetable(me, true)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        while true do
            if bVar1 then
                resources:ReleaseResource(xStack_104)
                return
            end
            if quest:GetStateInt("BalverineState") == 2 then break end
            -- LAB_00e15f22: (native jump target)
            if quest:GetStateInt("BalverineState") == 4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if not bVar1 then
                    quest:SetThingPersistent(me, true)
                    pCVar3 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(me, pCVar3)
                    quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
                    iVar8 = 0
                    iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
                    __native_entity_state:SetStateInt("WBCounter", iVar4)
                    quest:DisplayQuestInfo(true)
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then goto LAB_00e16971 end
                        bVar1 = me:MsgIsHitByHero()
                        if bVar1 then
                            goto LAB_00e16078
                        else
                            bVar1 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if bVar1 then
                                bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not bVar1 then goto LAB_00e16078 end
                            end
                            bVar1 = false
                        end
                        goto FLOW_past_lab_00e16078
                        ::LAB_00e16078::
                        bVar1 = true
                        ::FLOW_past_lab_00e16078::
                        if bVar1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16971 end
                            iVar8 = iVar8 + 1
                            quest:UpdateQuestInfoCounter(__native_entity_state:GetStateInt("WBCounter"), iVar8, -1)
                        end
                    until not (iVar8 < 10)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("WBCounter"))
                        quest:DisplayQuestInfo(false)
                        quest:EntitySetTargetable(me, false)
                        quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
                        resources:PrepareResource(xStack_104)
                        bVar1 = resources:TryAcquire(xStack_104, me, 4)
                        while not bVar1 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16971 end
                            bVar1 = resources:TryAcquire(xStack_104, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if not bVar1 then
                            pCVar3 = quest:GetThingWithScriptName("WB_EscapePoint2")
                            pCVar5 = pCVar3:GetPos()
                            xStack_58 = {x = pCVar5.x, y = pCVar5.y, z = pCVar5.z}
                            fVar11 = 2.5
                            x_stk_3c = resources:ScriptThing(xStack_104)
                            pCVar3 = x_stk_3c
                            bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_58, fVar11))
                            if bVar1 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if bVar1 then goto LAB_00e16971 end
                                    me:MoveToPosition(xStack_58, 0.5, 1, false, false)
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar2 = iVar4
                                    while cVar2 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if bVar1 then goto LAB_00e16971 end
                                        iVar4 = me:IsPerformingScriptTask()
                                        cVar2 = iVar4
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if bVar1 then goto LAB_00e16971 end
                                    fVar11 = 2.5
                                    x_stk_3c = resources:ScriptThing(xStack_104)
                                    pCVar3 = x_stk_3c
                                    bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_58, fVar11))
                                until not (bVar1)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if not bVar1 then
                                me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
                                iVar4 = me:IsPerformingScriptTask()
                                cVar2 = iVar4
                                while cVar2 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if bVar1 then goto LAB_00e16971 end
                                    iVar4 = me:IsPerformingScriptTask()
                                    cVar2 = iVar4
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if not bVar1 then
                                    me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
                                    quest:Pause(1.0)
                                    bVar10 = false
                                    bVar1 = false
                                    fVar11 = 0.0
                                    pCVar5 = me:GetPos()
                                    r1 = quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", pCVar5, fVar11, bVar1)
                                    quest:RemoveThing(me, false, true)
                                    quest:SetStateInt("BalverineState", 5)
                                    goto LAB_00e1647d
                                end
                            end
                        end
                    end
                end
                goto LAB_00e16971
            end
            ::LAB_00e1647d::
            if quest:GetStateInt("BalverineState") == 6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if not bVar1 then
                    r2 = quest:GetThingWithScriptName("WB_Guard1")
                    resources:PrepareResource(xStack_104)
                    bVar1 = resources:TryAcquire(xStack_104, me, 4)
                    while not bVar1 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then goto LAB_00e16964 end
                        bVar1 = resources:TryAcquire(xStack_104, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
                        resources:PrepareResource(xStack_104)
                        quest:SetThingPersistent(me, true)
                        pCVar3 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar3)
                        iVar8 = 0
                        iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 5, 1.0)
                        __native_entity_state:SetStateInt("WBCounter", iVar4)
                        quest:DisplayQuestInfo(true)
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16964 end
                            bVar1 = me:MsgIsHitByHero()
                            if bVar1 then
                                goto LAB_00e16666
                            else
                                bVar1 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                if bVar1 then
                                    bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                    if not bVar1 then goto LAB_00e16666 end
                                end
                                bVar1 = false
                            end
                            goto FLOW_past_lab_00e16666
                            ::LAB_00e16666::
                            bVar1 = true
                            ::FLOW_past_lab_00e16666::
                            if bVar1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if bVar1 then goto LAB_00e16964 end
                                iVar8 = iVar8 + 1
                                quest:UpdateQuestInfoCounter(__native_entity_state:GetStateInt("WBCounter"), iVar8, -1)
                            end
                        until not (iVar8 < 5)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if not bVar1 then
                            quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("WBCounter"))
                            quest:DisplayQuestInfo(false)
                            resources:PrepareResource(xStack_104)
                            bVar1 = resources:TryAcquire(xStack_104, me, 4)
                            while not bVar1 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if bVar1 then goto LAB_00e16964 end
                                bVar1 = resources:TryAcquire(xStack_104, me, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if not bVar1 then
                                r3 = quest:GetThingWithScriptName("WB_RunPoint2")
                                xStack_f0 = quest:RegisterTimer()
                                timerId = xStack_f0
                                quest:SetTimer(xStack_f0, 5)
                                iVar4 = quest:GetTimer(timerId)
                                while 0 < iVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if bVar1 then goto LAB_00e1694f end
                                    iVar4 = me:IsPerformingScriptTask()
                                    if not iVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if bVar1 then goto LAB_00e1694f end
                                        me:MoveToThing(r3, 0, 1)
                                    end
                                    pCVar5 = me:GetPos()
                                    bVar1 = quest:IsCameraPosOnScreen(pCVar5)
                                    if bVar1 then
                                        iVar4 = me:IsPerformingScriptTask()
                                        if not iVar4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if not bVar1 then
                                                quest:SetStateInt("BalverineState", 7)
                                                goto LAB_00e1687b
                                            end
                                            goto LAB_00e1694f
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if bVar1 then goto LAB_00e1694f end
                                        quest:SetStateInt("BalverineState", 7)
                                        goto LAB_00e1687b
                                    end
                                    goto FLOW_past_lab_00e1687b
                                    ::LAB_00e1687b::
                                    quest:FadeOutAndKillEntity(me, true, 3.0, true)
                                    ::FLOW_past_lab_00e1687b::
                                    iVar4 = quest:GetTimer(xStack_f0)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if not bVar1 then
                                    quest:SetStateInt("BalverineState", 7)
                                    quest:FadeOutAndKillEntity(me, true, 3.0, true)
                                    quest:DeregisterTimer(timerId)
                                    goto LAB_00e168ed
                                end
                                ::LAB_00e1694f::
                                quest:DeregisterTimer(timerId)
                            end
                        end
                    end
                    ::LAB_00e16964::
                end
                goto LAB_00e16971
            end
            ::LAB_00e168ed::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
        end
        ::FLOW_after_lab_00e15f22::
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetThingPersistent(me, true)
            pCVar3 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pCVar3)
            iVar8 = 0
            iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
            __native_entity_state:SetStateInt("WBCounter", iVar4)
            quest:DisplayQuestInfo(true)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then goto LAB_00e16971 end
                bVar1 = me:MsgIsHitByHero()
                if bVar1 then
                    goto LAB_00e15b1a
                else
                    bVar1 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar1 then
                        bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar1 then goto LAB_00e15b1a end
                    end
                    bVar1 = false
                end
                goto FLOW_past_lab_00e15b1a
                ::LAB_00e15b1a::
                bVar1 = true
                ::FLOW_past_lab_00e15b1a::
                if bVar1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then goto LAB_00e16971 end
                    iVar8 = iVar8 + 1
                    quest:UpdateQuestInfoCounter(__native_entity_state:GetStateInt("WBCounter"), iVar8, -1)
                end
            until not (iVar8 < 10)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("WBCounter"))
                quest:DisplayQuestInfo(false)
                quest:EntitySetTargetable(me, false)
                quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
                resources:PrepareResource(xStack_104)
                bVar1 = resources:TryAcquire(xStack_104, me, 4)
                while not bVar1 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then goto LAB_00e16971 end
                    bVar1 = resources:TryAcquire(xStack_104, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if not bVar1 then
                    pCVar3 = quest:GetThingWithScriptName("WB_EscapePoint1")
                    pCVar5 = pCVar3:GetPos()
                    xStack_64 = {x = pCVar5.x, y = pCVar5.y, z = pCVar5.z}
                    fVar11 = 2.5
                    x_stk_30 = resources:ScriptThing(xStack_104)
                    pCVar3 = x_stk_30
                    bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_64, fVar11))
                    if bVar1 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16971 end
                            me:MoveToPosition(xStack_64, 0.5, 1, false, false)
                            iVar4 = me:IsPerformingScriptTask()
                            cVar2 = iVar4
                            while cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if bVar1 then goto LAB_00e16971 end
                                iVar4 = me:IsPerformingScriptTask()
                                cVar2 = iVar4
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16971 end
                            fVar11 = 2.5
                            x_stk_30 = resources:ScriptThing(xStack_104)
                            pCVar3 = x_stk_30
                            bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_64, fVar11))
                        until not (bVar1)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
                        iVar4 = me:IsPerformingScriptTask()
                        cVar2 = iVar4
                        while cVar2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e16971 end
                            iVar4 = me:IsPerformingScriptTask()
                            cVar2 = iVar4
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if not bVar1 then
                            me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
                            quest:Pause(1.0)
                            bVar10 = false
                            bVar1 = false
                            fVar11 = 0.0
                            pCVar5 = me:GetPos()
                            r4 = quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", pCVar5, fVar11, bVar1)
                            quest:RemoveThing(me, false, true)
                            quest:SetStateInt("BalverineState", 3)
                            if quest:GetStateInt("BalverineState") == 4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if not bVar1 then
                                    quest:SetThingPersistent(me, true)
                                    pCVar3 = quest:GetHero()
                                    quest:GiveThingBestEnemyTarget(me, pCVar3)
                                    quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
                                    iVar8 = 0
                                    iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 10, 1.0)
                                    __native_entity_state:SetStateInt("WBCounter", iVar4)
                                    quest:DisplayQuestInfo(true)
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if bVar1 then goto LAB_00e16971 end
                                        bVar1 = me:MsgIsHitByHero()
                                        if bVar1 then
                                            goto LAB_00e16078_c1
                                        else
                                            bVar1 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                            if bVar1 then
                                                bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                                if not bVar1 then goto LAB_00e16078_c1 end
                                            end
                                            bVar1 = false
                                        end
                                        goto FLOW_past_lab_00e16078_c1
                                        ::LAB_00e16078_c1::
                                        bVar1 = true
                                        ::FLOW_past_lab_00e16078_c1::
                                        if bVar1 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if bVar1 then goto LAB_00e16971 end
                                            iVar8 = iVar8 + 1
                                            quest:UpdateQuestInfoCounter(__native_entity_state:GetStateInt("WBCounter"), iVar8, -1)
                                        end
                                    until not (iVar8 < 10)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if not bVar1 then
                                        quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("WBCounter"))
                                        quest:DisplayQuestInfo(false)
                                        quest:EntitySetTargetable(me, false)
                                        quest:EntitySetCombatType(me, "WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE")
                                        resources:PrepareResource(xStack_104)
                                        bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                        while not bVar1 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if bVar1 then goto LAB_00e16971 end
                                            bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if not bVar1 then
                                            pCVar3 = quest:GetThingWithScriptName("WB_EscapePoint2")
                                            pCVar5 = pCVar3:GetPos()
                                            xStack_58 = {x = pCVar5.x, y = pCVar5.y, z = pCVar5.z}
                                            fVar11 = 2.5
                                            x_stk_3c = resources:ScriptThing(xStack_104)
                                            pCVar3 = x_stk_3c
                                            bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_58, fVar11))
                                            if bVar1 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar1 = not alive
                                                    if bVar1 then goto LAB_00e16971 end
                                                    me:MoveToPosition(xStack_58, 0.5, 1, false, false)
                                                    iVar4 = me:IsPerformingScriptTask()
                                                    cVar2 = iVar4
                                                    while cVar2 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar1 = not alive
                                                        if bVar1 then goto LAB_00e16971 end
                                                        iVar4 = me:IsPerformingScriptTask()
                                                        cVar2 = iVar4
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar1 = not alive
                                                    if bVar1 then goto LAB_00e16971 end
                                                    fVar11 = 2.5
                                                    x_stk_3c = resources:ScriptThing(xStack_104)
                                                    pCVar3 = x_stk_3c
                                                    bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_58, fVar11))
                                                until not (bVar1)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if not bVar1 then
                                                me:PlayAnimation("HOWL", false, false, false, true, true, false, false)
                                                iVar4 = me:IsPerformingScriptTask()
                                                cVar2 = iVar4
                                                while cVar2 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar1 = not alive
                                                    if bVar1 then goto LAB_00e16971 end
                                                    iVar4 = me:IsPerformingScriptTask()
                                                    cVar2 = iVar4
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar1 = not alive
                                                if not bVar1 then
                                                    me:PlayAnimation("ST_RUN_JUMP", false, false, false, true, true, false, false)
                                                    quest:Pause(1.0)
                                                    bVar10 = false
                                                    bVar1 = false
                                                    fVar11 = 0.0
                                                    pCVar5 = me:GetPos()
                                                    r5 = quest:CreateEffectAtPos("EFFECT_GRASS_CLUMPS", pCVar5, fVar11, bVar1)
                                                    quest:RemoveThing(me, false, true)
                                                    quest:SetStateInt("BalverineState", 5)
                                                    goto LAB_00e1647d_c1
                                                end
                                            end
                                        end
                                    end
                                end
                                goto LAB_00e16971
                            end
                            ::LAB_00e1647d_c1::
                            if quest:GetStateInt("BalverineState") == 6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if not bVar1 then
                                    r6 = quest:GetThingWithScriptName("WB_Guard1")
                                    resources:PrepareResource(xStack_104)
                                    bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                    while not bVar1 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if bVar1 then goto LAB_00e16964_c1 end
                                        bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar1 = not alive
                                    if not bVar1 then
                                        quest:EntitySetCombatType(me, "WHITE_BALVERINE_ATTACK_STYLE")
                                        resources:PrepareResource(xStack_104)
                                        quest:SetThingPersistent(me, true)
                                        pCVar3 = quest:GetHero()
                                        quest:GiveThingBestEnemyTarget(me, pCVar3)
                                        iVar8 = 0
                                        iVar4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE", 5, 1.0)
                                        __native_entity_state:SetStateInt("WBCounter", iVar4)
                                        quest:DisplayQuestInfo(true)
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if bVar1 then goto LAB_00e16964_c1 end
                                            bVar1 = me:MsgIsHitByHero()
                                            if bVar1 then
                                                goto LAB_00e16666_c1
                                            else
                                                bVar1 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                                if bVar1 then
                                                    bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                                    if not bVar1 then goto LAB_00e16666_c1 end
                                                end
                                                bVar1 = false
                                            end
                                            goto FLOW_past_lab_00e16666_c1
                                            ::LAB_00e16666_c1::
                                            bVar1 = true
                                            ::FLOW_past_lab_00e16666_c1::
                                            if bVar1 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar1 = not alive
                                                if bVar1 then goto LAB_00e16964_c1 end
                                                iVar8 = iVar8 + 1
                                                quest:UpdateQuestInfoCounter(__native_entity_state:GetStateInt("WBCounter"), iVar8, -1)
                                            end
                                        until not (iVar8 < 5)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar1 = not alive
                                        if not bVar1 then
                                            quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("WBCounter"))
                                            quest:DisplayQuestInfo(false)
                                            resources:PrepareResource(xStack_104)
                                            bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                            while not bVar1 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar1 = not alive
                                                if bVar1 then goto LAB_00e16964_c1 end
                                                bVar1 = resources:TryAcquire(xStack_104, me, 4)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar1 = not alive
                                            if not bVar1 then
                                                r7 = quest:GetThingWithScriptName("WB_RunPoint2")
                                                xStack_f0 = quest:RegisterTimer()
                                                timerId = xStack_f0
                                                quest:SetTimer(xStack_f0, 5)
                                                iVar4 = quest:GetTimer(timerId)
                                                while 0 < iVar4 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar1 = not alive
                                                    if bVar1 then goto LAB_00e1694f_c1 end
                                                    iVar4 = me:IsPerformingScriptTask()
                                                    if not iVar4 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar1 = not alive
                                                        if bVar1 then goto LAB_00e1694f_c1 end
                                                        me:MoveToThing(r7, 0, 1)
                                                    end
                                                    pCVar5 = me:GetPos()
                                                    bVar1 = quest:IsCameraPosOnScreen(pCVar5)
                                                    if bVar1 then
                                                        iVar4 = me:IsPerformingScriptTask()
                                                        if not iVar4 then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar1 = not alive
                                                            if not bVar1 then
                                                                quest:SetStateInt("BalverineState", 7)
                                                                goto LAB_00e1687b_c1
                                                            end
                                                            goto LAB_00e1694f_c1
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar1 = not alive
                                                        if bVar1 then goto LAB_00e1694f_c1 end
                                                        quest:SetStateInt("BalverineState", 7)
                                                        goto LAB_00e1687b_c1
                                                    end
                                                    goto FLOW_past_lab_00e1687b_c1
                                                    ::LAB_00e1687b_c1::
                                                    quest:FadeOutAndKillEntity(me, true, 3.0, true)
                                                    ::FLOW_past_lab_00e1687b_c1::
                                                    iVar4 = quest:GetTimer(xStack_f0)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar1 = not alive
                                                if not bVar1 then
                                                    quest:SetStateInt("BalverineState", 7)
                                                    quest:FadeOutAndKillEntity(me, true, 3.0, true)
                                                    quest:DeregisterTimer(timerId)
                                                    goto LAB_00e168ed_c1
                                                end
                                                ::LAB_00e1694f_c1::
                                                quest:DeregisterTimer(timerId)
                                            end
                                        end
                                    end
                                    ::LAB_00e16964_c1::
                                end
                                goto LAB_00e16971
                            end
                            ::LAB_00e168ed_c1::
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            goto FLOW_after_lab_00e15f22
                        end
                    end
                end
            end
        end
        ::LAB_00e16971::
        resources:ReleaseResource(xStack_104)
    end
end

function Init(quest, me)
    local r1 = quest:GetThingWithScriptName("KG_Gate")
    local r2 = quest:PlayCriteriaSoundOnThing(r1, "KNG_HORN")
    quest:OverrideMusic(0x17, false, false)
    r1 = nil
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

