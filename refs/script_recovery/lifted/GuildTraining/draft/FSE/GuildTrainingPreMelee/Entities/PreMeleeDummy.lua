-- Generated native draft: PreMeleeDummy. Review coverage report before use.
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
    local cVar3, pCVar7
    local alive = true
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetTargetable(me, false)
    local iVar2 = quest:GetStateInt("PreMeleeMode")
    while iVar2 ~= 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        iVar2 = quest:GetStateInt("PreMeleeMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:EntitySetTargetable(me, true)
        iVar2 = quest:GetStateInt("PreMeleeMode")
        while iVar2 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            cVar3 = me:MsgIsHitByHero()
            if cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
                quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
            end
            iVar2 = quest:GetStateInt("PreMeleeMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            iVar2 = quest:GetStateInt("PreMeleeMode")
            while iVar2 == 2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                cVar3 = me:MsgIsHitByHeroWithWeapon("OBJECT_HERO_STICK")
                if not cVar3 then
                    cVar3 = me:MsgIsHitByHero()
                    if cVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                        quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
                        -- TODO(native): goto LAB_00d521f0
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        return
                    end
                    quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
                    quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
                    -- LAB_00d521f0: (native jump target)
                end
                iVar2 = quest:GetStateInt("PreMeleeMode")
            end
            alive = not quest:IsActiveThreadTerminating()
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

