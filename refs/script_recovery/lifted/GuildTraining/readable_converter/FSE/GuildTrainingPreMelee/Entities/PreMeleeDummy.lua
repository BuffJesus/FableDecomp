-- Readable native conversion: PreMeleeDummy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- PreMeleeDummy.Main (retail 0x00d51fc0)
function Main(quest, me)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetTargetable(me, false)
    while quest:GetStateInt("PreMeleeMode") ~= 1 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetTargetable(me, true)
    while quest:GetStateInt("PreMeleeMode") == 1 do
        if not quest:NewScriptFrame(me) then return end
        if me:MsgIsHitByHero() then
            quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
            quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    while quest:GetStateInt("PreMeleeMode") == 2 do
        if not quest:NewScriptFrame(me) then return end
        if me:MsgIsHitByHeroWithWeapon("OBJECT_HERO_STICK") then
            quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
            quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        elseif me:MsgIsHitByHero() then
            quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
        end
    end
end

-- PreMeleeDummy.Init (retail 0x00d51fb0)
function Init(quest, me)
end

-- PreMeleeDummy.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- PreMeleeDummy.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

