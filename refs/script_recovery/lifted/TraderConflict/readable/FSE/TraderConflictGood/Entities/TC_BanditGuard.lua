-- Readable native conversion: TC_BanditGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_BanditGuard.Main (retail 0x00dfb320)
function Main(quest, me)
    local hero = quest:GetHero()
    local predicateResult, predicateResult3, conversationId, conversationId2, conversationId3
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(2) then goto LAB_00dfb974 end
    if quest:IsActiveThreadTerminating() then goto LAB_00dfb974 end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00dfb974 end
    end
    if not quest:GetStateBool("CommentedOnBanditCostume") then
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then
            predicateResult = false
            goto FLOW_after_lab_00dfb588
        end
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
            predicateResult = false
            goto FLOW_after_lab_00dfb588
        end
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then
            predicateResult = false
            goto FLOW_after_lab_00dfb588
        end
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then
            predicateResult = false
            goto FLOW_after_lab_00dfb588
        end
        predicateResult = true
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then
            predicateResult = false
            goto FLOW_after_lab_00dfb588
        end
    else
        predicateResult = false
    end
    ::FLOW_after_lab_00dfb588::
    if predicateResult then
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:AddLineToConversation(conversationId, "TEXT_QST_B11_BANDIT_ATTACK_WEARING_BANDIT_COSTUME_10", me, hero, false)
        quest:SetStateBool("CommentedOnBanditCostume", true)
    else
        if not quest:GetStateBool("CommentedOnBanditCostume") then
            if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then
                        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then
                            if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then
                                predicateResult3 = false
                                goto FLOW_after_lab_00dfb79f
                            end
                        end
                    end
                end
            end
            predicateResult3 = true
        else
            predicateResult3 = false
        end
        ::FLOW_after_lab_00dfb79f::
        if predicateResult3 then
            conversationId2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId2, hero)
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_ATTACK_WEARING_PART_BANDIT_COSTUME_10", me, hero, false)
            quest:SetStateBool("CommentedOnBanditCostume", true)
            goto FLOW_after_lab_00dfb630
        end
        conversationId3 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId3, hero)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            quest:AddLineToConversation(conversationId3, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            quest:AddLineToConversation(conversationId3, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        else
            quest:AddLineToConversation(conversationId3, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        end
        if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
            quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
        else
            quest:SetStateInt("BanditSecurityLinesSaid", 0)
        end
    end
    ::FLOW_after_lab_00dfb630::
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::LAB_00dfb974::
    me:ReleaseControl()
end

-- TC_BanditGuard.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_BanditGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_BanditGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

