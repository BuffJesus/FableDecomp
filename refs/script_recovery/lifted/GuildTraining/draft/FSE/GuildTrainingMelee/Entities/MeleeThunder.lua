-- Generated native draft: MeleeThunder. Review coverage report before use.
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
    local bVar2, iVar1, pThing, xStack_10
    local alive = true
    xStack_10 = resources:NewResource()
    iVar1 = quest:GetStateInt("TutorialState")
    while iVar1 ~= 4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_10)
            return
        end
        iVar1 = quest:GetStateInt("TutorialState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        resources:PrepareResource(xStack_10)
        bVar2 = resources:TryAcquire(xStack_10, me, 4)
        while not bVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d582ed end
            bVar2 = resources:TryAcquire(xStack_10, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            iVar1 = quest:GetStateInt("TutorialState")
            while iVar1 == 4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d582ed end
                iVar1 = quest:GetStateInt("TutorialState")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                resources:PrepareResource(xStack_10)
                iVar1 = quest:GetStateInt("TutorialState")
                while iVar1 ~= 6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d58297 end
                    iVar1 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    resources:PrepareResource(xStack_10)
                    bVar2 = resources:TryAcquire(xStack_10, me, 4)
                    while not bVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d58297 end
                        bVar2 = resources:TryAcquire(xStack_10, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        iVar1 = quest:GetStateInt("TutorialState")
                        while iVar1 == 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00d58297 end
                            iVar1 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            resources:PrepareResource(xStack_10)
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                            until not (not bVar2)
                        end
                    end
                end
                ::LAB_00d58297::
                resources:ReleaseResource(xStack_10)
                return
            end
        end
    end
    ::LAB_00d582ed::
    resources:ReleaseResource(xStack_10)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

