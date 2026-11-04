-- Readable native conversion: TC_BanditGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TC_BanditGuard.Main (retail 0x00dfb320)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6, isDistanceBetweenThingsUnder, predicateResult8, predicateResult9
    local predicateResult10, predicateResult11, predicateResult12, conversationId, hero3, hero4
    local hero5, hero6, hero7, hero9, hero19, string
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 2) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    predicateResult11 = false
    predicateResult3 = false
    predicateResult = false
    predicateResult9 = false
    predicateResult5 = false
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local hero = quest:GetHero()
    isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 15.0)
    while not isDistanceBetweenThingsUnder do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        local hero2 = quest:GetHero()
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero2, 15.0)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if not (quest:GetStateBool("CommentedOnBanditCostume")) then
        predicateResult11 = false
        predicateResult3 = false
        predicateResult = false
        predicateResult9 = false
        predicateResult5 = true
        hero3 = quest:GetHero()
        if not quest:IsWearingClothingItem(hero3, "OBJECT_HERO_SHIRT_BANDITCAMP") then goto LAB_00dfb588 end
        predicateResult11 = true
        predicateResult3 = false
        predicateResult = false
        predicateResult9 = false
        predicateResult5 = true
        hero4 = quest:GetHero()
        if not quest:IsWearingClothingItem(hero4, "OBJECT_HERO_TROUSERS_BANDITCAMP") then goto LAB_00dfb588 end
        predicateResult11 = true
        predicateResult3 = true
        predicateResult = false
        predicateResult9 = false
        predicateResult5 = true
        hero5 = quest:GetHero()
        if not quest:IsWearingClothingItem(hero5, "OBJECT_HERO_HAT_BANDITCAMP") then goto LAB_00dfb588 end
        predicateResult11 = true
        predicateResult3 = true
        predicateResult = true
        predicateResult9 = false
        predicateResult5 = true
        hero6 = quest:GetHero()
        if not quest:IsWearingClothingItem(hero6, "OBJECT_HERO_BOOTS_BANDITCAMP") then goto LAB_00dfb588 end
        predicateResult11 = true
        predicateResult3 = true
        predicateResult = true
        predicateResult9 = true
        predicateResult5 = true
        hero7 = quest:GetHero()
        predicateResult8 = true
        if not quest:IsWearingClothingItem(hero7, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00dfb588 end
        goto FLOW_past_lab_00dfb588
    end
    ::LAB_00dfb588::
    predicateResult8 = false
    ::FLOW_past_lab_00dfb588::
    if predicateResult9 then
    end
    if predicateResult then
    end
    if predicateResult3 then
    end
    if predicateResult11 then
    end
    if predicateResult5 then
    end
    predicateResult4 = false
    predicateResult2 = false
    predicateResult10 = false
    predicateResult6 = false
    if predicateResult8 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        conversationId = quest:AddNewConversation(me, false, false)
        local hero8 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero8)
        string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_BANDIT_COSTUME_10"
        goto LAB_00dfb630
    else
        if not (quest:GetStateBool("CommentedOnBanditCostume")) then
            predicateResult4 = false
            predicateResult2 = false
            hero9 = quest:GetHero()
            if not quest:IsWearingClothingItem(hero9, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                predicateResult4 = false
                predicateResult2 = false
                predicateResult6 = true
                local hero10 = quest:GetHero()
                if not quest:IsWearingClothingItem(hero10, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
                    predicateResult4 = false
                    predicateResult2 = false
                    predicateResult6 = true
                    local hero11 = quest:GetHero()
                    if not quest:IsWearingClothingItem(hero11, "OBJECT_HERO_HAT_BANDITCAMP") then
                        predicateResult4 = true
                        predicateResult2 = false
                        predicateResult6 = true
                        local hero12 = quest:GetHero()
                        if not quest:IsWearingClothingItem(hero12, "OBJECT_HERO_BOOTS_BANDITCAMP") then
                            predicateResult4 = true
                            predicateResult2 = true
                            predicateResult10 = true
                            predicateResult6 = true
                            local hero13 = quest:GetHero()
                            if not quest:IsWearingClothingItem(hero13, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00dfb79f end
                        end
                    end
                end
            end
            predicateResult10 = true
            predicateResult12 = true
            goto FLOW_past_lab_00dfb79f
        end
        ::LAB_00dfb79f::
        predicateResult12 = false
        ::FLOW_past_lab_00dfb79f::
        if predicateResult2 then
        end
        if predicateResult4 then
        end
        if predicateResult6 then
        end
        if predicateResult10 then
        end
        if predicateResult12 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            conversationId = quest:AddNewConversation(me, false, false)
            local hero14 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero14)
            string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_PART_BANDIT_COSTUME_10"
            goto LAB_00dfb630
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        local conversationId2 = quest:AddNewConversation(me, false, false)
        local hero15 = quest:GetHero()
        quest:AddPersonToConversation(conversationId2, hero15)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            local hero16 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero16, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            local hero17 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero17, false)
        else
            local hero18 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero18, false)
        end
        if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
            quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
        else
            quest:SetStateInt("BanditSecurityLinesSaid", 0)
        end
    end
    goto FLOW_past_lab_00dfb630
    ::LAB_00dfb630::
    hero19 = quest:GetHero()
    quest:AddLineToConversation(conversationId, string, me, hero19, false)
    quest:SetStateBool("CommentedOnBanditCostume", true)
    ::FLOW_past_lab_00dfb630::
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
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

