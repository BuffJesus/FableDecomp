-- Readable native conversion: TC_BanditFighter. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local hitWarning

-- TC_BanditFighter.Main (retail 0x00df8970)
function Main(quest, me)
    local predicateResult, predicateResult4, predicateResult6
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:GiveThingBestEnemyTarget(me, quest:GetNearestWithScriptName(me, "IsAGuard"))
    while not quest:GetStateBool("PlayerEngaged") do
        if not quest:NewScriptFrame(me) then return end
        if not me:MsgIsHitBy("") then goto LAB_00df8b83 end
        if me:MsgIsHitByHero() then goto LAB_00df8b83 end
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df8b83 end
        end
        predicateResult = true
        goto FLOW_past_lab_00df8b83
        ::LAB_00df8b83::
        predicateResult = false
        ::FLOW_past_lab_00df8b83::
        if predicateResult then
            quest:ModifyThingHealth(me, 100.0, false)
        else
            if me:MsgIsHitByHero() then
                goto LAB_00df8c73
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df8c73 end
                end
                predicateResult4 = false
            end
            goto FLOW_past_lab_00df8c73
            ::LAB_00df8c73::
            predicateResult4 = true
            ::FLOW_past_lab_00df8c73::
            if predicateResult4 then
                quest:SetStateBool("PlayerEngaged", true)
            end
        end
        if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("PlayerEngaged", true)
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame(me) then return end
        if me:MsgIsHitByHero() then
            goto LAB_00df8dcb
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df8dcb end
            end
            predicateResult6 = false
        end
        goto FLOW_past_lab_00df8dcb
        ::LAB_00df8dcb::
        predicateResult6 = true
        ::FLOW_past_lab_00df8dcb::
        if predicateResult6 then
            if not hitWarning then
                local conversationID = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationID, hero)
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_BANDIT_ON_HIT_10", me, hero, false)
                hitWarning = true
            elseif quest:IsDistanceBetweenThingsUnder(me, quest:GetNearestWithScriptName(me, "TC_BanditFighter"), 15.0) then
                quest:SetStateBool("HeroAttackedBandit", true)
            end
        end
        if quest:GetStateBool("HeroAttackedBandit") then
            if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                if quest:IsActiveThreadTerminating() then return end
                if math.random(0, 32767) % 5 == 0 then
                    if quest:IsActiveThreadTerminating() then return end
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_SEEKING_REVENGE_10", me, hero, false)
                end
                quest:GiveThingBestEnemyTarget(me, hero)
                quest:EntityUnsetThingAsAllyOfThing(me, hero)
                quest:EntityUnsetThingAsAllyOfThing(hero, me)
                while not quest:GetStateBool("MissionSucceeded") do
                    if not quest:NewScriptFrame(me) then return end
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, false, true)
end

-- TC_BanditFighter.Init (retail 0x00df8940)
function Init(quest, me)
    hitWarning = false
end

-- TC_BanditFighter.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_BanditFighter.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

