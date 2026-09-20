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
    local bVar2, iVar1, pThing
    local alive = true
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetTargetable(me, false)
    iVar1 = quest:GetStateInt("PreMeleeMode")
    while iVar1 ~= 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar1 = quest:GetStateInt("PreMeleeMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:EntitySetTargetable(me, true)
        iVar1 = quest:GetStateInt("PreMeleeMode")
        while iVar1 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
                quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
            end
            iVar1 = quest:GetStateInt("PreMeleeMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            iVar1 = quest:GetStateInt("PreMeleeMode")
            while iVar1 == 2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = me:MsgIsHitByHeroWithWeapon("OBJECT_HERO_STICK")
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
                    quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
                else
                    bVar2 = me:MsgIsHitByHero()
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
                        goto LAB_00d521f0
                    end
                end
                ::LAB_00d521f0::
                iVar1 = quest:GetStateInt("PreMeleeMode")
            end
            alive = not quest:IsActiveThreadTerminating()
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

