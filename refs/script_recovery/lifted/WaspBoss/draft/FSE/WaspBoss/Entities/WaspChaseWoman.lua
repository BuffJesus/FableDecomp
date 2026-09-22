-- Generated native draft: WaspChaseWoman. Review coverage report before use.
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
    local bVar2, cVar3, iVar10, iVar6, iVar7, iVar8, iVar9, native_arg_sequence_1, p0, pCVar4, pCVar5, r1, r2, r3, xStack_10, xStack_58
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_10 = resources:NewResource()
    resources:PrepareResource(xStack_10)
    bVar2 = resources:TryAcquire(xStack_10, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11369 end
        bVar2 = resources:TryAcquire(xStack_10, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e11369 end
    if quest:GetStateBool("QueenHornetAttacks") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11369 end
        quest:RemoveThing(me, false, true)
    end
    cVar3 = quest:GetStateBool("StartChase")
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11369 end
        cVar3 = quest:GetStateBool("StartChase")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e11369 end
    r1 = quest:GetThingWithScriptName("WaspChaser")
    iVar7 = 2
    pCVar4 = tostring(3)
    pCVar4 = ("ChasedWomanNav" .. pCVar4)
    r2 = quest:GetThingWithScriptName(pCVar4)
    iVar10 = 1
    iVar9 = 0
    iVar8 = 1
    pCVar4 = tostring(2)
    pCVar4 = ("ChasedWomanNav" .. pCVar4)
    pCVar5 = quest:GetThingWithScriptName(pCVar4)
    me:FollowPreCalculatedRoute(pCVar5, iVar8, (iVar9 ~= 0), (iVar10 ~= 0))
    xStack_58 = quest:RegisterTimer()
    quest:SetTimer(xStack_58, 0xb)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        iVar6 = me:IsPerformingScriptTask()
        native_arg_sequence_1 = false
        if not iVar6 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if not native_arg_sequence_1 then
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, r2, 2.0)
            if bVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e1134e end
            iVar7 = (iVar7 + 1) % 6
            pCVar4 = tostring((iVar7 + 1) % 6)
            pCVar4 = ("ChasedWomanNav" .. pCVar4)
            pCVar5 = quest:GetThingWithScriptName(pCVar4)
            r2 = pCVar5
            me:ClearCommands()
            iVar10 = 1
            iVar9 = 0
            iVar8 = 1
            pCVar4 = tostring(iVar7)
            pCVar4 = ("ChasedWomanNav" .. pCVar4)
            pCVar5 = quest:GetThingWithScriptName(pCVar4)
            me:FollowPreCalculatedRoute(pCVar5, iVar8, (iVar9 ~= 0), (iVar10 ~= 0))
        end
        cVar3 = (r1 ~= nil and r1:IsAlive())
        if not cVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e1134e end
            quest:EntitySetAsScared(me, false)
            iVar7 = quest:AddNewConversation(me, false, false)
            pCVar5 = quest:GetHero()
            quest:AddPersonToConversation(iVar7, pCVar5)
            pCVar5 = quest:GetHero()
            quest:AddLineToConversation(iVar7, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10", me, pCVar5, false)
            r3 = quest:GetThingWithScriptName("VillagerEscapePos")
            if quest:GetStateInt("SavedVillagerCount") < 2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e11345 end
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe30))
                quest:SetStateInt("SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") + 1)
            end
            bVar2 = quest:IsDistanceBetweenThingsOver(me, r3, 2.0)
            goto LAB_00e112c4
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    quest:DeregisterTimer(xStack_58)
    goto LAB_00e11357
    ::LAB_00e112c4::
    if not bVar2 then goto LAB_00e11322 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e11345 end
    iVar6 = me:IsPerformingScriptTask()
    if not iVar6 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11345 end
        me:MoveToThing(r3, 1.0, 1)
    end
    bVar2 = quest:IsDistanceBetweenThingsOver(me, r3, 2.0)
    goto LAB_00e112c4
    ::LAB_00e11322::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(0xe4c), true)
    end
    ::LAB_00e11345::
    ::LAB_00e1134e::
    quest:DeregisterTimer(xStack_58)
    ::LAB_00e11357::
    ::LAB_00e11369::
    resources:ReleaseResource(xStack_10)
end

function Init(quest, me)
    quest:EntitySetAsScared(me, true)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

