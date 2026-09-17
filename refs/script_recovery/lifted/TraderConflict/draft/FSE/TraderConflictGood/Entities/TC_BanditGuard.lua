-- Generated native draft: TC_BanditGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, bVar9, fVar13, iVar11, p0, pCVar10, string, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_20 = resources:NewResource()
    bVar5 = false
    if bVar5 ~= 0 then
    end
    bVar5 = resources:TryAcquire(xStack_20, me, 2)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00dfb974 end
        bVar5 = resources:TryAcquire(xStack_20, me, 2)
    end
    bVar9 = false
    bVar4 = false
    bVar3 = false
    bVar8 = false
    bVar5 = false
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00dfb974 end
    fVar13 = 15.0
    pCVar10 = quest:GetHero()
    bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, fVar13)
    while not bVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00dfb974 end
        fVar13 = 15.0
        pCVar10 = quest:GetHero()
        bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, fVar13)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then goto LAB_00dfb974 end
    if not quest:GetStateBool("CommentedOnBanditCostume") then
        bVar9 = false
        bVar4 = false
        bVar3 = false
        bVar8 = false
        bVar5 = true
        pCVar10 = quest:GetHero()
        bVar6 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_SHIRT_BANDITCAMP")
        if not bVar6 then
            bVar6 = false
            goto FLOW_after_lab_00dfb588
        end
        bVar9 = true
        bVar4 = false
        bVar3 = false
        bVar8 = false
        bVar5 = true
        pCVar10 = quest:GetHero()
        bVar6 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_TROUSERS_BANDITCAMP")
        if not bVar6 then
            bVar6 = false
            goto FLOW_after_lab_00dfb588
        end
        bVar9 = true
        bVar4 = true
        bVar3 = false
        bVar8 = false
        bVar5 = true
        pCVar10 = quest:GetHero()
        bVar6 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_HAT_BANDITCAMP")
        if not bVar6 then
            bVar6 = false
            goto FLOW_after_lab_00dfb588
        end
        bVar9 = true
        bVar4 = true
        bVar3 = true
        bVar8 = false
        bVar5 = true
        pCVar10 = quest:GetHero()
        bVar6 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_BOOTS_BANDITCAMP")
        if not bVar6 then
            bVar6 = false
            goto FLOW_after_lab_00dfb588
        end
        bVar9 = true
        bVar4 = true
        bVar3 = true
        bVar8 = true
        bVar5 = true
        pCVar10 = quest:GetHero()
        bVar7 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_GLOVES_BANDITCAMP")
        bVar6 = true
        if not bVar7 then
            bVar6 = false
            goto FLOW_after_lab_00dfb588
        end
    else
        -- LAB_00dfb588: (native jump target)
        bVar6 = false
    end
    ::FLOW_after_lab_00dfb588::
    if bVar8 then
    end
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar9 then
    end
    if bVar5 then
    end
    bVar4 = false
    bVar3 = false
    bVar8 = false
    bVar5 = false
    if bVar6 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00dfb974 end
        iVar11 = quest:AddNewConversation(me, false, false)
        pCVar10 = quest:GetHero()
        quest:AddPersonToConversation(iVar11, pCVar10)
        string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_BANDIT_COSTUME_10"
        -- LAB_00dfb630: (native jump target)
        pCVar10 = quest:GetHero()
        quest:AddLineToConversation(iVar11, string, me, pCVar10, false)
        quest:SetStateBool("CommentedOnBanditCostume", true)
    else
        if not quest:GetStateBool("CommentedOnBanditCostume") then
            bVar4 = false
            bVar3 = false
            pCVar10 = quest:GetHero()
            bVar8 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_SHIRT_BANDITCAMP")
            if not bVar8 then
                bVar4 = false
                bVar3 = false
                bVar5 = true
                pCVar10 = quest:GetHero()
                bVar8 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_TROUSERS_BANDITCAMP")
                if not bVar8 then
                    bVar4 = false
                    bVar3 = false
                    bVar5 = true
                    pCVar10 = quest:GetHero()
                    bVar8 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_HAT_BANDITCAMP")
                    if not bVar8 then
                        bVar4 = true
                        bVar3 = false
                        bVar5 = true
                        pCVar10 = quest:GetHero()
                        bVar8 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_BOOTS_BANDITCAMP")
                        if not bVar8 then
                            bVar4 = true
                            bVar3 = true
                            bVar8 = true
                            bVar5 = true
                            pCVar10 = quest:GetHero()
                            bVar9 = quest:IsWearingClothingItem(pCVar10, "OBJECT_HERO_GLOVES_BANDITCAMP")
                            if not bVar9 then
                                bVar9 = false
                                goto FLOW_after_lab_00dfb79f
                            end
                        end
                    end
                end
            end
            bVar8 = true
            bVar9 = true
        else
            -- LAB_00dfb79f: (native jump target)
            bVar9 = false
        end
        ::FLOW_after_lab_00dfb79f::
        if bVar3 then
        end
        if bVar4 then
        end
        if bVar5 then
        end
        if bVar8 then
        end
        if bVar9 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00dfb974 end
            iVar11 = quest:AddNewConversation(me, false, false)
            pCVar10 = quest:GetHero()
            quest:AddPersonToConversation(iVar11, pCVar10)
            string = "TEXT_QST_B11_BANDIT_ATTACK_WEARING_PART_BANDIT_COSTUME_10"
            pCVar10 = quest:GetHero()
            quest:AddLineToConversation(iVar11, string, me, pCVar10, false)
            quest:SetStateBool("CommentedOnBanditCostume", true)
            goto FLOW_after_lab_00dfb630
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00dfb974 end
        iVar11 = quest:AddNewConversation(me, false, false)
        pCVar10 = quest:GetHero()
        quest:AddPersonToConversation(iVar11, pCVar10)
        if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
            pCVar10 = quest:GetHero()
            quest:AddLineToConversation(iVar11, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, pCVar10, false)
        elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
            pCVar10 = quest:GetHero()
            quest:AddLineToConversation(iVar11, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, pCVar10, false)
        else
            pCVar10 = quest:GetHero()
            quest:AddLineToConversation(iVar11, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, pCVar10, false)
        end
        if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
            quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
        else
            quest:SetStateInt("BanditSecurityLinesSaid", 0)
        end
    end
    ::FLOW_after_lab_00dfb630::
    bVar5 = false
    if bVar5 ~= 0 then
    end
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until not (not bVar5)
    ::LAB_00dfb974::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

