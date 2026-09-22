-- Generated native draft: FleeingWoman. Review coverage report before use.
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
    local bVar1, conversationID, fVar4, iVar3, p0, p0_00, pCVar2, r1, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    xStack_20 = resources:NewResource()
    resources:PrepareResource(xStack_20)
    bVar1 = resources:TryAcquire(xStack_20, me, 4)
    while not bVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00e0f616 end
        bVar1 = resources:TryAcquire(xStack_20, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        fVar4 = quest:ReadGlobalGameData(0xe44)
        pCVar2 = quest:GetHero()
        bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar2, fVar4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e0f616 end
            fVar4 = quest:ReadGlobalGameData(0xe44)
            pCVar2 = quest:GetHero()
            bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar2, fVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            r1 = quest:GetThingWithScriptName("FleeingWomanEscapePos")
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            repeat
                if bVar1 then
                    -- LAB_00e0f799: (native jump target)
                    resources:ReleaseResource(xStack_20)
                    return
                end
                if not __native_entity_state:GetStateBool("ScreamedAtHero") then
                    fVar4 = quest:ReadGlobalGameData(0xe48)
                    pCVar2 = quest:GetHero()
                    bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar2, fVar4)
                    if bVar1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then
                            resources:ReleaseResource(xStack_20)
                            return
                        end
                        __native_entity_state:SetStateBool("ScreamedAtHero", true)
                        conversationID = quest:AddNewConversation(me, false, false)
                        pCVar2 = quest:GetHero()
                        quest:AddPersonToConversation(conversationID, pCVar2)
                        pCVar2 = quest:GetHero()
                        quest:AddLineToConversation(conversationID, "TEXT_QST_072_WOMAN_FLEES_10", me, pCVar2, false)
                    end
                end
                iVar3 = me:IsPerformingScriptTask()
                if not iVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        resources:ReleaseResource(xStack_20)
                        return
                    end
                    if not (r1 ~= nil and not r1:IsNull()) then
                        p0_00 = {x = 0, y = 0, z = 0}
                    else
                        p0_00 = r1:GetPos()
                    end
                    me:MoveToPosition(p0_00, 1.0, 1, false, true)
                end
                bVar1 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                if bVar1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(0xe4c), true)
                    end
                    resources:ReleaseResource(xStack_20)
                    return
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
            until false
        end
    end
    ::LAB_00e0f616::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("ScreamedAtHero", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

