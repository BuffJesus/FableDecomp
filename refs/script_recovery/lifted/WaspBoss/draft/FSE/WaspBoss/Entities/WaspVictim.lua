-- Generated native draft: WaspVictim. Review coverage report before use.
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
    local __native_condition_1, bVar2, cVar3, dist, fret_0, iVar4, iVar6, p0, pCVar5, r1, r2, timerId, xStack_20, xStack_40
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    cVar3 = quest:GetStateBool("QuestStartScreened")
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar3 = quest:GetStateBool("QuestStartScreened")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_20 = resources:NewResource()
    resources:PrepareResource(xStack_20)
    bVar2 = resources:TryAcquire(xStack_20, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_20)
            return
        end
        bVar2 = resources:TryAcquire(xStack_20, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00e11772: (native jump target)
        resources:ReleaseResource(xStack_20)
        return
    end
    bVar2 = false
    fret_0 = quest:GetHealth(me)
    quest:ModifyThingHealth(me, (6.0 - fret_0), bVar2)
    if quest:GetStateBool("QueenHornetAttacks") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11aba end
        quest:RemoveThing(me, false, true)
    end
    r1 = quest:GetThingWithScriptName("WaspAttacker")
    xStack_40 = quest:RegisterTimer()
    timerId = xStack_40
    while true do
        __native_condition_1 = (r1 ~= nil and not r1:IsNull())
        if __native_condition_1 then
            cVar3 = (r1 ~= nil and r1:IsAlive())
            __native_condition_1 = cVar3
        end
        if not __native_condition_1 then break end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e11aa8 end
        iVar4 = quest:GetTimer(timerId)
        if iVar4 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e11aa8 end
            dist = quest:ReadGlobalGameData(0xe48)
            pCVar5 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, dist)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e11aa8 end
                quest:SetTimer(timerId, quest:ReadGlobalGameData(0xe54))
                iVar6 = quest:AddNewConversation(me, false, false)
                pCVar5 = quest:GetHero()
                quest:AddPersonToConversation(iVar6, pCVar5)
                pCVar5 = quest:GetHero()
                quest:AddLineToConversation(iVar6, "TEXT_QST_072_VILLAGER_MALE_SCREAMS_10", me, pCVar5, false)
                timerId = xStack_40
            end
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        iVar6 = quest:AddNewConversation(me, false, false)
        pCVar5 = quest:GetHero()
        quest:AddPersonToConversation(iVar6, pCVar5)
        pCVar5 = quest:GetHero()
        quest:AddLineToConversation(iVar6, "TEXT_QST_072_VILLAGER_MALE_ON_SAVED_10", me, pCVar5, false)
        quest:EntitySetAsScared(me, false)
        r2 = quest:GetThingWithScriptName("VillagerEscapePos")
        if quest:GetStateInt("SavedVillagerCount") < 2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe30))
                quest:SetStateInt("SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") + 1)
                goto LAB_00e11a0a
            end
        else
            goto LAB_00e11a0a
        end
        goto FLOW_past_lab_00e11a0a
        ::LAB_00e11a0a::
        bVar2 = quest:IsDistanceBetweenThingsOver(me, r2, 2.0)
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e11a9f end
                iVar4 = me:IsPerformingScriptTask()
                if not iVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e11a9f end
                    me:MoveToThing(r2, 1.0, 1)
                end
                bVar2 = quest:IsDistanceBetweenThingsOver(me, r2, 2.0)
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(0xe4c), true)
        end
        ::FLOW_past_lab_00e11a0a::
        ::LAB_00e11a9f::
    end
    ::LAB_00e11aa8::
    quest:DeregisterTimer(xStack_40)
    ::LAB_00e11aba::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
    quest:EntitySetAsScared(me, true)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

