-- Generated native draft: WB_Guard2. Review coverage report before use.
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
    local bVar1, cVar2, distance, iVar5, p0, p0_00, pCVar3, pCVar4, r1, r2, xStack_10, xStack_20, xStack_2c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    xStack_10 = resources:NewResource()
    resources:PrepareResource(xStack_10)
    bVar1 = resources:TryAcquire(xStack_10, me, 3)
    while not bVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00e174a0 end
        bVar1 = resources:TryAcquire(xStack_10, me, 3)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:EntitySetCutsceneBehaviour(me, 1)
        r1 = quest:GetThingWithScriptName("WB_Guard2Pos")
        if not (r1 ~= nil and not r1:IsNull()) then
            p0_00 = {x = 0, y = 0, z = 0}
        else
            p0_00 = r1:GetPos()
        end
        me:MoveToPosition(p0_00, 3.0, 1, false, true)
        bVar1 = quest:IsDistanceBetweenThingsUnder(me, r1, 5.0)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e17497 end
            bVar1 = quest:IsDistanceBetweenThingsUnder(me, r1, 5.0)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            cVar2 = quest:GetMasterGameState("WhiteBalverineFinished")
            repeat
                if cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        quest:RemoveThing(me, false, true)
                    end
                    -- LAB_00e1766f: (native jump target)
                    resources:ReleaseResource(xStack_10)
                    return
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    resources:ReleaseResource(xStack_10)
                    return
                end
                r2 = quest:GetThingWithScriptName("WB_WhiteBalverine")
                bVar1 = quest:IsDistanceBetweenThingsUnder(me, r2, 15.0)
                if bVar1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        pCVar3 = quest:GetHero()
                        pCVar4 = pCVar3:GetPos()
                        xStack_20 = {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z}
                        while true do
                            distance = 2.0
                            xStack_2c = resources:ScriptThing(xStack_10)
                            pCVar3 = xStack_2c
                            bVar1 = (pCVar3 ~= nil and pCVar3:IsDistanceFromPositionOver(xStack_20, distance))
                            xStack_2c = nil
                            if not bVar1 then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e17623 end
                            me:MoveToPosition(xStack_20, 0, 1, false, true)
                            iVar5 = me:IsPerformingScriptTask()
                            cVar2 = iVar5
                            while cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if bVar1 then
                                    r2 = nil
                                    r1 = nil
                                    resources:ReleaseResource(xStack_10)
                                    return
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar2 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e17623 end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if not bVar1 then goto LAB_00e17639 end
                    end
                    ::LAB_00e17623::
                    resources:ReleaseResource(xStack_10)
                    return
                end
                ::LAB_00e17639::
                cVar2 = quest:GetMasterGameState("WhiteBalverineFinished")
            until false
        end
        ::LAB_00e17497::
    end
    ::LAB_00e174a0::
    resources:ReleaseResource(xStack_10)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

