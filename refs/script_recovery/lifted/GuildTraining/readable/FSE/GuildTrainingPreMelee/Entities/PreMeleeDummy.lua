-- Reviewed native slice for Q_GuildTrainingPreMelee.PreMeleeDummy.
-- PreMeleeMode 1 accepts ordinary hits; mode 2 requires the hero stick.

function Init(quest, me)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetTargetable(me, false)
end

function Main(quest, me)
    local function waitForMode(mode)
        while quest:GetStateInt("PreMeleeMode") ~= mode do
            if not quest:NewScriptFrame(me) then return false end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end

    if not waitForMode(1) then return end
    quest:EntitySetTargetable(me, true)
    while quest:GetStateInt("PreMeleeMode") == 1 do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
        if me:MsgIsHitByHero() then
            quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
            quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
        end
    end

    while quest:GetStateInt("PreMeleeMode") == 2 do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
        if me:MsgIsHitByHeroWithWeapon("OBJECT_HERO_STICK") then
            quest:SetStateInt("DummyHits", quest:GetStateInt("DummyHits") + 1)
            quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        elseif me:MsgIsHitByHero() then
            quest:EntityPlayObjectAnimation(me, "WOBBLE", false)
        end
    end
end
