-- Readable native conversion: TC_BanditGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TC_BanditGuard.Main (retail 0x00dfb320)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult3, conversationId, string
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 2) do
        if not quest:NewScriptFrame(me) then goto LAB_00dfb974 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00dfb974 end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
        if not quest:NewScriptFrame(me) then goto LAB_00dfb974 end
    end
    if quest:GetStateBool("CommentedOnBanditCostume") then goto LAB_00dfb588 end
    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then goto LAB_00dfb588 end
    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then goto LAB_00dfb588 end
    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then goto LAB_00dfb588 end
    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then goto LAB_00dfb588 end
    predicateResult = true
    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00dfb588 end
    goto FLOW_past_lab_00dfb588
    ::LAB_00dfb588::
    predicateResult = false
    ::FLOW_past_lab_00dfb588::
    if predicateResult then
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_BANDIT_COSTUME_10"
        goto LAB_00dfb630
    else
        if quest:GetStateBool("CommentedOnBanditCostume") then goto LAB_00dfb79f end
        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then
            if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
                if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then
                        if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00dfb79f end
                    end
                end
            end
        end
        predicateResult3 = true
        goto FLOW_past_lab_00dfb79f
        ::LAB_00dfb79f::
        predicateResult3 = false
        ::FLOW_past_lab_00dfb79f::
        if predicateResult3 then
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_PART_BANDIT_COSTUME_10"
            goto LAB_00dfb630
        end
        local conversationId2 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId2, hero)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        else
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
        end
        if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
            quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
        else
            quest:SetStateInt("BanditSecurityLinesSaid", 0)
        end
    end
    goto FLOW_past_lab_00dfb630
    ::LAB_00dfb630::
    quest:AddLineToConversation(conversationId, string, me, hero, false)
    quest:SetStateBool("CommentedOnBanditCostume", true)
    ::FLOW_past_lab_00dfb630::
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::LAB_00dfb974::
    resources:ReleaseResource(resource)
end

-- TC_BanditGuard.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_BanditGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_BanditGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

