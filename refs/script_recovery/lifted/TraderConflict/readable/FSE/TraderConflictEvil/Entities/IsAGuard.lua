-- Readable native conversion: IsAGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- IsAGuard.Main (retail 0x00df9710)
function Main(quest, me)
    local predicateResult
    if not quest:NewScriptFrame(me) then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:GetStateBool("PlayerEngaged") do
            if not quest:NewScriptFrame(me) then return end
            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
            if me:MsgIsHitBy("") then
                if me:MsgIsHitByHero() then
                    predicateResult = false
                    goto FLOW_after_lab_00df983a
                end
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        predicateResult = false
                        goto FLOW_after_lab_00df983a
                    end
                end
                predicateResult = true
            else
                predicateResult = false
            end
            ::FLOW_after_lab_00df983a::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then return end
                quest:ModifyThingHealth(me, 40.0, false)
            elseif me:MsgIsHitByHero() or me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(me) then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("PlayerEngaged", true)
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
function OnPersist(quest, context)
end

-- IsAGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

