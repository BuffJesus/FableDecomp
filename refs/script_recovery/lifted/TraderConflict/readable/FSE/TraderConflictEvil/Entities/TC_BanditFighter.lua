-- Readable native conversion: TC_BanditFighter. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local hitWarning

-- TC_BanditFighter.Main (retail 0x00df8970)
function Main(quest, me)
    local predicateResult, predicateResult6
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    local hero10 = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(hero10, me)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local isAGuard = quest:GetNearestWithScriptName(me, "IsAGuard")
    quest:GiveThingBestEnemyTarget(me, isAGuard)
    while not quest:GetStateBool("PlayerEngaged") do
        if not quest:NewScriptFrame(me) then return end
        if not ((not me:MsgIsHitBy("")) or me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then return end
            quest:ModifyThingHealth(me, 100.0, false)
        else
            if me:MsgIsHitByHero() then
                goto LAB_00df8c73
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df8c73 end
                predicateResult = false
            end
            goto FLOW_past_lab_00df8c73
            ::LAB_00df8c73::
            predicateResult = true
            ::FLOW_past_lab_00df8c73::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("PlayerEngaged", true)
            end
        end
        local hero2 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0) then
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
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00df8dcb end
            predicateResult6 = false
        end
        goto FLOW_past_lab_00df8dcb
        ::LAB_00df8dcb::
        predicateResult6 = true
        ::FLOW_past_lab_00df8dcb::
        if predicateResult6 then
            if quest:IsActiveThreadTerminating() then return end
            if not hitWarning then
                if quest:IsActiveThreadTerminating() then return end
                local conversationID = quest:AddNewConversation(me, false, false)
                local hero3 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, hero3)
                local hero4 = quest:GetHero()
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_BANDIT_ON_HIT_10", me, hero4, false)
                hitWarning = true
            else
                if quest:IsActiveThreadTerminating() then return end
                local banditFighter = quest:GetNearestWithScriptName(me, "TC_BanditFighter")
                if quest:IsDistanceBetweenThingsUnder(me, banditFighter, 15.0) then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateBool("HeroAttackedBandit", true)
                end
            end
        end
        if quest:GetStateBool("HeroAttackedBandit") then
            local hero5 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero5, 15.0) then
                if quest:IsActiveThreadTerminating() then return end
                if quest:RetailRandModulo(32768) % 5 == 0 then
                    if quest:IsActiveThreadTerminating() then return end
                    local conversationId = quest:AddNewConversation(me, false, false)
                    local hero6 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId, hero6)
                    local hero7 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_SEEKING_REVENGE_10", me, hero7, false)
                end
                local hero8 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero8)
                local hero9 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(me, hero9)
                local hero11 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(hero11, me)
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

