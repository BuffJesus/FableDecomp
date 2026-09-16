-- Generated native draft: KillBird. Review coverage report before use.
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
    local cVar3, pCVar1
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_20);
        if bVar2 then
        end
        cVar3 = me:AcquireControl(4)
        while not cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d43263 end
            cVar3 = me:AcquireControl(4)
        end
        while CCreatureAction_TrollWhackGroundBase::Initialise(this), alive do
            alive = quest:NewScriptFrame(me)
        end
        ::LAB_00d43263::
    end
end

function Init(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsEnemyOfThing(me, hero)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local piVar1
    -- TODO(native): iStack_4 = this;
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        quest:SetStateBool("DisplayBirdKilledMessage", true)
        piVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
        -- TODO(native): *piVar1 = *piVar1 + 1;
    end
end

