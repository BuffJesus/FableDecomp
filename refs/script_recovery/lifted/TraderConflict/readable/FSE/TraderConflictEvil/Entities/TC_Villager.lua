-- Readable native conversion: TC_Villager. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_Villager.Main (retail 0x00df9180)
function Main(quest, me)
    local predicateResult, conversationID
    local screamOutTimer = quest:GetStateInt("ScreamOutTimer")
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    if not quest:IsRegionLoaded("BarrowFields") then
        return
    end
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    quest:EntitySetAsScared(me, true)
    quest:ClearThingHasInformation(me)
    quest:EntitySetCombatEnabled(me, false)
    quest:EntityUnsetThingAsAllyOfThing(me, hero)
    quest:EntityUnsetThingAsAllyOfThing(hero, me)
    while not quest:GetStateBool("PlayerEngaged") do
        if not quest:NewScriptFrame(me) then return end
        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
        if me:MsgIsHitBy("") then
            if me:MsgIsHitByHero() then
                predicateResult = false
                goto FLOW_after_lab_00df9372
            end
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(me) then
                    predicateResult = false
                    goto FLOW_after_lab_00df9372
                end
            end
            predicateResult = true
        else
            predicateResult = false
        end
        ::FLOW_after_lab_00df9372::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then return end
            quest:ModifyThingHealth(me, 100.0, false)
        elseif me:MsgIsHitByHero() or me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(me) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("PlayerEngaged", true)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:IsActiveThreadTerminating() do
        if quest:GetTimer(screamOutTimer) ~= 0 then quest:NewScriptFrame(me); goto continue_2 end
        if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
            if quest:IsActiveThreadTerminating() then return end
            conversationID = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationID, hero)
            if quest:EntityGetSex(me) == 2 then
                if quest:IsActiveThreadTerminating() then return end
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_FEMALE_PANIC", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then return end
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_MALE_PANIC", me, hero, false)
            end
            quest:SetTimer(screamOutTimer, math.random(0, 32767) % 15 + 15)
        end
        quest:NewScriptFrame(me)
        ::continue_2::
    end
end

-- TC_Villager.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_Villager.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_Villager.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

