-- Generated native draft: WillDummy. Review coverage report before use.
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
    local angle, bVar3, bVar4, bVar5, cVar2, fVar11, iVar6, iVar8, pCVar7, pCVar9, pThing, piVar1
    local alive = true
    bVar3 = false
    fVar11 = me:GetAngleXY()
    angle = fVar11
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 0xf)
            cVar2 = quest:GetMasterGameState("WillTrainingStarted")
            while cVar2 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                cVar2 = quest:GetMasterGameState("WillTrainingStarted")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                -- LAB_00d434fc: (native jump target)
                if not bVar4 then
                    repeat
                        bVar4 = me:MsgIsHitByHeroSpecialAbility(me)
                        if bVar4 then
                            -- LAB_00d43568: (native jump target)
                            bVar4 = false
                        else
                            bVar3 = true
                            bVar5 = me:MsgIsHitByHero()
                            bVar4 = true
                            if bVar5 then
                                bVar4 = false
                                goto FLOW_after_lab_00d43568
                            end
                        end
                        ::FLOW_after_lab_00d43568::
                        if bVar3 then
                            bVar3 = false
                        end
                        if not bVar4 then goto LAB_00d435c1 end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            return
                        end
                    until false
                end
            end
        end
    end
    ::FLOW_after_lab_00d434fc::
    do return end
    ::LAB_00d435c1::
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    bVar4 = me:MsgIsHitByHeroSpecialAbility(me)
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        quest:EntityPlayObjectAnimation(me, "GET_HIT_SPIN", false)
        -- TODO(native): xStack_94 = angle + (float)0.0;
        quest:EntitySetFacingAngle(me, xStack_94, true)
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        quest:EntitySetFacingAngle(me, angle + 0.0, true)
        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xac)
        -- TODO(native): *piVar1 = *piVar1 + 1;
        iVar6 = quest:GetTimer(quest:GetStateInt("WillHelpTimer"))
        if iVar6 < 1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            pCVar7 = quest:GetThingWithScriptName("WillApprentice")
            bVar4 = (pCVar7 ~= nil and pCVar7:IsAlive())
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar4 then
                if bVar5 then
                    return
                end
                bVar5 = false
                bVar4 = false
                pCVar7 = quest:GetThingWithScriptName("WillApprentice")
                iVar8 = quest:AddNewConversation(pCVar7, bVar4, bVar5)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar8, pCVar7)
                pCVar7 = quest:GetHero()
                pCVar9 = quest:GetThingWithScriptName("WillApprentice")
                quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT", pCVar9, pCVar7, false)
            else
                if bVar5 then
                    return
                end
                bVar5 = false
                bVar4 = false
                pCVar7 = quest:GetThingWithScriptName("TheRealGuildmaster")
                iVar8 = quest:AddNewConversation(pCVar7, bVar4, bVar5)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar8, pCVar7)
                pCVar7 = quest:GetHero()
                pCVar9 = quest:GetThingWithScriptName("TheRealGuildmaster")
                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT", pCVar9, pCVar7, false)
            end
            quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 7)
        end
        quest:EntitySetTargetable(me, false)
        quest:Pause(quest:ReadGlobalGameData(0xf18))
        quest:EntitySetFacingAngle(me, xStack_94, true)
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        quest:EntitySetFacingAngle(me, angle, true)
        quest:EntitySetTargetable(me, true)
    else
        bVar4 = me:MsgIsHitByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
            iVar6 = quest:GetTimer(quest:GetStateInt("WillHelpTimer"))
            if iVar6 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                pCVar7 = quest:GetThingWithScriptName("WillApprentice")
                bVar4 = (pCVar7 ~= nil and pCVar7:IsAlive())
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar4 then
                    if bVar5 then
                        return
                    end
                    bVar5 = false
                    bVar4 = false
                    pCVar7 = quest:GetThingWithScriptName("WillApprentice")
                    iVar8 = quest:AddNewConversation(pCVar7, bVar4, bVar5)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar8, pCVar7)
                    pCVar7 = quest:GetHero()
                    pCVar9 = quest:GetThingWithScriptName("WillApprentice")
                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_WILL_TROUBLE", pCVar9, pCVar7, false)
                else
                    if bVar5 then
                        return
                    end
                    bVar5 = false
                    bVar4 = false
                    pCVar7 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    iVar8 = quest:AddNewConversation(pCVar7, bVar4, bVar5)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar8, pCVar7)
                    pCVar7 = quest:GetHero()
                    pCVar9 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_WILL_TROUBLE", pCVar9, pCVar7, false)
                end
                quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 7)
            end
        end
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        repeat
            bVar4 = me:MsgIsHitByHeroSpecialAbility(me)
            if bVar4 then
                -- LAB_00d43568_c2: (native jump target)
                bVar4 = false
            else
                bVar3 = true
                bVar5 = me:MsgIsHitByHero()
                bVar4 = true
                if bVar5 then return end  -- TODO(native): goto LAB_00d43568_c2
            end
            if bVar3 then
                bVar3 = false
            end
            if not bVar4 then goto LAB_00d435c1 end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
        until false
    end
    goto FLOW_after_lab_00d434fc
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

