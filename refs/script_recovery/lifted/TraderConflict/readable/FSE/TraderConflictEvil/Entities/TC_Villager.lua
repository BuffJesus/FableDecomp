-- Readable native conversion: TC_Villager. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TC_Villager.Main (retail 0x00df9180)
function Main(quest, me)
    local predicateResult, predicateResult6
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
        if not me:MsgIsHitBy("") then goto LAB_00df9372 end
        if me:MsgIsHitByHero() then goto LAB_00df9372 end
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00df9372 end
        end
        predicateResult = true
        goto FLOW_past_lab_00df9372
        ::LAB_00df9372::
        predicateResult = false
        ::FLOW_past_lab_00df9372::
        if predicateResult then
            quest:ModifyThingHealth(me, 100.0, false)
        else
            if me:MsgIsHitByHero() then
                goto LAB_00df9462
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00df9462 end
                end
                predicateResult6 = false
            end
            goto FLOW_past_lab_00df9462
            ::LAB_00df9462::
            predicateResult6 = true
            ::FLOW_past_lab_00df9462::
            if predicateResult6 then
                quest:SetStateBool("PlayerEngaged", true)
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:IsActiveThreadTerminating() do
        if quest:GetTimer(screamOutTimer) ~= 0 then
            quest:NewScriptFrame(me)
        elseif not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
            quest:NewScriptFrame(me)
        else
            local conversationID = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationID, hero)
            if quest:EntityGetSex(me) == 2 then
                if quest:IsActiveThreadTerminating() then return end
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_FEMALE_PANIC", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then return end
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_MALE_PANIC", me, hero, false)
            end
            local scratchValue = math.random(0, 32767)
            quest:SetTimer(screamOutTimer, scratchValue % 15 + 15)
            quest:NewScriptFrame(me)
        end
    end
end

-- TC_Villager.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_Villager.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_Villager.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

