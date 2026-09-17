-- Generated native draft: TC_BanditFighter. Review coverage report before use.
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
    local CVar6, CVar7, bVar2, cVar1, conversationID, fVar8, iVar5, p0, pCVar3, pCVar4, r1, xStack_2c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
        quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
        pCVar3 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, pCVar3)
        pCVar4 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(pCVar4, me)
        cVar1 = quest:GetStateBool("QuestStartScreened")
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            cVar1 = quest:GetStateBool("QuestStartScreened")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            pCVar3 = quest:GetNearestWithScriptName(me, "IsAGuard")
            quest:GiveThingBestEnemyTarget(me, pCVar3)
            cVar1 = quest:GetStateBool("PlayerEngaged")
            CVar7 = 0x0
            while not cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                CVar6 = (CVar7 | 1)
                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                bVar2 = me:MsgIsHitBy("")
                if bVar2 then
                    CVar6 = (CVar7 | 3)
                    bVar2 = me:MsgIsHitByHero()
                    if bVar2 then
                        bVar2 = false
                        goto FLOW_after_lab_00df8b83
                    end
                    CVar6 = (CVar7 | 7)
                    bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar2 then
                        CVar6 = (CVar7 | 0xf)
                        bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                        if not bVar2 then
                            bVar2 = false
                            goto FLOW_after_lab_00df8b83
                        end
                    end
                    bVar2 = true
                else
                    -- LAB_00df8b83: (native jump target)
                    bVar2 = false
                end
                ::FLOW_after_lab_00df8b83::
                if (CVar6 & 8) ~= 0 then
                    CVar6 = (CVar6 & 0xfffffff7)
                end
                if (CVar6 & 4) ~= 0 then
                    CVar6 = (CVar6 & 0xfffffffb)
                end
                if (CVar6 & 2) ~= 0 then
                    CVar6 = (CVar6 & 0xfffffffd)
                end
                if (CVar6 & 1) ~= 0 then
                    CVar6 = (CVar6 & 0xfffffffe)
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    quest:ModifyThingHealth(me, 100.0, false)
                else
                    CVar7 = (CVar6 | 0x10)
                    bVar2 = me:MsgIsHitByHero()
                    if bVar2 then
                        -- LAB_00df8c73: (native jump target)
                        bVar2 = true
                        CVar6 = CVar7
                    else
                        CVar7 = (CVar6 | 0x30)
                        bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar2 then
                            CVar7 = (CVar6 | 0x70)
                            bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                            if not bVar2 then
                                bVar2 = true
                                CVar6 = CVar7
                                goto FLOW_after_lab_00df8c73
                            end
                        end
                        bVar2 = false
                        CVar6 = CVar7
                    end
                    ::FLOW_after_lab_00df8c73::
                    if (CVar6 & 0x40) ~= 0 then
                        CVar6 = (CVar6 & 0xffffffbf)
                    end
                    if (CVar6 & 0x20) ~= 0 then
                        CVar6 = (CVar6 & 0xffffffdf)
                    end
                    if (CVar6 & 0x10) ~= 0 then
                        CVar6 = (CVar6 & 0xffffffef)
                    end
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        quest:SetStateBool("PlayerEngaged", true)
                    end
                end
                fVar8 = 15.0
                pCVar3 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar8)
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    quest:SetStateBool("PlayerEngaged", true)
                end
                CVar7 = CVar6
                cVar1 = quest:GetStateBool("PlayerEngaged")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                cVar1 = quest:GetStateBool("MissionSucceeded")
                while not cVar1 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    CVar6 = (CVar7 | 0x80)
                    xStack_2c = CVar6
                    bVar2 = me:MsgIsHitByHero()
                    if bVar2 then
                        -- LAB_00df8dcb: (native jump target)
                        bVar2 = true
                    else
                        CVar6 = (CVar7 | 0x180)
                        xStack_2c = CVar6
                        bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar2 then
                            CVar6 = (CVar7 | 0x380)
                            xStack_2c = CVar6
                            bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                            if not bVar2 then
                                bVar2 = true
                                goto FLOW_after_lab_00df8dcb
                            end
                        end
                        bVar2 = false
                    end
                    ::FLOW_after_lab_00df8dcb::
                    if (CVar6 & 0x200) ~= 0 then
                        CVar6 = (CVar6 & 0xfffffdff)
                        xStack_2c = CVar6
                    end
                    if (CVar6 & 0x100) ~= 0 then
                        CVar6 = (CVar6 & 0xfffffeff)
                        xStack_2c = CVar6
                    end
                    if CVar6_b0 < 0 then
                        CVar6 = (CVar6 & 0xffffff7f)
                        xStack_2c = CVar6
                    end
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        if not __native_entity_state:GetStateBool("HitWarning") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                            conversationID = quest:AddNewConversation(me, false, false)
                            pCVar3 = quest:GetHero()
                            quest:AddPersonToConversation(conversationID, pCVar3)
                            pCVar3 = quest:GetHero()
                            quest:AddLineToConversation(conversationID, "TEXT_QST_B12_BANDIT_ON_HIT_10", me, pCVar3, false)
                            __native_entity_state:SetStateBool("HitWarning", true)
                            CVar6 = xStack_2c
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                            r1 = quest:GetNearestWithScriptName(me, "TC_BanditFighter")
                            bVar2 = quest:IsDistanceBetweenThingsUnder(me, r1, 15.0)
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    return
                                end
                                quest:SetStateBool("HeroAttackedBandit", true)
                            end
                        end
                    end
                    if quest:GetStateBool("HeroAttackedBandit") then
                        fVar8 = 15.0
                        pCVar3 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar8)
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                            if iVar5 % 5 == 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    return
                                end
                                iVar5 = quest:AddNewConversation(me, false, false)
                                pCVar3 = quest:GetHero()
                                quest:AddPersonToConversation(iVar5, pCVar3)
                                pCVar3 = quest:GetHero()
                                quest:AddLineToConversation(iVar5, "TEXT_QST_B12_BANDIT_SEEKING_REVENGE_10", me, pCVar3, false)
                            end
                            pCVar3 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(me, pCVar3)
                            pCVar3 = quest:GetHero()
                            quest:EntityUnsetThingAsAllyOfThing(me, pCVar3)
                            pCVar4 = quest:GetHero()
                            quest:EntityUnsetThingAsAllyOfThing(pCVar4, me)
                            cVar1 = quest:GetStateBool("MissionSucceeded")
                            while not cVar1 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    return
                                end
                                cVar1 = quest:GetStateBool("MissionSucceeded")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                        end
                    end
                    CVar7 = CVar6
                    cVar1 = quest:GetStateBool("MissionSucceeded")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:RemoveThing(me, false, true)
                end
            end
        end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HitWarning", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

