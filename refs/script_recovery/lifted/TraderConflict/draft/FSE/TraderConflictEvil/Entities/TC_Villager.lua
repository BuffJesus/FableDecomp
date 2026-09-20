-- Generated native draft: TC_Villager. Review coverage report before use.
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
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, cVar1, conversationID, dist, iVar10, p0, pCVar9, pThing1
    local alive = true
    bVar7 = false
    bVar4 = false
    bVar3 = false
    bVar6 = false
    bVar8 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        bVar5 = quest:IsRegionLoaded("BarrowFields")
        if not bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        cVar1 = quest:GetStateBool("QuestStartScreened")
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            cVar1 = quest:GetStateBool("QuestStartScreened")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            quest:EntitySetAsScared(me, true)
            quest:ClearThingHasInformation(me)
            quest:EntitySetCombatEnabled(me, false)
            pCVar9 = quest:GetHero()
            quest:EntityUnsetThingAsAllyOfThing(me, pCVar9)
            pThing1 = quest:GetHero()
            quest:EntityUnsetThingAsAllyOfThing(pThing1, me)
            cVar1 = quest:GetStateBool("PlayerEngaged")
            while not cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                bVar5 = me:MsgIsHitBy("")
                if bVar5 then
                    bVar3 = true
                    bVar5 = me:MsgIsHitByHero()
                    if bVar5 then goto LAB_00df9372 end
                    bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar6 then
                        bVar3 = true
                        bVar6 = true
                        bVar8 = true
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00df9372 end
                    end
                    bVar3 = true
                    bVar6 = true
                    bVar5 = true
                else
                    goto LAB_00df9372
                end
                goto FLOW_past_lab_00df9372
                ::LAB_00df9372::
                bVar5 = false
                ::FLOW_past_lab_00df9372::
                if bVar8 then
                    bVar8 = false
                end
                if bVar6 then
                    bVar6 = false
                end
                if bVar3 then
                    bVar3 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    quest:ModifyThingHealth(me, 100.0, false)
                else
                    bVar5 = me:MsgIsHitByHero()
                    if bVar5 then
                        goto LAB_00df9462
                    else
                        bVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar7 then
                            bVar7 = true
                            bVar4 = true
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar5 then goto LAB_00df9462 end
                        end
                        bVar7 = true
                        bVar5 = false
                    end
                    goto FLOW_past_lab_00df9462
                    ::LAB_00df9462::
                    bVar5 = true
                    ::FLOW_past_lab_00df9462::
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar7 then
                        bVar7 = false
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        quest:SetStateBool("PlayerEngaged", true)
                    end
                end
                cVar1 = quest:GetStateBool("PlayerEngaged")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if not bVar8 then
                alive = not quest:IsActiveThreadTerminating()
                bVar8 = not alive
                while not bVar8 do
                    iVar10 = quest:GetTimer(quest:GetStateInt("ScreamOutTimer"))
                    if iVar10 == 0 then
                        dist = 15.0
                        pCVar9 = quest:GetHero()
                        bVar8 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, dist)
                        if bVar8 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar8 = not alive
                            if bVar8 then
                                return
                            end
                            conversationID = quest:AddNewConversation(me, false, false)
                            pCVar9 = quest:GetHero()
                            quest:AddPersonToConversation(conversationID, pCVar9)
                            iVar10 = quest:EntityGetSex(me)
                            if iVar10 == 2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    return
                                end
                                pCVar9 = quest:GetHero()
                                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_FEMALE_PANIC", me, pCVar9, false)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar8 then
                                    return
                                end
                                pCVar9 = quest:GetHero()
                                quest:AddLineToConversation(conversationID, "TEXT_QST_B12_VILLAGER_MALE_PANIC", me, pCVar9, false)
                            end
                            iVar10 = math.random(0, 32767)
                            quest:SetTimer(quest:GetStateInt("ScreamOutTimer"), iVar10 % 0xf + 0xf)
                        end
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                end
            end
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

