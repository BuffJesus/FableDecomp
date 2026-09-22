-- Generated native draft: WaspHelper. Review coverage report before use.
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
    local bVar4, cVar5, fVar1, fVar10, fVar17, fVar18, fret_0, fret_00, iVar12, iVar13, iVar14, iVar15, p0, pCVar16, pCVar6, pCVar8, pScriptObject, pcVar11, puVar7, r1, r2, r3, uVar2, uVar3, xStack_10, xStack_20, xStack_40, xStack_5c, x_stk_30
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    xStack_20 = resources:NewResource()
    r1 = quest:GetThingWithScriptName("WaspGuardMoveToPos")
    resources:PrepareResource(xStack_20)
    bVar4 = resources:TryAcquire(xStack_20, me, 3)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        bVar4 = resources:TryAcquire(xStack_20, me, 3)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00e109ab end
    if not __native_entity_state:GetStateBool("PleadedToHero") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        iVar15 = 1
        iVar13 = 1.0
        pCVar6 = quest:GetHero()
        me:FollowThing(pCVar6, iVar13, (iVar15 ~= 0))
        fVar18 = quest:ReadGlobalGameData(0xe50)
        pCVar6 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar18)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            fVar18 = quest:ReadGlobalGameData(0xe50)
            pCVar6 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar18)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        if quest:GetStateBool("MissionSucceeded") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            quest:RemoveThing(me, false, true)
        end
        iVar13 = quest:GetTimer(quest:GetStateInt("ReachedWaspHelper"))
        if iVar13 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            resources:PrepareResource(xStack_20)
            bVar4 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109ab end
                bVar4 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            me:ClearCommands()
            xStack_10 = resources:StartMovie("")
            pCVar16 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            x_stk_30 = resources:ScriptThing(xStack_20)
            pCVar6 = x_stk_30
            fret_0 = quest:GetHealth(pCVar6)
            fVar1 = 0.0
            if fVar1 < fret_0 then
                iVar14 = 0
                iVar12 = 1
                iVar15 = 0
                iVar13 = 0
                pcVar11 = "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10"
                pCVar6 = quest:GetHero()
                r2 = me:Speak(pCVar6, pcVar11, iVar13, (iVar15 ~= 0), (iVar12 ~= 0), (iVar14 ~= 0))
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e0fd75 end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                end
                goto LAB_00e0fbf4
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            resources:PrepareResource(xStack_20)
            bVar4 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109ab end
                bVar4 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            me:ClearCommands()
            xStack_10 = resources:StartMovie("")
            pCVar16 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            x_stk_30 = resources:ScriptThing(xStack_20)
            pCVar6 = x_stk_30
            fret_00 = quest:GetHealth(pCVar6)
            fVar1 = 0.0
            if fVar1 < fret_00 then
                iVar14 = 0
                iVar12 = 1
                iVar15 = 0
                iVar13 = 0
                pcVar11 = "TEXT_QST_072_HELPER_FOLLOW_ME_10"
                pCVar6 = quest:GetHero()
                r3 = me:Speak(pCVar6, pcVar11, iVar13, (iVar15 ~= 0), (iVar12 ~= 0), (iVar14 ~= 0))
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e0fd75 end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                end
                goto LAB_00e0fbf4
            end
        end
        goto FLOW_past_lab_00e0fbf4
        ::LAB_00e0fbf4::
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            goto LAB_00e109ab
        end
        ::FLOW_past_lab_00e0fbf4::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        __native_entity_state:SetStateBool("PleadedToHero", true)
    end
    resources:PrepareResource(xStack_20)
    pScriptObject = xStack_20
    bVar4 = resources:TryAcquire(pScriptObject, me, 3)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        pScriptObject = xStack_20
        bVar4 = resources:TryAcquire(pScriptObject, me, 3)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00e109ab end
    if not __native_entity_state:GetStateBool("LeadToRegion") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        xStack_5c = quest:RegisterTimer()
        me:ClearCommands()
        if not (r1 ~= nil and not r1:IsNull()) then
            puVar7 = {x = 0, y = 0, z = 0}
        else
            puVar7 = r1:GetPos()
        end
        pCVar16 = 0x1
        me:MoveToPosition(puVar7, 1.0, 1, false, true)
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
        uVar2 = 0
        uVar3 = 0
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109a6 end
            xStack_40 = (quest:GetDistanceBetweenThings(me, r1) ^ 2)
            pCVar8 = quest:GetHero()
            fVar10 = (quest:GetDistanceBetweenThings(pCVar8, r1) ^ 2)
            fVar18 = xStack_40
            fVar17 = 9.0
            pCVar8 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar17)
            if (bVar4) or (fVar10 <= fVar18) then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                fVar17 = 6.5
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar17)
                if (bVar4) or (fVar10 <= fVar18) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e109a6 end
                    if not uVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e109a6 end
                        me:ClearCommands()
                        if not (r1 ~= nil and not r1:IsNull()) then
                            puVar7 = {x = 0, y = 0, z = 0}
                        else
                            puVar7 = r1:GetPos()
                        end
                        pCVar16 = 0x1
                        me:MoveToPosition(puVar7, 1.0, 1, false, true)
                        uVar2 = 1
                    end
                else
                    fVar18 = 8.0
                    pCVar8 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar18)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e109a6 end
                        if uVar2 ~= 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e109a6 end
                            me:ClearCommands()
                            if not (r1 ~= nil and not r1:IsNull()) then
                                puVar7 = {x = 0, y = 0, z = 0}
                            else
                                puVar7 = r1:GetPos()
                            end
                            pCVar16 = 0x0
                            me:MoveToPosition(puVar7, 1.0, 0, false, true)
                            uVar2 = 0
                        end
                    end
                end
                uVar3 = 0
                iVar13 = quest:GetTimer(xStack_5c)
                if iVar13 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        iVar15 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar15, pCVar6)
                        pCVar16 = quest:GetHero()
                        pCVar6 = "TEXT_QST_072_HELPER_GO_THIS_WAY"
                        quest:AddLineToConversation(iVar15, pCVar6, me, pCVar16, false)
                        iVar13 = 8
                        goto LAB_00e10323
                    end
                    goto LAB_00e109a6
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                me:ClearCommands()
                me:ClearAllActions()
                if not uVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e109a6 end
                    bVar4 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
                    iVar15 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar15, pCVar6)
                    pCVar8 = quest:GetHero()
                    pCVar16 = 0x0
                    pCVar6 = "TEXT_QST_072_HELPER_OVER_HERE"
                    quest:AddLineToConversation(iVar15, pCVar6, me, pCVar8, false)
                    quest:Pause(1.0)
                    uVar3 = 1
                end
                iVar13 = quest:GetTimer(xStack_5c)
                if iVar13 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e109a6 end
                    bVar4 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
                    iVar15 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar15, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_OVER_HERE", me, pCVar6, false)
                    me:PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, true, false, false)
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e109a6 end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e109a6 end
                    quest:Pause(0.8)
                    pCVar6 = 0x1
                    me:PlayAnimation("ST_OPINION_APPROVAL_WAVING_AT_DISTANCE", false, false, false, true, true, false, false)
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e109a6 end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e109a6 end
                    iVar13 = 5
                    goto LAB_00e10323
                end
            end
            goto FLOW_past_lab_00e10323
            ::LAB_00e10323::
            quest:SetTimer(xStack_5c, iVar13)
            ::FLOW_past_lab_00e10323::
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            __native_entity_state:SetStateBool("LeadToRegion", true)
            quest:DeregisterTimer(xStack_5c)
            goto LAB_00e1036a
        end
    else
        goto LAB_00e1036a
    end
    goto FLOW_past_lab_00e1036a
    ::LAB_00e1036a::
    me:ClearCommands()
    bVar4 = false
    quest:SetIsPushableByHero(me, bVar4)
    if not __native_entity_state:GetStateBool("WavedOver") then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        iVar15 = quest:AddNewConversation(me, false, false)
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar15, pCVar6)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_THIS_WAY_10", me, pCVar6, false)
        bVar4 = false
        pCVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
        iVar13 = me:IsPerformingScriptTask()
        cVar5 = iVar13
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
        iVar13 = me:IsPerformingScriptTask()
        cVar5 = iVar13
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        quest:EntitySetFacingAngleTowardsThing(me, r1, false)
        iVar13 = me:IsPerformingScriptTask()
        cVar5 = iVar13
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        me:PlayAnimation("ST_CALL_OVER", false, false, false, true, true, false, false)
        iVar13 = me:IsPerformingScriptTask()
        cVar5 = iVar13
        while cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e109ab end
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e109ab end
        __native_entity_state:SetStateBool("WavedOver", true)
    end
    xStack_5c = quest:RegisterTimer()
    quest:SetTimer(xStack_5c, 10)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    while not bVar4 do
        bVar4 = me:IsTalkedToByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            iVar15 = quest:AddNewConversation(me, false, false)
            pCVar6 = quest:GetHero()
            quest:AddPersonToConversation(iVar15, pCVar6)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then break end
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, pCVar6, false)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then break end
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, pCVar6, false)
            end
        end
        iVar13 = quest:GetTimer(xStack_5c)
        if iVar13 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            bVar4 = false
            pCVar6 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
            quest:SetTimer(xStack_5c, 10)
            iVar15 = quest:AddNewConversation(me, false, false)
            pCVar6 = quest:GetHero()
            quest:AddPersonToConversation(iVar15, pCVar6)
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then break end
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_NOT_MOVED_10", me, pCVar6, false)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then break end
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar15, "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10", me, pCVar6, false)
            end
            bVar4 = false
            pCVar6 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            quest:EntitySetFacingAngleTowardsThing(me, r1, false)
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            me:PlayAnimation("ST_CALL_OVER", false, false, false, true, true, false, false)
            iVar13 = me:IsPerformingScriptTask()
            cVar5 = iVar13
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e109a6 end
                iVar13 = me:IsPerformingScriptTask()
                cVar5 = iVar13
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    end
    ::FLOW_past_lab_00e1036a::
    ::LAB_00e109a6::
    quest:DeregisterTimer(xStack_5c)
    ::LAB_00e109ab::
    resources:ReleaseResource(xStack_20)
    do return end
    ::LAB_00e0fd75::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    goto LAB_00e109ab
end

function Init(quest, me)
    __native_entity_state:SetStateBool("PleadedToHero", false)
    __native_entity_state:SetStateBool("LeadToRegion", false)
    __native_entity_state:SetStateBool("WavedOver", false)
end

function OnPersist(quest, me, context)
    local pleadedToHero = quest:GetStateBool("PleadedToHero") or false
    pleadedToHero = quest:PersistTransferBool(context, "PleadedToHero", pleadedToHero)
    quest:SetStateBool("PleadedToHero", pleadedToHero)
end

function OnPredicateFail(quest, me)
end

