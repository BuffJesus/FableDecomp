-- Generated native draft: WaspChaser. Review coverage report before use.
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
    local bVar1, cVar2, native_arg_sequence_1, p0, r1, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        xStack_20 = resources:NewResource()
        resources:PrepareResource(xStack_20)
        bVar1 = resources:TryAcquire(xStack_20, me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e10d61 end
            bVar1 = resources:TryAcquire(xStack_20, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            r1 = quest:GetThingWithScriptName("WaspChaseWoman")
            me:FollowThing(r1, 1.0, true)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            while not bVar1 do
                cVar2 = (r1 ~= nil and r1:IsAlive())
                if not cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    native_arg_sequence_1 = false
                    if not bVar1 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        bVar1 = false
                        if bVar1 ~= 0 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        resources:PrepareResource(xStack_20)
                    end
                    break
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
            end
        end
        ::LAB_00e10d61::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

