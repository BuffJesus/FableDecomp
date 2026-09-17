-- Readable native conversion: TC_BanditFighter. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_BanditFighter.Main (retail 0x00df8970)
function Main(quest, me)
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, predicateResult3
    local predicateResult4, predicateResult6, playerEngaged, conversationID, conversationId
    local scratchValue10
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
    quest:EntitySetThingAsAllyOfThing(quest:GetHero(), me)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:GiveThingBestEnemyTarget(me, quest:GetNearestWithScriptName(me, "IsAGuard"))
    playerEngaged = quest:GetStateBool("PlayerEngaged")
    scratchValue3 = 0
    while not playerEngaged do
        if not quest:NewScriptFrame(me) then return end
        scratchValue = scratchValue3 | 1
        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
        if me:MsgIsHitBy("") then
            scratchValue = scratchValue3 | 3
            if me:MsgIsHitByHero() then
                predicateResult3 = false
                goto FLOW_after_lab_00df8b83
            end
            scratchValue = scratchValue3 | 7
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = scratchValue3 | 15
                if not me:MsgIsHitByHeroSpecialAbility(me) then
                    predicateResult3 = false
                    goto FLOW_after_lab_00df8b83
                end
            end
            predicateResult3 = true
        else
            predicateResult3 = false
        end
        ::FLOW_after_lab_00df8b83::
        if scratchValue & 8 ~= 0 then
            scratchValue = scratchValue & 0xfffffff7
        end
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
        end
        if scratchValue & 2 ~= 0 then
            scratchValue = scratchValue & 0xfffffffd
        end
        if scratchValue & 1 ~= 0 then
            scratchValue = scratchValue & 0xfffffffe
        end
        if predicateResult3 then
            if quest:IsActiveThreadTerminating() then return end
            quest:ModifyThingHealth(me, 100.0, false)
        else
            scratchValue4 = scratchValue | 16
            if me:MsgIsHitByHero() then
                predicateResult4 = true
                scratchValue = scratchValue4
            else
                scratchValue5 = scratchValue | 48
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue5 = scratchValue | 112
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        predicateResult4 = true
                        scratchValue = scratchValue5
                        goto FLOW_after_lab_00df8c73
                    end
                end
                predicateResult4 = false
                scratchValue = scratchValue5
            end
            ::FLOW_after_lab_00df8c73::
            if scratchValue & 64 ~= 0 then
                scratchValue = scratchValue & 0xffffffbf
            end
            if scratchValue & 32 ~= 0 then
                scratchValue = scratchValue & 0xffffffdf
            end
            if scratchValue & 16 ~= 0 then
                scratchValue = scratchValue & 0xffffffef
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("PlayerEngaged", true)
            end
        end
        if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 15.0) then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("PlayerEngaged", true)
        end
        scratchValue3 = scratchValue
        playerEngaged = quest:GetStateBool("PlayerEngaged")
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame(me) then return end
        scratchValue2 = scratchValue3 | 128
        scratchValue10 = scratchValue2
        if me:MsgIsHitByHero() then
            predicateResult6 = true
        else
            scratchValue2 = scratchValue3 | 384
            scratchValue10 = scratchValue2
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue2 = scratchValue3 | 896
                scratchValue10 = scratchValue2
                if not me:MsgIsHitByHeroSpecialAbility(me) then
                    predicateResult6 = true
                    goto FLOW_after_lab_00df8dcb
                end
            end
            predicateResult6 = false
        end
        ::FLOW_after_lab_00df8dcb::
        if scratchValue2 & 512 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffdff
            scratchValue10 = scratchValue2
        end
        if scratchValue2 & 256 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffeff
            scratchValue10 = scratchValue2
        end
        if CVar6_b0 < 0 then
            scratchValue2 = scratchValue2 & 0xffffff7f
            scratchValue10 = scratchValue2
        end
        if predicateResult6 then
            if not state:GetBool("HitWarning") then
                conversationID = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationID, quest:GetHero())
                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_BANDIT_ON_HIT_10", me, quest:GetHero(), false)
                state:SetBool("HitWarning", true)
                scratchValue2 = scratchValue10
            elseif quest:IsDistanceBetweenThingsUnder(me, quest:GetNearestWithScriptName(me, "TC_BanditFighter"), 15.0) then
                quest:SetStateBool("HeroAttackedBandit", true)
            end
        end
        if quest:GetStateBool("HeroAttackedBandit") then
            if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 15.0) then
                if quest:IsActiveThreadTerminating() then return end
                if conversationId % 5 == 0 then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_SEEKING_REVENGE_10", me, quest:GetHero(), false)
                end
                quest:GiveThingBestEnemyTarget(me, quest:GetHero())
                quest:EntityUnsetThingAsAllyOfThing(me, quest:GetHero())
                quest:EntityUnsetThingAsAllyOfThing(quest:GetHero(), me)
                while not quest:GetStateBool("MissionSucceeded") do
                    if not quest:NewScriptFrame(me) then return end
                end
                if quest:IsActiveThreadTerminating() then return end
            end
        end
        scratchValue3 = scratchValue2
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, false, true)
end

-- TC_BanditFighter.Init (retail 0x00df8940)
function Init(quest, me)
    state:SetBool("HitWarning", false)
end

-- TC_BanditFighter.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_BanditFighter.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

