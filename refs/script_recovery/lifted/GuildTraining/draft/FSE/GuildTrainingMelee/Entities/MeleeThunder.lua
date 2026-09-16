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
    local cVar4, iVar2, pCVar1
    local alive = true
    iVar2 = quest:GetStateInt("TutorialState")
    while iVar2 ~= 4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        iVar2 = quest:GetStateInt("TutorialState")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)local_10);
        if bVar3 then
        end
        cVar4 = me:AcquireControl(4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d582ed end
            cVar4 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            iVar2 = quest:GetStateInt("TutorialState")
            while iVar2 == 4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d582ed end
                iVar2 = quest:GetStateInt("TutorialState")
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)local_10);
                if bVar3 then
                end
                iVar2 = quest:GetStateInt("TutorialState")
                while iVar2 ~= 6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d58297 end
                    iVar2 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)local_10);
                    if bVar3 then
                    end
                    cVar4 = me:AcquireControl(4)
                    while not cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d58297 end
                        cVar4 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        iVar2 = quest:GetStateInt("TutorialState")
                        while iVar2 == 6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d58297 end
                            iVar2 = quest:GetStateInt("TutorialState")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)local_10);
                            if bVar3 then
                            end
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                            until not (alive)
                        end
                    end
                end
                ::LAB_00d58297::
                return
            end
        end
    end
    ::LAB_00d582ed::
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

