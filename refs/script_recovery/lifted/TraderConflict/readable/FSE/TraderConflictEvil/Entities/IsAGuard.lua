-- Readable native conversion: IsAGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- IsAGuard.Main (retail 0x00df9710)
function Main(quest, me)
    local predicateResult
    if not quest:NewScriptFrame(me) then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:GetStateBool("PlayerEngaged") do
            if not quest:NewScriptFrame(me) then return end
            if not ((not me:MsgIsHitBy("")) or me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
                quest:ModifyThingHealth(me, 40.0, false)
            else
                if me:MsgIsHitByHero() then
                    goto LAB_00df992a
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df992a end
                    predicateResult = false
                end
                goto FLOW_past_lab_00df992a
                ::LAB_00df992a::
                predicateResult = true
                ::FLOW_past_lab_00df992a::
                if predicateResult then
                    quest:SetStateBool("PlayerEngaged", true)
                end
            end
            if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 15.0) then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("PlayerEngaged", true)
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:NewScriptFrame(me)
    end
end

-- IsAGuard.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- IsAGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- IsAGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

