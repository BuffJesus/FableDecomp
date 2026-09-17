-- Readable native conversion: IsAGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- IsAGuard.Main (retail 0x00df9710)
function Main(quest, me)
    local predicateResult6
    if not quest:NewScriptFrame(me) then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:GetStateBool("PlayerEngaged") do
            if not quest:NewScriptFrame(me) then return end
            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
            if me:MsgIsHitBy("") then
                if me:MsgIsHitByHero() then
                    predicateResult6 = false
                    goto FLOW_after_lab_00df983a
                end
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        predicateResult6 = false
                        goto FLOW_after_lab_00df983a
                    end
                end
                predicateResult6 = true
            else
                predicateResult6 = false
            end
            ::FLOW_after_lab_00df983a::
            if predicateResult6 then
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

