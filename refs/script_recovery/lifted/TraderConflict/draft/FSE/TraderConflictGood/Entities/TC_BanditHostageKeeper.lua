-- Generated native draft: TC_BanditHostageKeeper. Review coverage report before use.
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
    local bVar2, conversationID, fVar5, p0, pCVar3, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_20 = resources:NewResource()
    bVar2 = false
    if bVar2 ~= 0 then
    end
    bVar2 = resources:TryAcquire(xStack_20, me, 2)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00dfbcd3 end
        bVar2 = resources:TryAcquire(xStack_20, me, 2)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        fVar5 = 15.0
        pCVar3 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar5)
        while not bVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00dfbcd3 end
            fVar5 = 15.0
            pCVar3 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar5)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            if iVar4 % 5 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00dfbcd3 end
                conversationID = quest:AddNewConversation(me, false, false)
                pCVar3 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, pCVar3)
                if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
                    pCVar3 = quest:GetHero()
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, pCVar3, false)
                elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
                    pCVar3 = quest:GetHero()
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, pCVar3, false)
                else
                    pCVar3 = quest:GetHero()
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, pCVar3, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20")
                end
                if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
                    quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
                else
                    quest:SetStateInt("BanditSecurityLinesSaid", 0)
                end
            end
            bVar2 = false
            if bVar2 ~= 0 then
            end
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
            until not (not bVar2)
        end
    end
    ::LAB_00dfbcd3::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

