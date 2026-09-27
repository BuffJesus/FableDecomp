-- Readable native conversion: FireHeart. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("SummoningTheShip.native_quest_helpers")

-- FireHeart.Main (retail 0x00df2f90)
function Main(quest, me)
    local predicateResult
    quest:EntitySetAlpha(me, 0.0, true)
    while not quest:GetStateBool("LighthouseStarted") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapRemoveMarker(me)
    while not quest:GetStateBool("SummonerAttacksStarted") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetAsDamageable(me, true)
    quest:DisplayQuestInfo(true)
    quest:EntitySetAlpha(me, 1.0, true)
    local timerId2 = quest:RegisterTimer()
    -- TODO(native): r2 = quest:AddQuestInfoBarHealth(r1, &0xff00ff00, "HUD_ICON_FIRE_HEART", 1.0)
--[[unresolved native value]]
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 3)
    while 1.0 < quest:GetHealth(me) do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
        if quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
            if 750.0 < quest:GetHealth(me) then
                if 1500.0 < quest:GetHealth(me) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
                    quest:Play2DSound("SND_MM_FIREHEART_BEAT_SLOW")
                    quest:SetTimer(timerId, 3)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
                    quest:Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM")
                    quest:SetTimer(timerId, 2)
                end
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
                quest:Play2DSound("SND_MM_FIREHEART_BEAT_FAST")
                quest:SetTimer(timerId, 1)
            end
        end
        if me:MsgIsHitBy("SummonerAttacker") then
            goto LAB_00df3310
        else
            if me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker") then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df3310 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00df3310
        ::LAB_00df3310::
        predicateResult = true
        ::FLOW_past_lab_00df3310::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
            if not ((quest:GetTimer(timerId2) ~= 0) or (not helpers.MakeBriarRoseComment(quest, me, "FIREHEART"))) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId2); return end
                quest:SetTimer(timerId2, 20)
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:SetStateBool("MissionFailed", true)
    end
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId2)
end

-- FireHeart.Init (retail 0x00df2f40)
function Init(quest, me)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
end

-- FireHeart.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FireHeart.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

