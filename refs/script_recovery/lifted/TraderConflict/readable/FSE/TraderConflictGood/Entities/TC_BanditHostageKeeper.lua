-- Readable native conversion: TC_BanditHostageKeeper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_BanditHostageKeeper.Main (retail 0x00dfba60)
function Main(quest, me)
    local hero = quest:GetHero()
    local conversationID
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(2) then goto LAB_00dfbcd3 end
    if not quest:IsActiveThreadTerminating() then
        while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
            if not quest:NewScriptFrame(me) then goto LAB_00dfbcd3 end
        end
        if not quest:IsActiveThreadTerminating() then
            if math.random(0, 32767) % 5 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00dfbcd3 end
                conversationID = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationID, hero)
                if quest:GetStateInt("BanditSecurityLinesSaid") == 0 then
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", me, hero, false)
                elseif quest:GetStateInt("BanditSecurityLinesSaid") == 1 then
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
                else
                    quest:AddLineToConversation(conversationID, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", me, hero, false)
                end
                if quest:GetStateInt("BanditSecurityLinesSaid") < 1 then
                    quest:SetStateInt("BanditSecurityLinesSaid", quest:GetStateInt("BanditSecurityLinesSaid") + 1)
                else
                    quest:SetStateInt("BanditSecurityLinesSaid", 0)
                end
            end
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
    end
    ::LAB_00dfbcd3::
    me:ReleaseControl()
end

-- TC_BanditHostageKeeper.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_BanditHostageKeeper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_BanditHostageKeeper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

